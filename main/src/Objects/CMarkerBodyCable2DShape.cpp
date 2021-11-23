/** ***********************************************************************************************
* @brief        Implementation for CMarkerBodyCable2DShape
*
* @author       Gerstmayr Johannes
* @date         2019-06-13 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CMarkerBodyCable2DShape.h"
#include "Autogenerated/CObjectANCFCable2D.h"
#include "Autogenerated/CObjectALEANCFCable2D.h"


void CMarkerBodyCable2DShape::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	position = ((CObjectBody*)(cSystemData.GetCObjects()[parameters.bodyNumber]))->GetPosition(Vector3D({0.,0.,0.}), configuration);
}

//void CMarkerBodyCable2DShape::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
//{
//	velocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetVelocity(configuration);
//}

//very specific marker for Cable2D shape; this is not the usual way to compute markerdata!
void CMarkerBodyCable2DShape::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
{
	//maxNumberOfSegments ... used for ConstSizeVectors

	//the markerdata consists of the shape functions SV(pos_i)*q_Cable, in which pos_i is evaluated at (numberOfSegments+1) equidistant points 
	Index n = cSystemData.GetCObjects()[parameters.bodyNumber]->GetNumberOfNodes(); //2=ANCF, 3=ALEANCF
	bool isALE = false;
	if (n == 3) { isALE = true; }

	CObjectANCFCable2DBase* cable = ((CObjectANCFCable2DBase*)(cSystemData.GetCObjects()[parameters.bodyNumber]));

	Index nCoordinatesCable = cable->GetODE2Size(); //gives 9 for ALE
	//Real L = cable->GetParameters().physicsLength;
	Real L = cable->GetLength();
	const Index ns = 4;   //number of shape functions

	Index nPoints = parameters.numberOfSegments + 1;
	Real nSegments = (Real)parameters.numberOfSegments;


	ConstSizeVector<ns> q0DisplRef;	//coordinates (displacement+reference) node 0
	ConstSizeVector<ns> q1DisplRef; //coordinates (displacement+reference) node 1
	cable->ComputeCurrentNodeCoordinates(q0DisplRef, q1DisplRef);
	LinkedDataVector q0(q0DisplRef);
	LinkedDataVector q1(q1DisplRef);

	ConstSizeVector<ns> q0VelConst;		//velocity coordinates node 0
	ConstSizeVector<ns> q1VelConst;		//velocity coordinates node 1
	cable->ComputeCurrentNodeVelocities(q0VelConst, q1VelConst);
	LinkedDataVector q0Vel(q0VelConst);
	LinkedDataVector q1Vel(q1VelConst);

	if (computeJacobian)
	{
		markerData.jacobian.SetNumberOfRowsAndColumns(nPoints*2, nCoordinatesCable); //stores the shape functions; very specific for ANCF cable
		markerData.jacobian.SetAll(0.);
	}

	markerData.vectorValue.SetNumberOfItems(nPoints * 2); //stores pairs of (x,y) positions of segment points! all set to zero
	markerData.vectorValue_t.SetNumberOfItems(nPoints * 2); //stores pairs of (x,y) velocities of segment points! all set to zero

	for (Index i = 0; i < nPoints; i++) //iterate over nPoints, including endpoints!
	{
		Real x = (Real)i*L / nSegments;
		Vector4D SV = cable->ComputeShapeFunctions(x, L); //could be precomputed and stored!

		Vector2D pos = cable->MapCoordinates(SV, q0, q1);
		markerData.vectorValue[i * 2] = pos[0];			//x-position
		markerData.vectorValue[i * 2 + 1] = pos[1];		//y-position

		Vector2D vel = cable->MapCoordinates(SV, q0Vel, q1Vel);
#ifdef __GNUC__
		Vector2D rx(0); //suppress warnings
#else
		Vector2D rx;
#endif
		if (isALE)
		{
			Vector4D SVx = cable->ComputeShapeFunctions_x(x, L); //could be precomputed and stored!
			rx = cable->MapCoordinates(SVx, q0, q1);
			Real vALE = ((CNodeODE2*)(((CObjectALEANCFCable2D*)cable)->GetCNode(2)))->GetCurrentCoordinateVector_t()[0];
			vel[0] += vALE * rx[0]; //add Eulerian term
			vel[1] += vALE * rx[1]; //add Eulerian term
		}

		markerData.vectorValue_t[i * 2] = vel[0];		//x-velocity
		markerData.vectorValue_t[i * 2 + 1] = vel[1];	//y-velocity

		if (computeJacobian)
		{
			for (Index j = 0; j < ns; j++)
			{	//compare this to positionJacobian of ANCFCable2D element ==> looks similar
				markerData.jacobian(2 * i, 2 * j) = SV[j];		//x-action on Cable element; this matrix could be stored in marker ...
				markerData.jacobian(2 * i + 1, 2 * j + 1) = SV[j];	//y-action on Cable element; this matrix could be stored in marker ...
			}
			if (isALE)
			{
				markerData.jacobian(2 * i, 2 * ns) = rx[0];		//x-action on Cable element; this matrix could be stored in marker ...
				markerData.jacobian(2 * i + 1, 2 * ns) = rx[1];	//y-action on Cable element; this matrix could be stored in marker ...
			}
		}
	}
	markerData.velocityAvailable = true;


}

//! compute markerdata: fill in according data for derivative of jacobian times vector v, e.g.: d(Jpos.T @ v)/dq
void CMarkerBodyCable2DShape::ComputeMarkerDataJacobianDerivative(const CSystemData& cSystemData, const Vector& v, MarkerData& markerData) const
{
	markerData.positionJacobianDerivative.SetNumberOfRowsAndColumns(0, 0); //signals that there is no dependency!
}



