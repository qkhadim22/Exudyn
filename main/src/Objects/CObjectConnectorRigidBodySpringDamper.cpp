/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorRigidBodySpringDamper
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorRigidBodySpringDamper.h"
#include "Linalg/RigidBodyMath.h"

void CObjectConnectorRigidBodySpringDamper::ComputeSpringForceTorque(const MarkerDataStructure& markerData, Index itemIndex,
	Matrix3D& A0all, Vector3D& vLocPos, Vector3D& vLocVel, Vector3D& vLocRot, Vector3D& vLocAngVel, Vector6D& fLocVec6D) const
{
	//spring is measured relative to marker0:

	const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Matrix3D& A0off = parameters.rotationMarker0;
	const Matrix3D& A1off = parameters.rotationMarker1;
	//Matrix3D A0all = (A0off * A0);
	//Matrix3D A1all = (A1off * A1);
	A0all = (A0*A0off);
	Matrix3D A1all = (A1*A1off);

	//relative position, spring length and inverse spring length
	vLocPos = A0all.GetTransposed()*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //vLocPos transformed to marker0 local coordinate system, where springs are defined
	vLocVel = A0all.GetTransposed()*(markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	//relative rotation
	Matrix3D relRot = A0all.GetTransposed() * A1all;
	vLocRot = RigidBodyMath::RotationMatrix2RotXYZ(relRot);

	vLocAngVel = A0all.GetTransposed()*(A1*markerData.GetMarkerData(1).angularVelocityLocal - A0 * markerData.GetMarkerData(0).angularVelocityLocal);


	if (vLocRot[0] > EXUstd::pi) { vLocRot[0] -= 2.*EXUstd::pi; }
	if (vLocRot[1] > EXUstd::pi) { vLocRot[1] -= 2.*EXUstd::pi; }
	if (vLocRot[2] > EXUstd::pi) { vLocRot[2] -= 2.*EXUstd::pi; }

	//compute resulting displacement vector:
	Vector6D uLoc6D;
	Vector6D vLoc6D;
	for (Index i = 0; i < 3; i++)
	{
		uLoc6D[i] = vLocPos[i];
		uLoc6D[i + 3] = vLocRot[i];
		vLoc6D[i] = vLocVel[i];
		vLoc6D[i + 3] = vLocAngVel[i];
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//evaluate force-torque vector:
	//compute resulting force vector:
	if (!parameters.springForceTorqueUserFunction)
	{
		uLoc6D -= parameters.offset;

		EXUmath::MultMatrixVector(parameters.stiffness, uLoc6D, fLocVec6D);

		Vector6D temp;
		EXUmath::MultMatrixVector(parameters.damping, vLoc6D, temp);
		fLocVec6D += temp;
	}
	else
	{
		EvaluateUserFunctionForce(fLocVec6D, cSystemData->GetMainSystemBacklink(), markerData.GetTime(), itemIndex, uLoc6D, vLoc6D);
	}
}


//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectConnectorRigidBodySpringDamper::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorRigidBodySpringDamper::ComputeODE2LHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Lhs) vector
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.);

	//pout << "test\n";
	if (parameters.activeConnector)
	{
		Vector3D vLocPos;
		Vector3D vLocVel;
		Vector3D vLocRot;
		Vector3D vLocAngVel;
		Vector6D fLocVec6D;
		Matrix3D A0all;
		ComputeSpringForceTorque(markerData, objectNumber, A0all, vLocPos, vLocVel, vLocRot, vLocAngVel, fLocVec6D);

		LinkedDataVector fPosLoc(fLocVec6D, 0, 3);
		LinkedDataVector fRotLoc(fLocVec6D, 3, 3);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//transform to global coordinates (application of forces/torques always global):
		Vector3D fPos;
		Vector3D fRot;
		EXUmath::MultMatrixVector(A0all, fPosLoc, fPos);
		EXUmath::MultMatrixVector(A0all, fRotLoc, fRot);

		//now link ode2Lhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			//positionJacobian.NumberOfColumns() == rotationJacobian.NumberOfColumns()
			LinkedDataVector ldv1(ode2Lhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).positionJacobian, fPos, ldv1);
			EXUmath::MultMatrixTransposedVectorAdd(markerData.GetMarkerData(1).rotationJacobian, fRot, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			fPos *= -1.;
			fRot *= -1.;
			LinkedDataVector ldv0(ode2Lhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, fPos, ldv0);
			EXUmath::MultMatrixTransposedVectorAdd(markerData.GetMarkerData(0).rotationJacobian, fRot, ldv0);
		}
		//pout << "  ode2Lhs=" << ode2Lhs << "\n";
	}

}



//! provide according output variable in "value"
void CObjectConnectorRigidBodySpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	Vector3D vLocPos;
	Vector3D vLocVel;
	Vector3D vLocRot;
	Vector3D vLocAngVel;
	Vector6D fLocVec6D;
	Matrix3D A0all;
	ComputeSpringForceTorque(markerData, itemIndex, A0all, vLocPos, vLocVel, vLocRot, vLocAngVel, fLocVec6D);

	LinkedDataVector fPosLoc(fLocVec6D, 0, 3);
	LinkedDataVector fRotLoc(fLocVec6D, 3, 3);

	switch (variableType)
	{
	case OutputVariableType::DisplacementLocal: value.CopyFrom(vLocPos); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(vLocVel); break;
	case OutputVariableType::Rotation: value.CopyFrom(vLocRot); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(vLocAngVel); break;
	case OutputVariableType::ForceLocal: value.CopyFrom(fPosLoc); break;
	case OutputVariableType::TorqueLocal: value.CopyFrom(fRotLoc); break;
	default:
		SysError("CObjectConnectorRigidBodySpringDamper::GetOutputVariableConnector failed"); //error should not occur, because types are checked!
	}
}


//! function called after Newton method; returns a residual error (force); 
Real CObjectConnectorRigidBodySpringDamper::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, Index itemIndex, PostNewtonFlags::Type& flags, Real& recommendedStepSize)
{
	Real discontinuousError = 0;
	flags = PostNewtonFlags::_None;
	if (parameters.postNewtonStepUserFunction && parameters.activeConnector && parameters.nodeNumber != EXUstd::InvalidIndex)
	{
		const Index overheadData = 2; //number of additional values in return value of postNewtonStepUserFunction
		LinkedDataVector dataCoordinates = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current);

		Vector3D vLocPos;
		Vector3D vLocVel;
		Vector3D vLocRot;
		Vector3D vLocAngVel;
		Vector6D fLocVec6D;
		Matrix3D A0all;

		//adds some overhead ...
		ComputeSpringForceTorque(markerDataCurrent, itemIndex, A0all, vLocPos, vLocVel, vLocRot, vLocAngVel, fLocVec6D);

		Vector returnValue(dataCoordinates.NumberOfItems() + overheadData); //return = [eps, deltaT_recommended, data[0], data[1], ...]

		Vector6D uLoc6D;
		Vector6D vLoc6D;
		for (Index i = 0; i < 3; i++)
		{
			uLoc6D[i] = vLocPos[i];
			uLoc6D[i + 3] = vLocRot[i];
			vLoc6D[i] = vLocVel[i];
			vLoc6D[i + 3] = vLocAngVel[i];
		}

		EvaluateUserFunctionPostNewtonStep(returnValue, cSystemData->GetMainSystemBacklink(), markerDataCurrent.GetTime(), itemIndex,
			dataCoordinates, uLoc6D, vLoc6D);

		discontinuousError = returnValue[0];
		recommendedStepSize = returnValue[1];

		for (Index i = 0; i < dataCoordinates.NumberOfItems(); i++)
		{
			dataCoordinates[i] = returnValue[i + overheadData];
		}
	}

	//if (parameters.activeConnector)
	//{
	//	LinkedDataVector currentState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current);	//copy, but might change values ...

	//	ConstSizeVector<maxNumberOfSegments> currentGapPerSegment;
	//	ConstSizeVector<maxNumberOfSegments> referenceCoordinatePerSegment;
	//	ConstSizeVector<maxNumberOfSegments> xDirectionGap;
	//	ConstSizeVector<maxNumberOfSegments> yDirectionGap;
	//	ComputeGap(markerDataCurrent, currentGapPerSegment, referenceCoordinatePerSegment, xDirectionGap, yDirectionGap);

	//	for (Index i = 0; i < parameters.numberOfContactSegments; i++)
	//	{
	//		//if (currentGapPerSegment[i] > 0 && currentState[i] <= 0 || currentGapPerSegment[i] <= 0 && currentState[i] > 0) //OLD: brackets missing!
	//		if ((currentGapPerSegment[i] > 0 && currentState[i] <= 0) || (currentGapPerSegment[i] <= 0 && currentState[i] > 0))
	//		{//action: state1=currentGapState, error = |currentGap*k|
	//			discontinuousError += fabs((currentGapPerSegment[i] - currentState[i])* parameters.contactStiffness);
	//			currentState[i] = currentGapPerSegment[i];
	//		}
	//	}
	//}
	return discontinuousError;
}

