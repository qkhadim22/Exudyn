/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorRollingDiscPenalty
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
#include "Autogenerated/CObjectConnectorRollingDiscPenalty.h"
#include "Utilities/RigidBodyMath.h"

Vector2D CObjectConnectorRollingDiscPenalty::ComputeSlipForce(const CObjectConnectorRollingDiscPenaltyParameters& parameters, 
	const Vector2D& localSlipVelocity, const Vector2D& dataLocalSlipVelocity, Real contactForce) const
{
	Vector2D slipForce({ 0,0 });
	Real slipVelocity = localSlipVelocity.GetL2Norm();
	Real dataSlipVelocity = dataLocalSlipVelocity.GetL2Norm(); //of last discontinuous iteration, will not be changed during Newton; use GenericDataVariable HERE!
	if (dataSlipVelocity != 0 && slipVelocity != 0)
	{
		Vector2D slipDirection = (1. / slipVelocity) * localSlipVelocity;
		Vector2D dataSlipDirection = (1. / dataSlipVelocity) * dataLocalSlipVelocity;
		Real phi = 1.; //regularization, see docu; Geradin and Cardona, Flex. Mult. Systems, page 174
		if (dataSlipVelocity <= parameters.dryFrictionProportionalZone && parameters.dryFrictionProportionalZone != 0.)
		{
			Real fact = slipVelocity / parameters.dryFrictionProportionalZone; //in proportional zone, we can use the current slip velocity
			phi = (2. - fact)*fact;

			dataSlipDirection = slipDirection; //use current slip direction in this case, gives much better convergence in "sticking" case
		}

		//tangential contact force vector:
		Matrix2D frictionMatrix(2, 2, { parameters.dryFriction[0],0,0,parameters.dryFriction[1] });
		slipForce = -(phi * fabs(contactForce) * dataSlipDirection); //negative sign, as slip force acts against slip velocity
		if (parameters.dryFrictionAngle == 0)
		{
			//for (Index j = 0; j < 2; j++)
			//{
			//	slipForce[j] = -(parameters.dryFriction[j] * phi * fabs(contactForce) * dataSlipDirection[j]);
			//}
			slipForce = frictionMatrix * slipForce;
		}
		else
		{
			Real phi = parameters.dryFrictionAngle;
			Matrix2D rotFriction(2, 2, { cos(phi),-sin(phi),sin(phi),cos(phi) });
			slipForce = rotFriction.GetTransposed() * (frictionMatrix * (rotFriction * slipForce));
			//pout << "angle=" << phi << "\n";
			//pout << "  friction matrix=" << rotFriction.GetTransposed() * frictionMatrix * rotFriction << "\n";
		}
	}
	return slipForce;
}

//! compute contact force helper function; vectors pC, vC provide position and velocity of contact point in global coordinate system
//  vectors wLateral, w2 and w3 provide a joint J1 coordinate system, vectors also given in global coordinate system
//  contact force fContact is given in local joint J1 coordinates
//  computeCurrent: if true, the contact forces are computed for current values, but for the data/history variables
void CObjectConnectorRollingDiscPenalty::ComputeContactForces(const MarkerDataStructure& markerData, 
	const CObjectConnectorRollingDiscPenaltyParameters& parameters, bool computeCurrent, Vector3D& pC, Vector3D& vC, 
	Vector3D& wLateral, Vector3D& w2, Vector3D& w3, Vector3D& fContact, Vector2D& localSlipVelocity) const
{
	const Vector3D& p1 = markerData.GetMarkerData(1).position;
	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Vector3D& v1 = markerData.GetMarkerData(1).velocity;
	Vector3D omega1 = A1 * markerData.GetMarkerData(1).angularVelocityLocal;
	//const Vector3D& p0 = markerData.GetMarkerData(0).position; //use already z-position ..., but no transformation

	Vector3D vAxisLocal({ 1,0,0 });
	Vector3D w1 = A1 * vAxisLocal;

	w2 = w1.CrossProduct(parameters.planeNormal); //longitudinal direction; cross product has length != 1
	w2.Normalize();
	w3 = w1.CrossProduct(w2); //w1 and w2 are perpendicular ==> w3 already normalized
	wLateral = parameters.planeNormal.CrossProduct(w2);

	//compute contact point position and velocity:
	pC = p1 + parameters.discRadius * w3;
	vC = v1 + omega1.CrossProduct(parameters.discRadius * w3);

	fContact.SetAll(0); //if no contact or no friction
	Real contactForce = 0;

	Real currentGapState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[2];
	if (computeCurrent) { currentGapState = pC * parameters.planeNormal; }

	Vector2D dataLocalSlipVelocity({ ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[0],
		((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[1] });
	localSlipVelocity = Vector2D({ vC*wLateral, vC*w2 });

	if (currentGapState < 0) //only if gap smaller zero ==> use GenericDataVariable HERE!
	{
		contactForce = -(parameters.contactStiffness * pC[2] + parameters.contactDamping * vC[2]);

		Vector2D slipForce = ComputeSlipForce(parameters, localSlipVelocity, dataLocalSlipVelocity, contactForce);

		fContact[0] = slipForce[0];
		fContact[1] = slipForce[1];
	}
	fContact[2] = contactForce;
}


//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectConnectorRollingDiscPenalty::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorRollingDiscPenalty::ComputeODE2LHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Lhs) vector
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.);

	//pout << "test\n";
	if (parameters.activeConnector)
	{
		Vector3D pC; //deviation of contact conditions
		Vector3D vC; //deviation of velocity at contact point
		Vector3D w2; //normalized vector in longitudinal direction
		Vector3D w3; //normalized vector from wheel center to contact point
		Vector3D wLateral; //normalized vector from wheel center to contact point
		Vector3D fContact; //contact force (0=lateral, 1=longitudinal, 2=normal direction), local coordinates
		Vector2D localSlipVelocity;
		ComputeContactForces(markerData, parameters, false, pC, vC, wLateral, w2, w3, fContact, localSlipVelocity);

		Vector3D fPos = -(fContact[0]* wLateral + fContact[1] * w2 + fContact[2] * parameters.planeNormal); //in global coordinates now

		Vector3D fRot = (parameters.discRadius*w3).CrossProduct(fPos);

		if (parameters.rollingFrictionViscous) //not acting on rotation!
		{
			//this is the rolling friction approximated for disc axis parallel to ground:
			Vector3D velGround = markerData.GetMarkerData(1).velocity;
			velGround[2] = 0;
			fPos += parameters.rollingFrictionViscous*fabs(fContact[2])*velGround;
		}



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
void CObjectConnectorRollingDiscPenalty::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Vector3D& v1 = markerData.GetMarkerData(1).velocity;
	Vector3D omega1 = A1 * markerData.GetMarkerData(1).angularVelocityLocal;

	Vector3D pC; //deviation of contact conditions
	Vector3D vC; //deviation of velocity at contact point
	Vector3D w2; //normalized vector in longitudinal direction
	Vector3D w3; //normalized vector from wheel center to contact point
	Vector3D wLateral; //normalized vector from wheel center to contact point
	Vector3D fContact; //contact force (0=lateral, 1=longitudinal, 2=normal direction)
	Vector2D localSlipVelocity;
	ComputeContactForces(markerData, parameters, false, pC, vC, wLateral, w2, w3, fContact, localSlipVelocity);

	//special joint transformation matrix
	Matrix3D AJ1(3, 3, {
		wLateral[0], w2[0], parameters.planeNormal[0],
		wLateral[1], w2[1], parameters.planeNormal[1],
		wLateral[2], w2[2], parameters.planeNormal[2] });
	////transpose of special joint transformation matrix
	//Matrix3D AJ1T(3, 3, {
	//	wLateral[0], wLateral[1], wLateral[2],
	//	w2[0], w2[1], w2[2],
	//	w3[0], w3[1], w3[2]});
	Vector3D vSlip = AJ1.GetTransposed()*vC; //splip velocity in special joint coordinates; computed by neglegting the rotation of the wheel for the contact point

	Vector3D vTrail; //velocity of the trail, in joint coordinates; computed by neglegting the rotation of the wheel for the contact point
	Real r = parameters.discRadius;

	//compute trail velocity: point projected from pC to wheel axis:
	//Real cosAlpha = p1[2] / r;
	//REWRITE IN TERMS OF SIN(...) !!! more stable
	Real cosAlpha = -w3 * parameters.planeNormal; //alternative, without using z-coordinate; more accurate because z-drift
	Real sign = 1.;
	if (w3*wLateral > 0) { sign = -1.; }
	//Real lP = r / cosAlpha; //=r^2/p1[2]
	if (cosAlpha*cosAlpha > 1) { cosAlpha = EXUstd::Sgn(cosAlpha); }
	Real axisOffsetX = sign * r * sqrt(1. - cosAlpha * cosAlpha) / cosAlpha;
	Real cosAlpha_t = v1[2] / r;
	Real axisOffsetX_t = 0;
	if (1. - cosAlpha * cosAlpha > 1e-15 && fabs(cosAlpha_t) > 1e-8)
	{
		axisOffsetX_t = sign * r * cosAlpha_t / EXUstd::Square(cosAlpha) *(-1. / sqrt(1. - cosAlpha * cosAlpha) * cosAlpha * cosAlpha - sqrt(1. - cosAlpha * cosAlpha));
	}

	vTrail = v1 - omega1.CrossProduct(A1*Vector3D({ axisOffsetX,0,0 })) - A1 * Vector3D({ axisOffsetX_t,0,0 });
	vTrail[2] = 0; //z-velocity

	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(pC); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(vSlip); break;
	case OutputVariableType::Velocity: value.CopyFrom(vTrail); break;
	case OutputVariableType::ForceLocal:
	{
		value.CopyFrom(fContact); //is already in joint J1 coordinates
		break;
	}
	case OutputVariableType::RotationMatrix: {
		value.SetVector(9, AJ1.GetDataPointer());
		break;
	}
	default:
		SysError("CObjectJointRollingDisc::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

//! function called after Newton method; returns a residual error (force)
Real CObjectConnectorRollingDiscPenalty::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, Index itemIndex, PostNewtonFlags::Type& flags, Real& recommendedStepSize)
{
	Real discontinuousError = 0;
	flags = PostNewtonFlags::_None;

	Real& dataGapState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[2];
	Vector2D dataLocalSlipVelocity({ ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[0],
		((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[1] });

	Vector3D pC; //deviation of contact conditions
	Vector3D vC; //deviation of velocity at contact point
	Vector3D w2; //normalized vector in longitudinal direction
	Vector3D w3; //normalized vector from wheel center to contact point
	Vector3D wLateral; //normalized vector from wheel center to contact point
	Vector3D fContact; //contact force (0=lateral, 1=longitudinal, 2=normal direction), local coordinates
	Vector2D localSlipVelocity;
	ComputeContactForces(markerDataCurrent, parameters, true, pC, vC, wLateral, w2, w3, fContact, localSlipVelocity);
	Real currentGap = pC * parameters.planeNormal;

	//delete: Real previousState = currentState;
	if ((currentGap > 0 && dataGapState <= 0) || (currentGap <= 0 && dataGapState > 0)) //action: state1=dataGapState, error = |currentGap*k|
	{
		discontinuousError += fabs(currentGap * parameters.contactStiffness);

		//contact has changed ==> slipping was wrong anyway (0 => F_slip or F_slip => 0)
		Vector2D slipForce = ComputeSlipForce(parameters, localSlipVelocity, localSlipVelocity, fContact[2]);
		discontinuousError += slipForce.GetL2Norm();
		
		//pout << "PN " << markerDataCurrent.GetTime() << ": changed from gap " << dataGapState << " to " << currentGap << "\n";
	}
	else if (currentGap < 0 ) //both states are the same, only interesting if gap is negative
	{
		//old and new state have contact ==> check if right slipping condition has been assumed:
		Vector2D dataSlipForce = ComputeSlipForce(parameters, localSlipVelocity, dataLocalSlipVelocity, fContact[2]);
		Vector2D slipForce = ComputeSlipForce(parameters, localSlipVelocity, localSlipVelocity, fContact[2]);

		discontinuousError += (slipForce - dataSlipForce).GetL2Norm(); //always there is an error because slip direction is updated slowly
		
		//if ((slipForce - dataSlipForce).GetL2Norm() != 0)
		//{
		//	pout << "PN " << markerDataCurrent.GetTime() << ": vel= " << localSlipVelocity << ", dataVel= " << dataLocalSlipVelocity << "\n";
		//}
	}

	//store new contact gap and slip velocities:
	((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[0] = localSlipVelocity[0];
	((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[1] = localSlipVelocity[1];
	dataGapState = currentGap;

	return discontinuousError;
}

//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
void CObjectConnectorRollingDiscPenalty::PostDiscontinuousIterationStep()
{

}

