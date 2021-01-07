/** ***********************************************************************************************
* @brief        implementation for MarkerBodyMass
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
#include "Autogenerated/CMarkerBodyMass.h"


void CMarkerBodyMass::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	const CObjectBody* cBody = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]));
	position = cBody->GetPosition(cBody->GetLocalCenterOfMass(), configuration);
}

void CMarkerBodyMass::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
{

	if (computeJacobian)
	{
		const CObjectBody* cBody = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]));
		cBody->GetAccessFunctionBody(AccessFunctionType::DisplacementMassIntegral_q, 
			cBody->GetLocalCenterOfMass(), 
			markerData.positionJacobian); //use positionJacobian to keep compatibility with BodyPosition marker
	}
	markerData.velocityAvailable = false;

}

