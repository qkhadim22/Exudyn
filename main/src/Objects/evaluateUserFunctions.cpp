/** ***********************************************************************************************
* @brief        implementation of all CObject EvaluateUserFunction... functions, 
*				to reduce dependencies on pybind and MainSystem.h
*
* @author       Gerstmayr Johannes
* @date         2021-01-04 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h> //accept numpy arrays: numpy array automatically converted to std::vector<Real,...> ==> accepts np.array([1,0,0]) and [1,0,0] as return value!
#include "Pymodules/PybindUtilities.h" //for CObjectGenericODE2

#include "Main/MainSystem.h"

#include "Autogenerated/CObjectConnectorCartesianSpringDamper.h"
#include "Autogenerated/CObjectConnectorCoordinate.h"
#include "Autogenerated/CObjectConnectorCoordinateSpringDamper.h"
#include "Autogenerated/CObjectConnectorSpringDamper.h"
#include "Autogenerated/CObjectConnectorRigidBodySpringDamper.h"

#include "Autogenerated/CObjectFFRF.h"
#include "Autogenerated/CObjectFFRFreducedOrder.h"
#include "Autogenerated/CObjectGenericODE2.h"
#include "Autogenerated/CObjectJointGeneric.h"


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectConnectorCartesianSpringDamper::EvaluateUserFunctionForce(Vector3D& force, const MainSystemBase& mainSystem, Real t, Vector3D& vPos, Vector3D& vVel) const
{
	//user function args:(deltaL, deltaL_t, Real stiffness, Real damping, Real offset, Real dryFriction, Real dryFrictionProportionalZone)
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		force = parameters.springForceUserFunction((const MainSystem&)mainSystem, t, vPos, vVel,
			parameters.stiffness, parameters.damping, parameters.offset);
	}, "ObjectConnectorCartesianSpringDamper::springForceUserFunction");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectConnectorCoordinate::EvaluateUserFunctionOffset(Real& offset, const MainSystemBase& mainSystem, Real t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		offset = parameters.offsetUserFunction((const MainSystem&)mainSystem, t, parameters.offset);
	}, "ObjectConnectorCoordinate::offsetUserFunction");
}

//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectConnectorCoordinate::EvaluateUserFunctionOffset_t(Real& offset, const MainSystemBase& mainSystem, Real t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		offset = parameters.offsetUserFunction_t((const MainSystem&)mainSystem,  t, parameters.offset);
	}, "ObjectConnectorCoordinate::offsetUserFunction_t");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectConnectorRigidBodySpringDamper::EvaluateUserFunctionForce(Vector6D& fLocVec6D, const MainSystemBase& mainSystem, Real t, 
	Vector6D& uLoc6D, Vector6D& vLoc6D) const
{
	Vector3D u, rot, v, omega;
	for (Index i = 0; i < 3; i++)
	{
		u[i] = uLoc6D[i];
		rot[i] = uLoc6D[i + 3];
		v[i] = vLoc6D[i];
		omega[i] = vLoc6D[i + 3];
	}

	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		fLocVec6D = parameters.springForceTorqueUserFunction((const MainSystem&)mainSystem, t,
			u, rot, v, omega, EXUmath::Matrix6DToStdArray66(parameters.stiffness), EXUmath::Matrix6DToStdArray66(parameters.damping),
			EXUmath::Matrix3DToStdArray33(parameters.rotationMarker0), EXUmath::Matrix3DToStdArray33(parameters.rotationMarker1),
			parameters.offset);

	}, "ObjectConnectorRigidBodySpringDamper::springForceTorqueUserFunction");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectJointGeneric::EvaluateUserFunctionOffset(Vector6D& offset, const MainSystemBase& mainSystem, Real t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		//user function args:(Real t, Real load)
		offset = (Vector6D)(parameters.offsetUserFunction((const MainSystem&)mainSystem, t, parameters.offsetUserFunctionParameters));
	}, "ObjectJointGeneric::offsetUserFunction");

}

//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectJointGeneric::EvaluateUserFunctionOffset_t(Vector6D& offset, const MainSystemBase& mainSystem, Real t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		//user function args:(Real t, Real load)
		offset = (Vector6D)(parameters.offsetUserFunction((const MainSystem&)mainSystem, t, parameters.offsetUserFunctionParameters));
	}, "ObjectJointGeneric::offsetUserFunction  (called from ComputeJacobianAE)");

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectConnectorSpringDamper::EvaluateUserFunctionForce(Real& force, const MainSystemBase& mainSystem, Real t, Real deltaL, Real deltaL_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		//user function args:(deltaL, deltaL_t, Real stiffness, Real damping, Real springForce)
		force = parameters.springForceUserFunction((const MainSystem&)mainSystem, t, 
			deltaL, deltaL_t, parameters.stiffness, parameters.damping, parameters.force);
	}, "ObjectConnectorSpringDamper::springForceUserFunction");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectGenericODE2::EvaluateUserFunctionForce(Vector& force, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		force = (Vector)(parameters.forceUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t));
	}, "ObjectGenericODE2::forceUserFunction");
}

//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectGenericODE2::EvaluateUserFunctionMassMatrix(Matrix& massMatrix, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		EPyUtils::NumPy2Matrix(parameters.massMatrixUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t), massMatrix);
	}, "ObjectGenericODE2::massMatrixUserFunction");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectFFRF::EvaluateUserFunctionForce(Vector& force, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		force = (Vector)(parameters.forceUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t));
	}, "ObjectFFRF::forceUserFunction");
}

//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectFFRF::EvaluateUserFunctionMassMatrix(Matrix& massMatrix, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		EPyUtils::NumPy2Matrix(parameters.massMatrixUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t), massMatrix);
	}, "ObjectFFRF::massMatrixUserFunction");
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectFFRFreducedOrder::EvaluateUserFunctionForce(Vector& force, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		force = (Vector)(parameters.forceUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t));
	}, "ObjectFFRFreducedOrder::forceUserFunction");
}

//! call to user function implemented in separate file to avoid including pybind and MainSystem.h at too many places
void CObjectFFRFreducedOrder::EvaluateUserFunctionMassMatrix(Matrix& massMatrix, const MainSystemBase& mainSystem, Real t, const StdVector& coordinates, const StdVector& coordinates_t) const
{
	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
	{
		EPyUtils::NumPy2Matrix(parameters.massMatrixUserFunction((const MainSystem&)mainSystem, t, coordinates, coordinates_t), massMatrix);
	}, "ObjectFFRFreducedOrder::massMatrixUserFunction");
}



