/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorLinearSpringDamper
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
#include "Autogenerated/CObjectConnectorLinearSpringDamper.h"
#include "Linalg/RigidBodyMath.h"

void CObjectConnectorLinearSpringDamper::ComputeSpringForce(const MarkerDataStructure& markerData, Index itemIndex,
	Matrix3D& A0, Real& displacement, Real& velocity, Real& force) const
{
	//spring is measured relative to marker0:

	A0 = markerData.GetMarkerData(0).orientation;

	//relative displacement
	displacement = (A0 * parameters.axisMarker0) * (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position);
	velocity = (A0 * parameters.axisMarker0) * (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compute resulting force:
	if (!parameters.springForceUserFunction)
	{
		force = parameters.stiffness * (displacement- parameters.offset) + 
			parameters.damping * (velocity - parameters.velocityOffset) + parameters.force;
	}
	else
	{
		EvaluateUserFunctionForce(force, cSystemData->GetMainSystemBacklink(), markerData.GetTime(), itemIndex, displacement, velocity);
	}
}


//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectConnectorLinearSpringDamper::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorLinearSpringDamper::ComputeODE2LHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Lhs) vector
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).rotationJacobian.NumberOfColumns() + markerData.GetMarkerData(1).rotationJacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.);

	if (parameters.activeConnector)
	{
		Real displacement, velocity, force;
		Matrix3D A0;

		//compute local torque
		ComputeSpringForce(markerData, objectNumber, A0, displacement, velocity, force);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//transform to scalar force to global coordinates (application of forces/torques always global):
		Vector3D fTrans = A0 * (parameters.axisMarker0 * force);

		//now link ode2Lhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Lhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), 
				markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
			EXUmath::MultMatrixTransposedVectorAdd(markerData.GetMarkerData(1).positionJacobian, fTrans, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			fTrans *= -1.;
			LinkedDataVector ldv0(ode2Lhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());
			EXUmath::MultMatrixTransposedVectorAdd(markerData.GetMarkerData(0).positionJacobian, fTrans, ldv0);
		}
	}

}


//! provide according output variable in "value"
void CObjectConnectorLinearSpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	Real displacement, velocity, force;
	Matrix3D A0;

	//compute local torque
	ComputeSpringForce(markerData, itemIndex, A0, displacement, velocity, force);
	//Vector3D fTrans = A0 * (parameters.axisMarker0 * force);

	switch (variableType)
	{
	case OutputVariableType::DisplacementLocal: value.CopyFrom(Vector1D(displacement)); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(Vector1D(velocity)); break;
	case OutputVariableType::ForceLocal: value.CopyFrom(Vector1D(force)); break;
	default:
		SysError("CObjectConnectorLinearSpringDamper::GetOutputVariableConnector failed"); //error should not occur, because types are checked!
	}
}
