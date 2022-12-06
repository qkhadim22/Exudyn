/** ***********************************************************************************************
* @brief        CObjectBeamGeometricallyExact2D implementation
*
* @author       Gerstmayr Johannes
* @date         2021-03-21 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
//#include "Objects/CObjectANCFCable2DBase.h"
#include "Autogenerated/CObjectBeamGeometricallyExact2D.h"


Vector2D CObjectBeamGeometricallyExact2D::ComputeShapeFunctions(Real x) const
{
	Real lElem = parameters.physicsLength;
	return Vector2D({ (lElem*0.5 - x) / lElem, (lElem*0.5 + x) / lElem });
}

Vector2D CObjectBeamGeometricallyExact2D::ComputeShapeFunctions_x(Real x) const
{
	Real lElem = parameters.physicsLength;
	return Vector2D({ -1 / lElem , 1 / lElem });
}

//! map element coordinates (position or velocity level) given by nodal vectors q0 and q1 onto compressed shape function vector to compute position, etc.
Vector3D CObjectBeamGeometricallyExact2D::MapCoordinates(const Vector2D& SV, const LinkedDataVector& q0, const LinkedDataVector& q1) const
{
	return Vector3D({ SV[0] * q0[0] + SV[1] * q1[0], 
					  SV[0] * q0[1] + SV[1] * q1[1], 
					  SV[0] * q0[2] + SV[1] * q1[2]});
}

Matrix2D CObjectBeamGeometricallyExact2D::GetRotationMatrix2D(Real theta) const
{
	return Matrix2D(2, 2, {cos(theta), -sin(theta), sin(theta), cos(theta)}); //rotation about z-axis, stored in 2D matrix
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! Computational function: compute mass matrix
void CObjectBeamGeometricallyExact2D::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	if (massMatrixComputed) //advantage due to precomputed mass matrix comparatively small (will improve in case of sparse matrices)
	{
		massMatrix.CopyFrom(precomputedMassMatrix); //just assignement; ConstSizeMatrix is directly assigned to Matrix (no double copy)
	}
	else
	{
		massMatrix.SetScalarMatrix(nODE2coordinates, 0.); //set 6x6 matrix

		Real a = -0.5*parameters.physicsLength;
		Real b = 0.5*parameters.physicsLength;
		Real rhoA = parameters.physicsMassPerLength;
		Real rhoJ = parameters.physicsCrossSectionInertia;

		Index cnt = 0;
		Index ns = 2;
		for (auto item : EXUmath::gaussRuleOrder3Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector2D SV = ComputeShapeFunctions(x);
			Real factInt = (0.5*(b - a)*EXUmath::gaussRuleOrder3Weights[cnt++]); //integration factor

			for (Index i = 0; i < ns; i++)
			{
				for (Index j = 0; j < ns; j++)
				{
					massMatrix(i * 3 + 0, j * 3 + 0) += SV[i] * SV[j] * rhoA * factInt;
					massMatrix(i * 3 + 1, j * 3 + 1) += SV[i] * SV[j] * rhoA * factInt;
					massMatrix(i * 3 + 2, j * 3 + 2) += SV[i] * SV[j] * rhoJ * factInt;
				}
			}
		}
		precomputedMassMatrix.CopyFrom(massMatrix); //assignement operator would cause double copy!
		massMatrixComputed = true;
	}
	//pout << "M=" << massMatrix << "\n";
}

//! compute strains and variation of strains for given interpolated derivatives, time derivatives, etc. at position x
void CObjectBeamGeometricallyExact2D::ComputeGeneralizedStrains(Real x, Real& theta, Vector2D& SV, Vector2D& SV_x, 
	Real& gamma1, Real& gamma2, Real& theta_x, Real& gamma1_t, Real& gamma2_t, Real& theta_xt, 
	CSVector6D& deltaGamma1, CSVector6D& deltaGamma2) const
{
	SV = ComputeShapeFunctions(x);
	SV_x = ComputeShapeFunctions_x(x);

	//could be speed up by only computing relevant components!
	//const int ns = 2; //number of shape functions
	CSVector3D qNode0(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector(), 0); //displacement coordinates node 0
	CSVector3D qNode1(((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector(), 0); //displacement coordinates node 1

	CSVector3D qNode0_t(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t(), 0); //displacement coordinates node 0
	CSVector3D qNode1_t(((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector_t(), 0); //displacement coordinates node 1

	CSVector3D qNode0Ref(((CNodeODE2*)GetCNode(0))->GetReferenceCoordinateVector(), 0);
	CSVector3D qNode1Ref(((CNodeODE2*)GetCNode(1))->GetReferenceCoordinateVector(), 0);

	Vector2D referenceSlopeVector({ SV_x[0] * qNode0Ref[0] + SV_x[1] * qNode1Ref[0] ,
									SV_x[0] * qNode0Ref[1] + SV_x[1] * qNode1Ref[1] }); //reference slope vector, r'=d r / dx needed in reference configuration needed for computation of strains

	qNode0Ref += qNode0;
	qNode1Ref += qNode1;

	theta = SV[0] * qNode0Ref[2] + SV[1] * qNode1Ref[2]; //rotations need also reference values

	Real u1_x = SV_x[0] * qNode0[0] + SV_x[1] * qNode1[0]; //here, reference values are not directly meaningful: length L would need to be included
	Real u2_x = SV_x[0] * qNode0[1] + SV_x[1] * qNode1[1];

	if (parameters.includeReferenceRotations)
	{
		theta_x = SV_x[0] * qNode0Ref[2] + SV_x[1] * qNode1Ref[2] - parameters.physicsReferenceCurvature; //in precurved case, reference values shall not contribute to curvature
		//u1_x = SV_x[0] * qNode0Ref[0] + SV_x[1] * qNode1Ref[0];
		//u2_x = SV_x[0] * qNode0Ref[1] + SV_x[1] * qNode1Ref[1];
	}
	else
	{
		theta_x = SV_x[0] * qNode0[2] + SV_x[1] * qNode1[2] - parameters.physicsReferenceCurvature; //in precurved case, reference values shall not contribute to curvature
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Real cosTheta = cos(theta);
	Real sinTheta = sin(theta);
	const Vector2D& RS = referenceSlopeVector;
	gamma1 = cosTheta * (RS[0] + u1_x - cosTheta) + sinTheta * (RS[1] + u2_x - sinTheta);

	Real termGamma1 = (-sinTheta * (RS[0] + u1_x - cosTheta) + cosTheta * sinTheta + cosTheta * (RS[1] + u2_x - sinTheta) - sinTheta * cosTheta);

	deltaGamma1 = CSVector6D({ cosTheta*SV_x[0], sinTheta*SV_x[0], termGamma1*SV[0],
			cosTheta*SV_x[1], sinTheta*SV_x[1], termGamma1*SV[1] });

	gamma2 = -sinTheta * (RS[0] + u1_x - cosTheta) + cosTheta * (RS[1] + u2_x - sinTheta);

	Real termGamma2 = (-cosTheta * (RS[0] + u1_x - cosTheta) - sinTheta * sinTheta - sinTheta * (RS[1] + u2_x - sinTheta) - cosTheta * cosTheta);

	deltaGamma2 = CSVector6D({ -sinTheta * SV_x[0], cosTheta * SV_x[0], termGamma2*SV[0],
			-sinTheta * SV_x[1], cosTheta * SV_x[1], termGamma2*SV[1] });

	theta_xt = 0.;
	gamma1_t = 0.;
	gamma2_t = 0.;

	if (parameters.physicsBendingDamping != 0.)
	{
		theta_xt = SV_x[0] * qNode0_t[2] + SV_x[1] * qNode1_t[2]; //in precurved case, reference values shall not contribute to curvature
	}

	if (parameters.physicsAxialDamping != 0. || parameters.physicsShearDamping != 0.)
	{
		CSVector6D elementQ_t({ qNode0_t[0], qNode0_t[1], qNode0_t[2],
			qNode1_t[0], qNode1_t[1], qNode1_t[2] }); //element velocity coordinates

		gamma1_t = deltaGamma1 * elementQ_t;
		gamma2_t = deltaGamma2 * elementQ_t;
	}

}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to 'ode2Lhs'
void CObjectBeamGeometricallyExact2D::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	ode2Lhs.SetNumberOfItems(nODE2coordinates);
	ode2Lhs.SetAll(0.);

	//compute elastic forces at midpoint:
	Real L = parameters.physicsLength;
	Real x = 0; //midpoint, beam goes from -L/2 ... L/2

	Vector2D SV, SV_x;
	Real theta, gamma1, gamma2, theta_x, gamma1_t, gamma2_t, theta_xt;
	CSVector6D deltaGamma1, deltaGamma2;

	ComputeGeneralizedStrains(x, theta, SV, SV_x, 
		gamma1, gamma2, theta_x, gamma1_t, gamma2_t, theta_xt, 
		deltaGamma1, deltaGamma2);

	Real fact = L; //integration factor

	ode2Lhs.MultAdd((parameters.physicsAxialDamping*gamma1_t + parameters.physicsAxialStiffness*gamma1)*fact, deltaGamma1);
	ode2Lhs.MultAdd((parameters.physicsShearDamping*gamma2_t + parameters.physicsShearStiffness*gamma2)*fact, deltaGamma2);
	ode2Lhs[2 + 0] += (parameters.physicsBendingDamping*theta_xt + parameters.physicsBendingStiffness*theta_x)*fact*SV_x[0];
	ode2Lhs[2 + 3] += (parameters.physicsBendingDamping*theta_xt + parameters.physicsBendingStiffness*theta_x)*fact*SV_x[1];

}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectBeamGeometricallyExact2D::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + 
		(Index)AccessFunctionType::AngularVelocity_qt +
		(Index)AccessFunctionType::JacobianTtimesVector_q+
		(Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in 'value' according to object access
void CObjectBeamGeometricallyExact2D::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	//Real L = parameters.physicsLength;

	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate
		Vector2D SV = ComputeShapeFunctions(x);
		value.SetNumberOfRowsAndColumns(3, nODE2coordinates); //3D velocity, 6 coordinates qt

		value.SetAll(0.);
		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 3) = SV[1];
		value(1, 4) = SV[1];

		CHECKandTHROW(localPosition[1] == 0, "CObjectBeamGeometricallyExact2D::GetAccessFunctionBody (for MarkerBody): only implemented if localPosition[1]==0");

		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate

		value.SetNumberOfRowsAndColumns(3, nODE2coordinates); //3D velocity, 6 coordinates qt
		value.SetAll(0.); //last row not necessary to set to zero ... 
		
		Vector2D SV = ComputeShapeFunctions(x);
		value(2, 2) = SV[0];
		value(2, 5) = SV[1];

		break;
	}
	case AccessFunctionType::JacobianTtimesVector_q: //jacobian w.r.t. global position and global orientation!!!
	{
		CHECKandTHROW(localPosition[1] == 0, "CObjectBeamGeometricallyExact2D::GetAccessFunctionBody [JacobianTtimesVector_q] (for MarkerBody): only implemented if localPosition[1]==0");
		value.SetNumberOfRowsAndColumns(0, 0); //indicates that all entries are zero
		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		value.SetNumberOfRowsAndColumns(3, nODE2coordinates); //3D velocity, 6 coordinates qt
		value.SetAll(0.);

		Real L = parameters.physicsLength;
		Real rhoA = parameters.physicsMassPerLength;

		Vector2D SV = rhoA * L * ComputeShapeFunctions(0.); //0=midpoint at axis

		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 3) = SV[1];
		value(1, 4) = SV[1];
		break;
	}
	default:
		SysError("CObjectBeamGeometricallyExact2D:GetAccessFunctionBody illegal accessType");
	}

}

//! provide according output variable in 'value'
void CObjectBeamGeometricallyExact2D::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, 
	ConfigurationType configuration, Vector& value, Index objectNumber) const
{
	Real theta, gamma1, gamma2, theta_x, gamma1_t, gamma2_t, theta_xt;
	CSVector6D deltaGamma1, deltaGamma2;

	if (!EXUstd::IsOfType((Index)variableType, (Index)OutputVariableType::Position + (Index)OutputVariableType::Displacement +
		(Index)OutputVariableType::Velocity + (Index)OutputVariableType::Rotation))
	{
		//compute elastic forces at midpoint:
		//Real L = parameters.physicsLength;
		//Real x = 0; //midpoint, beam goes from -L/2 ... L/2

		Vector2D SV, SV_x;

		ComputeGeneralizedStrains(localPosition[0], theta, SV, SV_x,
			gamma1, gamma2, theta_x, gamma1_t, gamma2_t, theta_xt,
			deltaGamma1, deltaGamma2);
	}

	switch (variableType)
	{
	case OutputVariableType::Position:		value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetDisplacement(localPosition, configuration)); break;
	case OutputVariableType::Velocity:		value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	case OutputVariableType::Rotation:		value.SetVector({ 0., 0., GetRotation(localPosition, configuration) }); break;
	case OutputVariableType::StrainLocal:	value.SetVector({gamma1, 0., 0.,  0., 0., gamma2}); break;
	case OutputVariableType::CurvatureLocal:value.SetVector({ 0., 0., theta_x }); break;
	case OutputVariableType::ForceLocal:	value.SetVector({ (parameters.physicsAxialDamping*gamma1_t + parameters.physicsAxialStiffness*gamma1),
		(parameters.physicsShearDamping*gamma2_t + parameters.physicsShearStiffness*gamma2), 0. }); break;
	case OutputVariableType::TorqueLocal:	value.SetVector({ 0., 0., (parameters.physicsBendingDamping*theta_xt + parameters.physicsBendingStiffness*theta_x) }); break;
	default:
		SysError("CObjectBeamGeometricallyExact2D::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//! return the (global) position of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}
	//include off-axis position:
	if (localPosition[1] != 0.)
	{
		Vector2D p = GetRotationMatrix2D(u[2]) * Vector2D({ 0,localPosition[1] });
		u[0] += p[0];
		u[1] += p[1];
	}

	return Vector3D({ u[0], u[1],0. });

}

//! return the (global) displacemnet of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference);
}

//! return the (global) velocity of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	Vector3D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));

	//include off-axis position:
	if (localPosition[1] != 0.)
	{
		//add velocity due to rotation of cross section
		Vector2D vRot = GetRotationMatrix2D(u[2]) * Vector2D({ -v[2]*localPosition[1] , 0}); //omega x locPos
		v[0] += vRot[0];
		v[1] += vRot[1];
	}

	return Vector3D({ v[0],v[1],0. });

}

//! return configuration dependent rotation matrix of node; returns always a 3D Matrix, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Matrix3D CObjectBeamGeometricallyExact2D::GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	return Matrix3D(3, 3, { cos(u[2]), -sin(u[2]), 0,
							sin(u[2]),  cos(u[2]), 0,
							0,          0,         1 }); //rotation about z-axis, stored in 3D matrix
}

//! return configuration dependent rotation matrix of node; returns always a 3D Matrix, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Real CObjectBeamGeometricallyExact2D::GetRotation(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	return u[2];
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Vector3D CObjectBeamGeometricallyExact2D::GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));
	return Vector3D({0,0,v[2]});
}





