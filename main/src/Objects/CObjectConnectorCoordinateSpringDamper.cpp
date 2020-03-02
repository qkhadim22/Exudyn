/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorCoordinateSpringDamper
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorCoordinateSpringDamper.h"

//! function provides the force, relative displacement and relative velocity of the connector
void ComputeConnectorCoordinateSpringDamperProperties(const MarkerDataStructure& markerData, const CObjectConnectorCoordinateSpringDamperParameters& parameters,
	Real& relPos, Real& relVel, Real& force)
{
	//relative position, spring length and inverse spring length
	relPos = (markerData.GetMarkerData(1).value - markerData.GetMarkerData(0).value);

	//unit direction and relative velocity of spring-damper
	relVel = (markerData.GetMarkerData(1).value_t - markerData.GetMarkerData(0).value_t);

	force = 0; //default; necessary, e.g. if computed in GetOutputVariable...
	if (parameters.activeConnector)
	{
		if (!parameters.springForceUserFunction)
		{
			//compute resulting force vector:
			force = parameters.stiffness * (relPos - parameters.offset) + parameters.damping * relVel;
			if (parameters.dryFriction != 0.)
			{
				if (fabs(relVel) < parameters.dryFrictionProportionalZone)
				{
					//as long as vVel < dryFrictionProportionalZone, friction force shall linearly increase
					force += relVel / parameters.dryFrictionProportionalZone * parameters.dryFriction;
				}
				else
				{
					force += parameters.dryFriction*EXUstd::Sgn(relVel); //this should be put into the nonlinear iteration for better Newton convergence ...
				}
			}
		}
		else
		{
			//user function args:(deltaL, deltaL_t, Real stiffness, Real damping, Real offset, Real dryFriction, Real dryFrictionProportionalZone)
			force = parameters.springForceUserFunction(markerData.GetTime(), relPos, relVel, parameters.stiffness, parameters.damping, parameters.offset, parameters.dryFriction, parameters.dryFrictionProportionalZone);
		}
	}
}



//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
//  MODEL: f
void CObjectConnectorCoordinateSpringDamper::ComputeODE2RHS(Vector& ode2Rhs, const MarkerDataStructure& markerData) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorCoordinateSpringDamper::ComputeODE2RHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Rhs) vector
	ode2Rhs.SetNumberOfItems(markerData.GetMarkerData(0).jacobian.NumberOfColumns() + markerData.GetMarkerData(1).jacobian.NumberOfColumns());
	ode2Rhs.SetAll(0.);

	if (parameters.activeConnector)
	{
		Real relPos;
		Real relVel;
		Real force;
		ComputeConnectorCoordinateSpringDamperProperties(markerData, parameters, relPos, relVel, force);


		Vector1D fVec(force); //convert to vector to allow matrix-multiplication as usual ...

		//now link ode2Rhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Rhs, markerData.GetMarkerData(0).jacobian.NumberOfColumns(), markerData.GetMarkerData(1).jacobian.NumberOfColumns());

			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).jacobian, fVec, ldv1);
		}

		if (markerData.GetMarkerData(0).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv0(ode2Rhs, 0, markerData.GetMarkerData(0).jacobian.NumberOfColumns());

			fVec *= -1.;
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).jacobian, fVec, ldv0);

		}
	}

}

void CObjectConnectorCoordinateSpringDamper::ComputeJacobianODE2_ODE2(ResizableMatrix& jacobian, ResizableMatrix& jacobian_ODE2_t, const MarkerDataStructure& markerData) const
{
	CHECKandTHROWstring("ERROR: illegal call to CObjectConnectorCoordinateSpringDamper::ComputeODE2RHSJacobian");
}

////! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
//OutputVariableType CObjectConnectorCoordinateSpringDamper::GetOutputVariableTypes() const
//{
//	//return OutputVariableType::_None;
//	return (OutputVariableType)((Index)OutputVariableType::Distance + (Index)OutputVariableType::Displacement +
//		(Index)OutputVariableType::Velocity + (Index)OutputVariableType::Force);
//}

//! provide according output variable in "value"
void CObjectConnectorCoordinateSpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	Real relPos;
	Real relVel;
	Real force;
	ComputeConnectorCoordinateSpringDamperProperties(markerData, parameters, relPos, relVel, force);

	switch (variableType)
	{
	case OutputVariableType::Displacement: value = Vector({ relPos }); break;
	case OutputVariableType::Velocity: value = Vector({ relVel }); break;
	case OutputVariableType::Force: value = Vector({ force }); break;
	default:
		SysError("CObjectConnectorCoordinateSpringDamper::GetOutputVariable failed"); //error should not occur, because types are checked!
	}

}

