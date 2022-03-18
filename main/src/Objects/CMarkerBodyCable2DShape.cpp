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
	const Index dim = 2;
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

	Vector2D r_x(0.); //suppress warnings
	Real verticalOffset = parameters.verticalOffset;
	Vector4D SVx(0.);
	CHECKandTHROW(!(isALE && verticalOffset != 0.), "CMarkerBodyCable2DShape::ComputeMarkerData: for ALEANCFCable2D elements, the verticalOffset must be zero");

	for (Index i = 0; i < nPoints; i++) //iterate over nPoints, including endpoints!
	{
		Real x = (Real)i*L / nSegments;
		Vector4D SV = cable->ComputeShapeFunctions(x, L); //could be precomputed and stored!

		Vector2D pos = cable->MapCoordinates(SV, q0, q1);

		Vector2D vel = cable->MapCoordinates(SV, q0Vel, q1Vel);

		if (isALE || verticalOffset != 0.)
		{
			SVx = cable->ComputeShapeFunctions_x(x, L); //could be precomputed and stored!
			r_x = cable->MapCoordinates(SVx, q0, q1);
		}
		if (isALE)
		{
			Real vALE = ((CNodeODE2*)(((CObjectALEANCFCable2D*)cable)->GetCNode(2)))->GetCurrentCoordinateVector_t()[0];
			
			vel[0] += vALE * r_x[0]; //add Eulerian term
			vel[1] += vALE * r_x[1]; //add Eulerian term
		}

		Real rxNorm = 0.;
		Real rxNormInv = 0.;
		if (verticalOffset != 0.)
		{
			rxNorm = r_x.GetL2Norm();
			if (rxNorm != 0.)
			{
				rxNormInv = 1. / rxNorm;
			}
			Vector2D n({ -r_x[1] * rxNormInv, r_x[0] * rxNormInv });
			pos += verticalOffset * n;


			Real x = r_x[0]; //x-slopex
			Real y = r_x[1]; //y-slopex

			Vector2D slope_t = cable->MapCoordinates(SVx, ((CNodeODE2*)cable->GetCNode(0))->GetCurrentCoordinateVector_t(), ((CNodeODE2*)cable->GetCNode(1))->GetCurrentCoordinateVector_t());

			//verticalOffset not for ALE case, see exception above; would require additional terms
			Real angVel = (-y * slope_t[0] + x * slope_t[1]) / (x*x + y * y);
			vel += (-verticalOffset * angVel * rxNormInv) * r_x;
		}
		markerData.vectorValue[i * 2] = pos[0];			//x-position
		markerData.vectorValue[i * 2 + 1] = pos[1];		//y-position
		markerData.vectorValue_t[i * 2] = vel[0];		//x-velocity
		markerData.vectorValue_t[i * 2 + 1] = vel[1];	//y-velocity

		if (computeJacobian)
		{
			for (Index j = 0; j < ns; j++)
			{	//compare this to positionJacobian of ANCFCable2D element 
				//jacobian takes 2 rows and dim*ns columns for every contact point
				markerData.jacobian(dim * i, dim * j) = SV[j];		//x-action on Cable element; this matrix could be stored in marker ...
				markerData.jacobian(dim * i + 1, dim * j + 1) = SV[j];	//y-action on Cable element; this matrix could be stored in marker ...
			}
			if (verticalOffset != 0.)
			{
				//see CObjectANCFCable2DBase::GetAccessFunctionBody
				Vector2D n({ -r_x[1], r_x[0] }); //not normalized!
				Vector2D t0 = rxNormInv * r_x;

				////v = r_t + (-verticalOffset*omega)*t0
				////omega = (n * r'_t)/(r')^2 = (-r'y*r'_xt + r'x*r'_yt)/(r')^2 //see CNodePoint2DSlope1::GetAngularVelocity
				////dv/dq_t = [S-y*diadic(t0, omega_qt)] = [S - y/(r')^2 * diadic(t0, n) * S')

				for (Index j = 0; j < ns; j++)
				{
					for (Index k = 0; k < dim; k++)
					{
						Real omega_qt = n[k] * SVx[j] * (rxNormInv*rxNormInv);
						markerData.jacobian(dim * i + 0, dim * j + k) -= verticalOffset * omega_qt * t0[0];
						markerData.jacobian(dim * i + 1, dim * j + k) -= verticalOffset * omega_qt * t0[1];

						//value(k, i * dim + k) += SV[i]; //done above
					}

				}
			}
			if (isALE)
			{
				//verticalOffset not considered, see exception above
				//the following term is only written to last row of jacobian!
				markerData.jacobian(dim * i, dim * ns) = r_x[0];		//x-action on Cable element; this matrix could be stored in marker ...
				markerData.jacobian(dim * i + 1, dim * ns) = r_x[1];	//y-action on Cable element; this matrix could be stored in marker ...
			}
		}
	}
	markerData.velocityAvailable = true;


}

//! compute markerdata: fill in according data for derivative of jacobian times vector v, e.g.: d(Jpos.T @ v)/dq
void CMarkerBodyCable2DShape::ComputeMarkerDataJacobianDerivative(const CSystemData& cSystemData, const Vector6D& v6D, MarkerData& markerData) const
{
	markerData.jacobianDerivative.SetNumberOfRowsAndColumns(0, 0); //signals that there is no dependency!
}



