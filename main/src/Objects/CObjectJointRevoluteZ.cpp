/** ***********************************************************************************************
* @brief        CObjectJointRevoluteZ implementation; follows mostly the CObjectJointGeneric.cpp implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectJointRevoluteZ.h"
#include "Linalg/RigidBodyMath.h"

#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h> //accept numpy arrays: numpy array automatically converted to std::vector<Real,...> ==> accepts np.array([1,0,0]) and [1,0,0] as return value!

//! Computational function: compute algebraic equations and write residual into "algebraicEquations" 
void CObjectJointRevoluteZ::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, Index itemIndex, bool velocityLevel) const
{
	
	if (parameters.activeConnector)
	{
		//CPU time for total algebraic equations (including call from CSystem.cpp):
		//  index3 takes roughly 175ns (i7)
		//  index2 takes roughly 300ns (i7)
		constexpr Index lockedAxis1 = 0; //x
		constexpr Index lockedAxis2 = 1; //y
		constexpr Index freeAxis = 2;	 //z

		algebraicEquations.SetNumberOfItems(nConstraints);
		//const LinkedDataVector& lambda = markerData.GetLagrangeMultipliers();

		const MarkerData& markerData0 = markerData.GetMarkerData(0);
		const MarkerData& markerData1 = markerData.GetMarkerData(1);

		const Matrix3D& A0 = markerData0.orientation;
		//const Matrix3D& A0off = parameters.rotationMarker0;
		//Matrix3D A0all = (A0*A0off);


		const Matrix3D& A1 = markerData1.orientation;
		//const Matrix3D& A1off = parameters.rotationMarker1;
		//Matrix3D A1all = (A1*A1off);


		Vector3D vRot0 = A0*parameters.rotationMarker0.GetColumnVector<3>(freeAxis);
		Vector3D vLocked1 = A1 * parameters.rotationMarker1.GetColumnVector<3>(lockedAxis1);
		Vector3D vLocked2 = A1 * parameters.rotationMarker1.GetColumnVector<3>(lockedAxis2);
		//Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
		//Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
		//Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

		if (!velocityLevel)
		{
			//++++++++++++++++++++++++++++++++
			//translation constraints:
			//global equations in case that all position coordinates are locked!
			Vector3D vPos = (markerData1.position - markerData0.position);

			algebraicEquations[0] = vPos[0];
			algebraicEquations[1] = vPos[1];
			algebraicEquations[2] = vPos[2];

			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			algebraicEquations[lockedAxis1 + 3] = vRot0 * vLocked1;
			algebraicEquations[lockedAxis2 + 3] = vRot0 * vLocked2;
			//algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3]; //only in generic joint
		}
		else //index2
		{
			//CHECKandTHROWstring("CObjectJointRevoluteZ: velocity level not implemented");
			CHECKandTHROW(markerData1.velocityAvailable && markerData0.velocityAvailable, "CObjectJointRevoluteZ::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

			//++++++++++++++++++++++++++++++++
			//translation constraints:

			//use difference of velocities in global coordinates
			Vector3D vVel = (markerData1.velocity - markerData0.velocity);

			algebraicEquations[0] = vVel[0];
			algebraicEquations[1] = vVel[1];
			algebraicEquations[2] = vVel[2];

			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			//Matrix3D A0_t = markerData0.orientation * RigidBodyMath::Vector2SkewMatrix(markerData0.angularVelocityLocal);
			//Matrix3D A1_t = markerData1.orientation * RigidBodyMath::Vector2SkewMatrix(markerData1.angularVelocityLocal);
			//Matrix3D A0all_t = (A0_t*A0off);
			//Matrix3D A1all_t = (A1_t*A1off);

			//Vector3D vRot0_t = A0all_t.GetColumnVector<3>(freeAxis);
			//Vector3D vLocked1_t = A1all_t.GetColumnVector<3>(lockedAxis1);
			//Vector3D vLocked2_t = A1all_t.GetColumnVector<3>(lockedAxis2);

			Vector3D vRot0_t = A0 * markerData0.angularVelocityLocal.CrossProduct(parameters.rotationMarker0.GetColumnVector<3>(freeAxis));
			Vector3D vLocked1_t = A1 * markerData1.angularVelocityLocal.CrossProduct(parameters.rotationMarker1.GetColumnVector<3>(lockedAxis1));
			Vector3D vLocked2_t = A1 * markerData1.angularVelocityLocal.CrossProduct(parameters.rotationMarker1.GetColumnVector<3>(lockedAxis2));


			//free axis leads to Lagrange multiplier; locked axes must be perpendicular to rotation axis of marker0 (vRot0)
			algebraicEquations[lockedAxis1 + 3] = vRot0_t * vLocked1 + vRot0 * vLocked1_t;
			algebraicEquations[lockedAxis2 + 3] = vRot0_t * vLocked2 + vRot0 * vLocked2_t;
			//algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3]; //only in generic joint
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [lambda0,lambda1]^T = [0,0]^T, means that the current values need to be copied
	}
	
/*
	if (parameters.activeConnector)
	{

		algebraicEquations.SetNumberOfItems(nConstraints);
		//const LinkedDataVector& lambda = markerData.GetLagrangeMultipliers();

		const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
		const Matrix3D& A0off = parameters.rotationMarker0;
		Matrix3D A0all = (A0*A0off);

		const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
		const Matrix3D& A1off = parameters.rotationMarker1;
		Matrix3D A1all = (A1*A1off);

		const Index lockedAxis1 = 0; //x
		const Index lockedAxis2 = 1; //y
		const Index freeAxis = 2;	 //z

		Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
		Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
		Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

		if (!velocityLevel)
		{
			//++++++++++++++++++++++++++++++++
			//translation constraints:
			//global equations in case that all position coordinates are locked!
			Vector3D vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position);

			algebraicEquations[0] = vPos[0];
			algebraicEquations[1] = vPos[1];
			algebraicEquations[2] = vPos[2];

			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			algebraicEquations[lockedAxis1 + 3] = vRot0 * vLocked1;
			algebraicEquations[lockedAxis2 + 3] = vRot0 * vLocked2;
			//algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3]; //only in generic joint
		}
		else //index2
		{
			//CHECKandTHROWstring("CObjectJointRevoluteZ: velocity level not implemented");
			CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable, "CObjectJointRevoluteZ::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

			//++++++++++++++++++++++++++++++++
			//translation constraints:

			//use difference of velocities in global coordinates
			Vector3D vVel = (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

			algebraicEquations[0] = vVel[0];
			algebraicEquations[1] = vVel[1];
			algebraicEquations[2] = vVel[2];

			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			Matrix3D A0_t = markerData.GetMarkerData(0).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(0).angularVelocityLocal);
			Matrix3D A1_t = markerData.GetMarkerData(1).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(1).angularVelocityLocal);
			Matrix3D A0all_t = (A0_t*A0off);
			Matrix3D A1all_t = (A1_t*A1off);

			Vector3D vRot0_t = A0all_t.GetColumnVector<3>(freeAxis);
			Vector3D vLocked1_t = A1all_t.GetColumnVector<3>(lockedAxis1);
			Vector3D vLocked2_t = A1all_t.GetColumnVector<3>(lockedAxis2);

			//free axis leads to Lagrange multiplier; locked axes must be perpendicular to rotation axis of marker0 (vRot0)
			algebraicEquations[lockedAxis1 + 3] = vRot0_t * vLocked1 + vRot0 * vLocked1_t;
			algebraicEquations[lockedAxis2 + 3] = vRot0_t * vLocked2 + vRot0 * vLocked2_t;
			//algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3]; //only in generic joint
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [lambda0,lambda1]^T = [0,0]^T, means that the current values need to be copied
	}
*/

}


void CObjectJointRevoluteZ::ComputeJacobianAE(ResizableMatrix& jacobian_ODE2, ResizableMatrix& jacobian_ODE2_t, ResizableMatrix& jacobian_ODE1, 
	ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData, Real t, Index itemIndex) const
{
	if (parameters.activeConnector)
	{
		//markerData contains already the correct jacobians ==> transformed to constraint jacobian
		Index nColumnsJac0 = markerData.GetMarkerData(0).positionJacobian.NumberOfColumns();
		Index nColumnsJac1 = markerData.GetMarkerData(1).positionJacobian.NumberOfColumns();
		jacobian_ODE2.SetNumberOfRowsAndColumns(nConstraints, nColumnsJac0 + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());

		const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
		const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
		//const Matrix3D& A0off = EXUmath::unitMatrix3D; //parameters.rotationMarker0;
		//const Matrix3D& A1off = EXUmath::unitMatrix3D; //parameters.rotationMarker1;
		const Matrix3D& A0off = parameters.rotationMarker0;
		const Matrix3D& A1off = parameters.rotationMarker1;
		Matrix3D A0all = (A0*A0off);
		Matrix3D A1all = (A1*A1off);

		//global equations
		const ResizableMatrix& posJac0 = markerData.GetMarkerData(0).positionJacobian;
		//vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //local equations (marker0-fixed)
		for (Index i = 0; i < nColumnsJac0; i++)
		{
			jacobian_ODE2(0, i) = -posJac0(0, i); //negative sign in marker0 because of eq: (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position)
			jacobian_ODE2(1, i) = -posJac0(1, i);
			jacobian_ODE2(2, i) = -posJac0(2, i);
		}
		const ResizableMatrix& posJac1 = markerData.GetMarkerData(1).positionJacobian;
		for (Index i = 0; i < nColumnsJac1; i++)
		{
			jacobian_ODE2(0, i + nColumnsJac0) = posJac1(0, i);
			jacobian_ODE2(1, i + nColumnsJac0) = posJac1(1, i);
			jacobian_ODE2(2, i + nColumnsJac0) = posJac1(2, i);
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//CHECKandTHROWstring("CObjectJointRevoluteZ: revolute configuration (2 rotation locked) not implemented");
		const Index lockedAxis1 = 0; //x
		const Index lockedAxis2 = 1; //y
		const Index freeAxis = 2;	 //z

		Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
		Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
		Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

		Matrix3D vRot0Skew(RigidBodyMath::Vector2SkewMatrix(vRot0));
		Matrix3D vLocked1Skew(RigidBodyMath::Vector2SkewMatrix(vLocked1));
		Matrix3D vLocked2Skew(RigidBodyMath::Vector2SkewMatrix(vLocked2));

		//vRot0_t = omegaTilde * vRot0 = -vRot0tilde * omega ==> vRot0/dTheta = -vRot0tilde * G
		for (Index i = 0; i < nColumnsJac0; i++)
		{
			Vector3D Gvec0 = -1.*markerData.GetMarkerData(0).rotationJacobian.GetColumnVector<3>(i);
			jacobian_ODE2(3 + lockedAxis1, i) = vLocked1 * (vRot0Skew * Gvec0);
			jacobian_ODE2(3 + lockedAxis2, i) = vLocked2 * (vRot0Skew * Gvec0);
			//jacobian_ODE2(3 + freeAxis, i) = 0; //only in GenericJoint
		}
		for (Index i = 0; i < nColumnsJac1; i++)
		{
			Vector3D Gvec1 = -1.*markerData.GetMarkerData(1).rotationJacobian.GetColumnVector<3>(i);
			jacobian_ODE2(3 + lockedAxis1, i + nColumnsJac0) = vRot0 * (vLocked1Skew * Gvec1);
			jacobian_ODE2(3 + lockedAxis2, i + nColumnsJac0) = vRot0 * (vLocked2Skew * Gvec1);
			//jacobian_ODE2(3 + freeAxis, i + nColumnsJac0) = 0; //only in GenericJoint
		}

		jacobian_ODE2_t.SetNumberOfRowsAndColumns(0, 0); //for safety? check that this cannot happen ...
		//jacobian_AE.SetNumberOfRowsAndColumns(0, 0);//for safety!
	}
	else
	{
		jacobian_AE.SetScalarMatrix(nConstraints, 1.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
	}
}

JacobianType::Type CObjectJointRevoluteZ::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function); 
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}

//! provide according output variable in "value"
void CObjectJointRevoluteZ::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
	const Matrix3D& A0off = parameters.rotationMarker0;
	Matrix3D A0all = (A0*A0off);

	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Matrix3D& A1off = parameters.rotationMarker1;
	Matrix3D A1all = (A1*A1off);

	//use difference of positions in local joint J0 coordinates
	Vector3D vPosLocal = A0all.GetTransposed()*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); 
	Vector3D vVelLocal = A0all.GetTransposed()*(markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	Vector3D angVelLocal = A0all.GetTransposed()*(A1*markerData.GetMarkerData(1).angularVelocityLocal - A0 * markerData.GetMarkerData(0).angularVelocityLocal); //difference of global angular velocities, projected into J0

	//compute relative rotation, J0-fixed:
	Matrix3D relRot = A0all.GetTransposed() * A1all;
	Vector3D vLocRot = RigidBodyMath::RotationMatrix2RotXYZ(relRot);

	if (vLocRot[0] > EXUstd::pi) { vLocRot[0] -= 2.*EXUstd::pi; }
	if (vLocRot[1] > EXUstd::pi) { vLocRot[1] -= 2.*EXUstd::pi; }
	if (vLocRot[2] > EXUstd::pi) { vLocRot[2] -= 2.*EXUstd::pi; }

	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(markerData.GetMarkerData(0).position); break;
	case OutputVariableType::Velocity: value.CopyFrom(markerData.GetMarkerData(0).velocity); break;
	case OutputVariableType::DisplacementLocal: value.CopyFrom(vPosLocal); break;
	case OutputVariableType::Rotation: value.CopyFrom(vLocRot); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(vVelLocal); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(angVelLocal); break;
	case OutputVariableType::ForceLocal:
	{
		Vector3D force({ GetCurrentAEcoordinate(0), GetCurrentAEcoordinate(1), GetCurrentAEcoordinate(2) }); //x,y and z global coordinates
		force = A0all.GetTransposed()*force;
		value.CopyFrom(force);
		break;
	}
	case OutputVariableType::TorqueLocal:
	{
		Vector3D torque({ GetCurrentAEcoordinate(3), GetCurrentAEcoordinate(4), 0.}); //local x, y constrained rotations; z = free rotation
		value.CopyFrom(torque);
		break;
	}
	default:
		SysError("CObjectJointRevoluteZ::GetOutputVariable failed"); //error should not occur, because types are checked!
	}

}



