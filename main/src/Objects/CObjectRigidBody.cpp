/** ***********************************************************************************************
* @brief        CObjectRigidBody implementation
*
* @author       Gerstmayr Johannes
* @date         2019-10-19 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CNodeRigidBodyEP.h"
#include "Autogenerated/CObjectRigidBody.h"
#include "Utilities/RigidBodyMath.h"

//! Computational function: compute mass matrix
void CObjectRigidBody::ComputeMassMatrix(Matrix& massMatrix) const
{
	//set mass terms in first 3 diagonal entries and set remaining entries to zero (the last 4x4 entries will be overwritten when filling in inertia terms)
	massMatrix.SetScalarMatrix(nODE2Coordinates, parameters.physicsMass);

	//old code: inertia = (GetGbarT()*Iphi)*GetGbar();
	
	ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
	ConstSizeMatrix<nRotationCoordinates * 3> Glocal = RigidBodyMath::EP2Glocal(ep);
	//ConstSizeMatrix<nRotationCoordinates * 3> GlocalT = Glocal.GetTransposed();
	
	ConstSizeMatrix<9> localInertia;
	RigidBodyMath::ComputeInertiaMatrix(parameters.physicsInertia, localInertia);

	ConstSizeMatrix<nRotationCoordinates * 3> GlocalInertia;			//temporary matrix
	ConstSizeMatrix<nRotationCoordinates*nRotationCoordinates> inertia; //final inertia term

	//EXUmath::MultMatrixMatrix(GlocalT, localInertia, GlocalInertia);
	EXUmath::MultMatrixTransposedMatrix(Glocal, localInertia, GlocalInertia);
	EXUmath::MultMatrixMatrix(GlocalInertia, Glocal, inertia);

	//pout << "inertia=" << inertia << "\n";
	//delete:
	//ConstSizeVector<nRotationCoordinates> ev({ 0.936055553906925, 0.300000000000000,0.170000000000000,0.070000000000000 });
	//pout << "A=" << RigidBodyMath::EP2RotationMatrix(ev) << "\n";

	for (int i = 0; i < nRotationCoordinates; i++)
	{
		for (int j = 0; j < nRotationCoordinates; j++)
			{ massMatrix(3 + i, 3 + j) = inertia(i, j); }
	}
}

//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
void CObjectRigidBody::ComputeODE2RHS(Vector& ode2Rhs) const
{
	ode2Rhs.SetNumberOfItems(nODE2Coordinates);
	ode2Rhs.SetAll(0.);

	ConstSizeMatrix<9> localInertia;
	RigidBodyMath::ComputeInertiaMatrix(parameters.physicsInertia, localInertia);

	ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
	ConstSizeVector<nRotationCoordinates> ep_t = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters_t();
	//pout << "ep = " << ep << "\n";
	//pout << "ep_t=" << ep_t << "\n";

	ConstSizeMatrix<nRotationCoordinates * 3> Glocal = RigidBodyMath::EP2Glocal(ep);

	//compute: Glocal^T * (omegaBar.Cross(localInertia*omegaBar))
	Vector3D omegaBar;
	EXUmath::MultMatrixVector(Glocal, ep_t, omegaBar);
	//pout << "omegaBar=" << omegaBar << "\n";

	//+++++++++++++++++++++++++++++++++++++
	//Version1 (different to Version2 (gives different forcesEP; difference acting in the nullspace of ep):
	Vector3D temp = omegaBar.CrossProduct(localInertia * omegaBar);
	ConstSizeVector<nRotationCoordinates> forcesEP; //forces acting on Euler parameter coordinates
	EXUmath::MultMatrixTransposedVector(Glocal, temp, forcesEP);

	//+++++++++++++++++++++++++++++++++++++
	//Version2:
	//RHS = -2*Gbar_t^T*Ibar*omegaBar
	//omegaBar *= 2.;
	//Vector3D temp = localInertia * omegaBar;
	//ConstSizeVector<nRotationCoordinates> forcesEP; //forces acting on Euler parameter coordinates
	//ConstSizeMatrix<nRotationCoordinates * 3> Glocal_t = RigidBodyMath::EP_t2Glocal_t(ep_t);
	//EXUmath::MultMatrixTransposedVector(Glocal_t, temp, forcesEP);

	//++++++++++++++
	//ConstSizeMatrix<nRotationCoordinates * 3> Glocal_t = RigidBodyMath::EP_t2Glocal_t(ep_t);
	//Matrix Gbar_tT = Glocal_t;
	//Vector temp2; 
	//temp2.CopyFrom(temp);
	//Vector forcesEP = Gbar_tT.GetTransposed() * temp2;

	//+++++++++++++++++++++++++++++++++++++
	//pout << "Qv = " << -1 * forcesEP << "\n";

	//*******************************
	//original code H1:
	//GetBetaP(betap);
	//Matrix3D Gbar = GetGbar();
	//Vector3D omegabar = Gbar * betap; //--->Mult is faster
	//Vector3D temp = (omegabar.Cross(Iphi*omegabar));
	//Mult(Gbar.GetTp(), temp, betap);

	for (Index i = 0; i < nRotationCoordinates; i++)
	{
		ode2Rhs[i + 3] += forcesEP[i]; //positive sign, because object ODEforces are put on LHS
	}

}

//! Compute algebraic equations part of rigid body
void CObjectRigidBody::ComputeAlgebraicEquations(Vector& algebraicEquations, bool useIndex2) const
{
	algebraicEquations.SetNumberOfItems(1);
	if (!useIndex2)
	{
		//position level constraint:

		ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
		algebraicEquations[0] = ep * ep - 1.;
	}
	else
	{
		//velocity level constraint:
		ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
		ConstSizeVector<nRotationCoordinates> ep_t = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters_t();
		
		algebraicEquations[0] = 2. * (ep * ep_t);
	}
}

//! Compute jacobians of algebraic equations part of rigid body w.r.t. ODE2
void CObjectRigidBody::ComputeJacobianAE(ResizableMatrix& jacobian, ResizableMatrix& jacobian_t, ResizableMatrix& jacobian_AE) const
{
	//markerData contains already the correct jacobians ==> transformed to constraint jacobian
	jacobian.SetNumberOfRowsAndColumns(nAECoordinates, nODE2Coordinates);
	jacobian_t.SetNumberOfRowsAndColumns(0, 0); //for safety!
	jacobian_AE.SetNumberOfRowsAndColumns(0, 0);//for safety!

	ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();

	//jacobian = [0 0 0 2*ep0 2*ep1 2*ep2 2*ep3]
	for (Index i = 0; i < 3; i++) { jacobian(0, i) = 0.; }
	for (Index i = 0; i < nRotationCoordinates; i++) { jacobian(0, 3+i) = 2.*ep[i]; }
}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectRigidBody::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::AngularVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in "value" according to object access
void CObjectRigidBody::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt: //global translational velocity at localPosition derivative w.r.t. qt = L-matrix = [I   -A*uLocalTilde*Glocal]
	{
		//this function relates a 3D translatory velocity to the time derivative of all coordinates: v_trans = Jac*q_dot
		ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
		ConstSizeVector<nRotationCoordinates> ep_t = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters_t();
		ConstSizeMatrix<nRotationCoordinates * 3> Lep = RigidBodyMath::EP2Glocal(ep); //Lep=Glocal now
		ConstSizeMatrix<9> uLocal_tilde = RigidBodyMath::Vector2SkewMatrix(localPosition);
		uLocal_tilde *= -1.;//negative sign in -A*uLocalTilde*Glocal
		//pout << "Glocal  =" << Lep << "\n";
		//pout << "  utilde=" << uLocal_tilde << "\n";
		//pout << "  A     =" << RigidBodyMath::EP2RotationMatrix(ep) << "\n";

		ConstSizeMatrix<nRotationCoordinates * 3> temp; //temporary matrix during computation

		EXUmath::MultMatrixMatrix(uLocal_tilde, Lep, temp);
		//pout << "  temp  =" << temp << "\n";
		EXUmath::MultMatrixMatrix(RigidBodyMath::EP2RotationMatrix(ep), temp, Lep);
		//pout << "  L     =" << Lep << "\n";

		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates);
		//unit matrix
		value(0, 0) = 1.; value(0, 1) = 0.; value(0, 2) = 0.;
		value(1, 0) = 0.; value(1, 1) = 1.; value(1, 2) = 0.;
		value(2, 0) = 0.; value(2, 1) = 0.; value(2, 2) = 1.;

		//-A*uLocalTilde*Glocal part (=L in this case
		for (Index i = 0; i < 3; i++)
		{
			for (Index j = 0; j < nRotationCoordinates; j++)
			{
				value(i, 3+j) = Lep(i, j);
			}
		}


		break;
	}
	case AccessFunctionType::AngularVelocity_qt: //global angular velocity
	{
		//d(omega)/d(q_t): derivative of angular velocity w.r.t. all coordinates_t = [0_{3x3} G-matrix]
		ConstSizeVector<nRotationCoordinates> ep = ((CNodeRigidBodyEP*)GetCNode(0))->GetEulerParameters();
		ConstSizeMatrix<nRotationCoordinates * 3> G = RigidBodyMath::EP2G(ep);

		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates);
		for (Index i = 0; i < 3; i++)
		{
			value(0, i) = 0;
			value(1, i) = 0;
			value(2, i) = 0;
			for (Index j = 0; j < nRotationCoordinates; j++)
			{
				value(i, 3+j) = G(i,j);
			}
		}
		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates);

		Real m = parameters.physicsMass;

		for (Index i = 0; i < 3; i++)
		{
			for (Index j = 0; j < nODE2Coordinates; j++)
			{
				if (i != j) { value(i, j) = 0.; }
				else { value(i, j) = m; } //only diagonal term!
			}
		}

		break;
	}
	default:
		SysError("CObjectRigidBody:GetAccessFunctionBody illegal accessType");
	}
}

//! provide according output variable in "value"
void CObjectRigidBody::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocity(localPosition, configuration)); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(GetAngularVelocityLocal(localPosition, configuration)); break;
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = GetRotationMatrix(localPosition, configuration);
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::Rotation: {
		Matrix3D rotMat = GetRotationMatrix(localPosition, configuration);
		Vector3D rot = RigidBodyMath::RotationMatrix2AngXYZ(rotMat);
		value.SetVector(3, rot.GetDataPointer());
		break;
	}
	default:
		SysError("CObjectRigidBody::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//! @todo: add ConfigurationType to CObjectMassPoint::GetPosition; 
//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectRigidBody::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2AE*)GetCNode(0))->GetPosition(configuration) + ((CNodeODE2AE*)GetCNode(0))->GetRotationMatrix(configuration) * localPosition;

}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectRigidBody::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	// \dot R + A * \localOmega x \localPosition
	return ((CNodeODE2AE*)GetCNode(0))->GetVelocity(configuration) + 
		((CNodeODE2AE*)GetCNode(0))->GetRotationMatrix(configuration) * 
		((CNodeODE2AE*)GetCNode(0))->GetAngularVelocityLocal(configuration).CrossProduct(localPosition); //add omega x r
}

//! return the (global) position of "localPosition" according to configuration type
Vector3D CObjectRigidBody::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2AE*)GetCNode(0))->GetPosition(configuration) - ((CNodeODE2AE*)GetCNode(0))->GetPosition(ConfigurationType::Reference); //this also works for NodePointGround
}

Matrix3D CObjectRigidBody::GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2AE*)GetCNode(0))->GetRotationMatrix(configuration);
}

//! return configuration dependent angular velocity of rigid body; returns always a 3D Vector
Vector3D CObjectRigidBody::GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2AE*)GetCNode(0))->GetAngularVelocity(configuration);
}

//! return configuration dependent local (=body fixed) angular velocity of rigid body; returns always a 3D Vector
Vector3D CObjectRigidBody::GetAngularVelocityLocal(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2AE*)GetCNode(0))->GetAngularVelocityLocal(configuration);
}

