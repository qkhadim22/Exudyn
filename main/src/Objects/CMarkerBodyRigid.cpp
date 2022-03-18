/** ***********************************************************************************************
* @brief        implementation for MarkerBodyRigid
*
* @author       Gerstmayr Johannes
* @date         2019-05-02 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CMarkerBodyRigid.h"


void CMarkerBodyRigid::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	position = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetPosition(parameters.localPosition, configuration);
}

void CMarkerBodyRigid::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
{
	velocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetVelocity(parameters.localPosition, configuration);
}

void CMarkerBodyRigid::GetRotationMatrix(const CSystemData& cSystemData, Matrix3D& rotationMatrix, ConfigurationType configuration) const
{

	rotationMatrix = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetRotationMatrix(parameters.localPosition, configuration);
}

void CMarkerBodyRigid::GetAngularVelocity(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	angularVelocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetAngularVelocity(parameters.localPosition, configuration);
}

void CMarkerBodyRigid::GetAngularVelocityLocal(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	angularVelocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetAngularVelocityLocal(parameters.localPosition, configuration);
}

void CMarkerBodyRigid::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
//void CMarkerBodyRigid::GetPositionJacobian(const CSystemData& cSystemData, Matrix& jacobian) const
{
	((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->ComputeRigidBodyMarkerData(parameters.localPosition, computeJacobian, markerData);
	//markerData.position = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetPosition(parameters.localPosition, ConfigurationType::Current);
	//markerData.velocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetVelocity(parameters.localPosition, ConfigurationType::Current);

	//markerData.orientation = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetRotationMatrix(parameters.localPosition, ConfigurationType::Current);
	//markerData.angularVelocityLocal = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetAngularVelocityLocal(parameters.localPosition, ConfigurationType::Current);
	//markerData.velocityAvailable = true;

	//if (computeJacobian)
	//{
	//	((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->
	//		GetAccessFunctionBody(AccessFunctionType::TranslationalVelocity_qt, parameters.localPosition, markerData.positionJacobian);
	//	((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->
	//		GetAccessFunctionBody(AccessFunctionType::AngularVelocity_qt, parameters.localPosition, markerData.rotationJacobian);
	//}
}

//! compute markerdata: fill in according data for derivative of jacobian times vector v, e.g.: d(J.T @ v)/dq
void CMarkerBodyRigid::ComputeMarkerDataJacobianDerivative(const CSystemData& cSystemData, const Vector6D& v6D, MarkerData& markerData) const
{
	if (!EXUstd::IsOfType(cSystemData.GetCObjects()[parameters.bodyNumber]->GetAccessFunctionTypes(), AccessFunctionType::JacobianTtimesVector_q))
	{
		CHECKandTHROWstring("CMarkerBodyRigid::ComputeMarkerDataJacobianDerivative: body " + EXUstd::ToString(parameters.bodyNumber) + " does not provide a jacobian derivative; use different markers or set newton.numericalDifferentiation.forODE2connectors = True or use explicit integrator for contact");
	}

	//v has always size 6, must be copied to markerData.jacobianDerivative
	markerData.jacobianDerivative.SetNumberOfRowsAndColumns(1, 6);
	for (Index i = 0; i < v6D.NumberOfItems(); i++)
	{
		markerData.jacobianDerivative(0, i) = v6D[i];
	}
	//==>markerData.jacobianDerivative is input to GetAccessFunctionBody(...)
	((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->
		GetAccessFunctionBody(AccessFunctionType::JacobianTtimesVector_q, parameters.localPosition, markerData.jacobianDerivative);
}


