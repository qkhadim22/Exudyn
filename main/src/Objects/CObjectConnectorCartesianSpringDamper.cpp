/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorCartesianSpringDamper
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
#include "Autogenerated/CObjectConnectorCartesianSpringDamper.h"

//! function provides the force, relative displacement and relative velocity of the connector
inline void CObjectConnectorCartesianSpringDamper::ComputeSpringForce(const MarkerDataStructure& markerData, Index itemIndex,
	//const CObjectConnectorCartesianSpringDamperParameters& parameters,
	Vector3D& vPos, Vector3D& vVel, Vector3D& fVec) const
{
	//relative position, spring length and inverse spring length
	vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position - parameters.offset);

	//unit direction and relative velocity of spring-damper
	vVel = (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	fVec.SetAll(0.);
	if (parameters.activeConnector)
	{
		//compute resulting force vector:
		if (!parameters.springForceUserFunction)
		{
			for (Index i = 0; i < 3; i++)
			{
				fVec[i] = parameters.stiffness[i] * vPos[i] + parameters.damping[i] * vVel[i];
			}
		}
		else
		{
			EvaluateUserFunctionForce(fVec, cSystemData->GetMainSystemBacklink(), markerData.GetTime(), itemIndex, vPos, vVel);
		}
	}

}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
//  MODEL: f
void CObjectConnectorCartesianSpringDamper::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorCartesianSpringDamper::ComputeODE2LHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Lhs) vector
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.);

	if (parameters.activeConnector)
	{
		Vector3D vPos, vVel, fVec;
		ComputeSpringForce(markerData, objectNumber, vPos, vVel, fVec);
		//markerData.GetConnectorForceJac() = fVec;

		//now link ode2Lhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Lhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());

			//ldv1 = (1.)*(markerData.GetMarkerData(1).positionJacobian.GetTransposed()*f); //slow version		
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).positionJacobian, fVec, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv0(ode2Lhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());

			//ldv0 = (-1.)*(jacobian0.GetTransposed()*f); //SLOW version
			fVec *= -1.;
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, fVec, ldv0);
			//ldv0 *= -1.; done in fVec ==> should be faster
		}
	}

}

//! compute global 6D force and torque which is used for computation of derivative of jacobian; used only in combination with ComputeJacobianODE2_ODE2
void CObjectConnectorCartesianSpringDamper::ComputeJacobianForce6D(const MarkerDataStructure& markerData, Index objectNumber, Vector6D& force6D) const
{
	if (parameters.activeConnector) 
	{
		Vector3D vPos, vVel, fVec;
		ComputeSpringForce(markerData, objectNumber, vPos, vVel, fVec);
		force6D.SetVector({ fVec[0], fVec[1], fVec[2], 0., 0., 0.});
	}
	else { force6D.SetAll(0.); }
}

//! Computational function: compute Jacobian of \hac{ODE2} \ac{LHS} equations w.r.t. ODE2 coordinates and ODE2 velocities; write either dense local jacobian into dense matrix of MatrixContainer or ADD sparse triplets INCLUDING ltg mapping to sparse matrix of MatrixContainer
void CObjectConnectorCartesianSpringDamper::ComputeJacobianODE2_ODE2(EXUmath::MatrixContainer& jacobianODE2, JacobianTemp& temp,
	Real factorODE2, Real factorODE2_t,
	Index objectNumber, const ArrayIndex& ltg, const MarkerDataStructure& markerData) const
{
	if (parameters.activeConnector)
	{
		temp.localJacobian.SetNumberOfRowsAndColumns(3, 3);
		temp.localJacobian.SetAll(0.);
		//compute inner jacobian: factorODE2 * d(F)/(dq) + factorODE2_t * d(F)/(dq_t)
		//Matrix3D K(3, 3, 0.);
		for (Index i = 0; i < 3; i++)
		{
			temp.localJacobian(i, i) = parameters.stiffness[i] * factorODE2 + parameters.damping[i] * factorODE2_t;
		}
	}
	//compute jacobianODE2 in dense mode; temp.localJacobian is modified!
	ComputeJacobianODE2_ODE2generic(temp.localJacobian, jacobianODE2, temp, factorODE2, factorODE2_t, objectNumber, markerData, 
		parameters.activeConnector, false, false);
}


//! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags
JacobianType::Type CObjectConnectorCartesianSpringDamper::GetAvailableJacobians() const
{
	if (!parameters.springForceUserFunction) {
		return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t + JacobianType::ODE2_ODE2_function + JacobianType::ODE2_ODE2_t_function);
	}
	else {
		return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t);
	}
}


//! provide according output variable in "value"
void CObjectConnectorCartesianSpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	Vector3D vPos, vVel, fVec;
	ComputeSpringForce(markerData, itemIndex, vPos, vVel, fVec);

	switch (variableType)
	{
	case OutputVariableType::Displacement: value.CopyFrom(vPos); break;
	case OutputVariableType::Distance: value.SetVector({vPos.GetL2Norm()}); break;
	case OutputVariableType::Velocity: value.CopyFrom(vVel); break;
	case OutputVariableType::Force: value.CopyFrom(fVec); break;
	default:
		SysError("CObjectConnectorCartesianSpringDamper::GetOutputVariableConnector failed"); //error should not occur, because types are checked!
	}
}


