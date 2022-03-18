/** ***********************************************************************************************
* @brief        Implementation of CObjectConnectorSpringDamper
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorSpringDamper.h"


//compute the properties which are needed for computation of LHS and needed for OutputVariables
void CObjectConnectorSpringDamper::ComputeConnectorProperties(const MarkerDataStructure& markerData, Index itemIndex,
	Vector3D& relPos, Vector3D& relVel, Real& force, Vector3D& forceDirection) const
{
	relPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position);
	Real springLength = relPos.GetL2Norm();
	Real springLengthInv;

	if (springLength != 0.) { springLengthInv = 1. / springLength; }
	else { springLengthInv = 1.; SysError("CObjectConnectorSpringDamper::ComputeODE2LHS: springLength = 0"); }

	//unit direction and relative velocity of spring-damper
	forceDirection = springLengthInv * relPos;
	relVel = (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	//stiffness term; this is the term without the jacobian [delta l_vec]; compare Shabana MultibodyDynamics1998, page 119:
	force = 0;
	if (parameters.activeConnector)
	{
		if (!parameters.springForceUserFunction)
		{
			// delta W_spring = k*(l-l0)*(1/l)*l_vec* [delta l_vec]
			force += (parameters.stiffness * (springLength - parameters.referenceLength));

			//damping term  + force:
			force += (parameters.damping * (relVel*forceDirection - parameters.velocityOffset)) + parameters.force;
		}
		else
		{
			Real forceAdd;
			EvaluateUserFunctionForce(forceAdd, cSystemData->GetMainSystemBacklink(), markerData.GetTime(), itemIndex,
				springLength - parameters.referenceLength, relVel*forceDirection - parameters.velocityOffset);
			force += forceAdd;
		}
	}
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
//  MODEL: f
void CObjectConnectorSpringDamper::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	//relative position, spring length and inverse spring length
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectConnectorSpringDamper::ComputeODE2LHS: marker do not provide velocityLevel information");

	//link separate vectors to result (ode2Lhs) vector
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.); //this is the default; used if !activeConnector

	if (parameters.activeConnector)
	{
		Real force;
		Vector3D relPos, relVel, forceDirection;
		ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);
		Vector3D fVec = force * forceDirection;

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
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, fVec, ldv0);
			ldv0 *= -1.;
		}
	}
}





//! compute global 6D force and torque which is used for computation of derivative of jacobian; used only in combination with ComputeJacobianODE2_ODE2
void CObjectConnectorSpringDamper::ComputeJacobianForce6D(const MarkerDataStructure& markerData, Index objectNumber, Vector6D& force6D) const
{
	if (parameters.activeConnector)
	{
		Real force;
		Vector3D relPos, relVel, forceDirection;
		ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);
		Vector3D fVec = force * forceDirection;

		force6D.SetVector({ fVec[0], fVec[1], fVec[2], 0., 0., 0. });
	}
	else { force6D.SetAll(0.); }
}

//! Computational function: compute Jacobian of \hac{ODE2} \ac{LHS} equations w.r.t. ODE2 coordinates and ODE2 velocities; write either dense local jacobian into dense matrix of MatrixContainer or ADD sparse triplets INCLUDING ltg mapping to sparse matrix of MatrixContainer
void CObjectConnectorSpringDamper::ComputeJacobianODE2_ODE2(EXUmath::MatrixContainer& jacobianODE2, JacobianTemp& temp,
	Real factorODE2, Real factorODE2_t,
	Index objectNumber, const ArrayIndex& ltg, const MarkerDataStructure& markerData) const
{
	//approx. 3.5 times faster than numerical jacobian!
	if (parameters.activeConnector)
	{
		temp.localJacobian.SetNumberOfRowsAndColumns(3, 3);
		//temp.localJacobian.SetAll(0.);
		//compute inner jacobian: factorODE2 * d(F)/(dq) + factorODE2_t * d(F)/(dq_t)
		Real force;
		Vector3D relPos, relVel, forceDirection;
		ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);

		//Real factor = parameters.stiffness * factorODE2 + parameters.damping * factorODE2_t;
		//temp.localJacobian.SetWithDiadicProduct(factor*forceDirection, forceDirection);

		//Real factor = parameters.stiffness * factorODE2 + parameters.damping * factorODE2_t;
		Real L = relPos.GetL2Norm();
		//Real L = relPos * forceDirection;
		Real Linv = 1. / L;

		Matrix3D innerJac;

		Matrix3D IsubVV2;
		IsubVV2.SetWithDiadicProduct((-Linv)*forceDirection, forceDirection);
		IsubVV2(0, 0) += Linv;
		IsubVV2(1, 1) += Linv;
		IsubVV2(2, 2) += Linv;

		//force times derivative of relPos
		innerJac = (factorODE2 * force)*IsubVV2;

		//derivative of velocity term in force w.r.t. relPos times direction
		Matrix3D VV1;
		VV1.SetWithDiadicProduct((factorODE2 * parameters.damping)*forceDirection, relVel);
		innerJac += VV1*IsubVV2;

		//derivative of force w.r.t. relPos (k) and relVel (d) times direction
		Matrix3D VV2;
		VV2.SetWithDiadicProduct((factorODE2*parameters.stiffness + factorODE2_t * parameters.damping)*forceDirection, forceDirection);
		innerJac += VV2;

		////velocity term:
		//Matrix3D VV3;
		//VV3.SetWithDiadicProduct(factorODE2_t * parameters.damping * forceDirection, forceDirection);
		//innerJac += VV3;

		//else //compute numerical jacobian, agrees 100% with analytical
		//{
		//	//const ResizableMatrix& jac0 = markerData.GetMarkerData(0).positionJacobian;
		//	//const ResizableMatrix& jac1 = markerData.GetMarkerData(1).positionJacobian;

		//	////CHECKandTHROWstring("ERROR: illegal call to CObjectConnectorCartesianSpringDamper::ComputeJacobianODE2_ODE2");
		//	//Index n0 = jac0.NumberOfColumns();
		//	//Index n1 = jac1.NumberOfColumns();
		//	innerJac.SetScalarMatrix(3, 0.);

		//	Real force;
		//	Vector3D relPos, relVel, forceDirection;
		//	ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);
		//	Vector3D f0 = force * forceDirection;

		//	LinkedDataVector pos0(markerData.GetMarkerData(0).position);
		//	LinkedDataVector pos1(markerData.GetMarkerData(1).position);
		//	LinkedDataVector vel0(markerData.GetMarkerData(0).velocity);
		//	LinkedDataVector vel1(markerData.GetMarkerData(1).velocity);

		//	Real eps = 1e-8;
		//	Real store;
		//	Vector3D f1;
		//	for (Index j = 0; j < 3; j++)
		//	{
		//		store = pos1[j];
		//		pos1[j] += eps;
		//		ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);
		//		pos1[j] = store;
		//		f1 = force * forceDirection;
		//		for (Index i = 0; i < 3; i++)
		//		{
		//			innerJac(i, j) += factorODE2*(1. / eps)*(f1[i] - f0[i]);
		//		}

		//		store = vel1[j];
		//		vel1[j] += eps;
		//		ComputeConnectorProperties(markerData, objectNumber, relPos, relVel, force, forceDirection);
		//		vel1[j] = store;
		//		f1 = force * forceDirection;
		//		for (Index i = 0; i < 3; i++)
		//		{
		//			innerJac(i, j) += factorODE2_t*(1. / eps)*(f1[i] - f0[i]);
		//		}


		//	}

		//}
		temp.localJacobian.CopyFrom(innerJac);

	}
	
	//compute jacobianODE2 in dense mode; temp.localJacobian is modified!
	ComputeJacobianODE2_ODE2generic(temp.localJacobian, jacobianODE2, temp, factorODE2, factorODE2_t, objectNumber, markerData,
		parameters.activeConnector, false, false);
	//pout << jacobianODE2.GetEXUdenseMatrix() << "\n";



}

//! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags
JacobianType::Type CObjectConnectorSpringDamper::GetAvailableJacobians() const
{
//inner jacobian agrees with numerical jacobian, but d(vel)/dpos not yet included!
//#ifdef EXUDYN_RELEASE
//	bool jacAnalytic = false;
//#else
	bool jacAnalytic = true;
//#endif
	if (!parameters.springForceUserFunction && jacAnalytic) {
		return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t + JacobianType::ODE2_ODE2_function + JacobianType::ODE2_ODE2_t_function);
	}
	else {
		return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t);
	}
}






//! provide according output variable in "value"
void CObjectConnectorSpringDamper::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	Real force;
	Vector3D relPos, relVel, forceDirection;
	ComputeConnectorProperties(markerData, itemIndex, relPos, relVel, force, forceDirection);

	switch (variableType)
	{
	case OutputVariableType::Distance: value.SetVector({ relPos.GetL2Norm() }); break;
	case OutputVariableType::Displacement: value.CopyFrom(relPos); break;
	case OutputVariableType::Velocity: value.CopyFrom(relVel); break;
	case OutputVariableType::Force: value.CopyFrom(force*forceDirection); break;
	default:
		SysError("CObjectConnectorSpringDamper::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

