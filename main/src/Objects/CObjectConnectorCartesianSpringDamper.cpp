/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorCartesianSpringDamper
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorCartesianSpringDamper.h"

#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h> //accept numpy arrays: numpy array automatically converted to std::vector<Real,...> ==> accepts np.array([1,0,0]) and [1,0,0] as return value!


//! function provides the force, relative displacement and relative velocity of the connector
inline void CObjectConnectorCartesianSpringDamper::ComputeSpringForce(const MarkerDataStructure& markerData, const CObjectConnectorCartesianSpringDamperParameters& parameters,
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
			//user function args:(deltaL, deltaL_t, Real stiffness, Real damping, Real offset, Real dryFriction, Real dryFrictionProportionalZone)
			//fVec = parameters.springForceUserFunction(markerData.GetTime(), vPos, vVel, parameters.stiffness, parameters.damping, parameters.offset);
			UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
			{
				fVec = parameters.springForceUserFunction(markerData.GetTime(), vPos, vVel, parameters.stiffness, parameters.damping, parameters.offset);
			}, "ObjectConnectorCartesianSpringDamper::springForceUserFunction");
		}
	}

}

//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
//  MODEL: f
void CObjectConnectorCartesianSpringDamper::ComputeODE2RHS(Vector& ode2Rhs, const MarkerDataStructure& markerData) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorCartesianSpringDamper::ComputeODE2RHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Rhs) vector
	ode2Rhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	ode2Rhs.SetAll(0.);

	if (parameters.activeConnector)
	{
		Vector3D vPos, vVel, fVec;
		ComputeSpringForce(markerData, parameters, vPos, vVel, fVec);

		//now link ode2Rhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Rhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());

			//ldv1 = (1.)*(markerData.GetMarkerData(1).positionJacobian.GetTransposed()*f); //slow version		
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).positionJacobian, fVec, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv0(ode2Rhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());

			//ldv0 = (-1.)*(jacobian0.GetTransposed()*f); //SLOW version
			fVec *= -1.;
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, fVec, ldv0);
			//ldv0 *= -1.; done in fVec ==> should be faster
		}
	}

}

void CObjectConnectorCartesianSpringDamper::ComputeJacobianODE2_ODE2(ResizableMatrix& jacobian, ResizableMatrix& jacobian_ODE2_t, const MarkerDataStructure& markerData) const
{
	CHECKandTHROWstring("ERROR: illegal call to CObjectConnectorCartesianSpringDamper::ComputeODE2RHSJacobian");
}

//! provide according output variable in "value"
void CObjectConnectorCartesianSpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	Vector3D vPos, vVel, fVec;
	ComputeSpringForce(markerData, parameters, vPos, vVel, fVec);

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


