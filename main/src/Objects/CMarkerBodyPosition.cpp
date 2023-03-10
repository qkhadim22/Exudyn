/** ***********************************************************************************************
* @brief        implementation for MarkerBodyPosition
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
#include "Autogenerated/CMarkerBodyPosition.h"


void CMarkerBodyPosition::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	position = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetPosition(parameters.localPosition, configuration);
}

void CMarkerBodyPosition::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
{
	velocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetVelocity(parameters.localPosition, configuration);
}

void CMarkerBodyPosition::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
//void CMarkerBodyPosition::GetPositionJacobian(const CSystemData& cSystemData, Matrix& jacobian) const
{
	markerData.position = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetPosition(parameters.localPosition, ConfigurationType::Current);
	markerData.velocity = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetVelocity(parameters.localPosition, ConfigurationType::Current);
	markerData.velocityAvailable = true;

	if (computeJacobian)
	{
		((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->
			GetAccessFunctionBody(AccessFunctionType::TranslationalVelocity_qt, parameters.localPosition, markerData.positionJacobian);
	}
}

//! compute markerdata: fill in according data for derivative of jacobian times vector v, e.g.: d(J.T @ v)/dq
void CMarkerBodyPosition::ComputeMarkerDataJacobianDerivative(const CSystemData& cSystemData, const Vector6D& v6D, MarkerData& markerData) const
{
	if (!EXUstd::IsOfType(cSystemData.GetCObjects()[parameters.bodyNumber]->GetAccessFunctionTypes(), AccessFunctionType::JacobianTtimesVector_q))
	{
		CHECKandTHROWstring("MarkerBodyPosition::ComputeMarkerDataJacobianDerivative: body " + EXUstd::ToString(parameters.bodyNumber) + " does not provide a jacobian derivative; use different markers or set newton.numericalDifferentiation.forODE2connectors = True or use explicit integrator for contact");
	}

	//v has size 6, must be copied to markerData.jacobianDerivative
	markerData.jacobianDerivative.SetNumberOfRowsAndColumns(1, 6);
	markerData.jacobianDerivative.SetAll(0.);
	for (Index i = 0; i < v6D.NumberOfItems(); i++)
	{
		markerData.jacobianDerivative(0, i) = v6D[i]; //only first 3 (position) components!
	}
	//==>markerData.jacobianDerivative is input to GetAccessFunctionBody(...)
	((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->
		GetAccessFunctionBody(AccessFunctionType::JacobianTtimesVector_q, parameters.localPosition, markerData.jacobianDerivative);
}

