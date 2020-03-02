/** ***********************************************************************************************
* @brief        Implementation of CObjectContactCircleCable2D
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */


#include "Main/CSystemData.h"
#include "Linalg/Geometry.h"
#include "Autogenerated/CNodeGenericData.h"
//#include "Autogenerated/CObjectContactCoordinate.h"

//for consistency checks:
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectContactCircleCable2D.h"


bool MainObjectContactCircleCable2D::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectContactCircleCable2D* cObject = (CObjectContactCircleCable2D*)GetCObject();
	Index node = cObject->GetNodeNumber(0);

	if (cObject->GetParameters().numberOfContactSegments > cObject->maxNumberOfSegments)
	{
		errorString = "ObjectContactCircleCable2D: numberOfContactSegments must be <= " + EXUstd::ToString(cObject->maxNumberOfSegments);
		return false;
	}

	//check for valid node number already done prior to this function
	if (std::strcmp(mainSystem.GetMainSystemData().GetMainNode(node).GetTypeName(), "GenericData") != 0) 
	{
		errorString = "ObjectContactCircleCable2D: node must be of type 'GenericData'";
		return false;
	}

	Index nc = ((const CNodeGenericData&)(cObject->GetCSystemData()->GetCNode(node))).GetNumberOfDataCoordinates();
	if (nc != cObject->GetParameters().numberOfContactSegments)
	{
		errorString = STDstring("ObjectContactCircleCable2D: NodeGenericData (Node ") + EXUstd::ToString(node) + ") must have " + EXUstd::ToString(cObject->GetParameters().numberOfContactSegments)
			+ " coordinates (found: " + EXUstd::ToString(nc) + ")";
		return false;
	}

	if (cObject->GetParameters().contactDamping != 0)
	{
		errorString = STDstring("ObjectContactCircleCable2D: contactDamping is not yet implemented; set parameter to zero");
		return false;
	}

	const ArrayIndex& nMarkers = cObject->GetMarkerNumbers();
	if (!(mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[0]).GetType() & Marker::Position))
	{
		errorString = STDstring("ObjectContactCircleCable2D: Marker 0 must be of type = 'Position'");
		return false;
	}

	if (mainSystem.GetMainSystemData().GetMainMarkers()[nMarkers[1]]->GetTypeName() != "Cable2DShape")
	{
		errorString = STDstring("ObjectContactCircleCable2D: Marker 1 must be of type = 'Cable2DShape'");
		return false;
	}

	return true;
}


//! compute gap for given configuration (current, start of step, ...); gap <= 0 means contact, gap > 0 is no contact
//! gapPerSegment returns the 'area' gap per segment, which is the distance/penetration * segment length (or real penetrating area)
//! referenceCoordinatePerSegment returns the reference coordinate at the segment (range: [0,1]) in case of contact ==> used to apply forces (either this is the nearest point or a vertex of the segment)
//! the x/yDirectionGap show the direction of the gap, in which the contact force should act
void CObjectContactCircleCable2D::ComputeGap(const MarkerDataStructure& markerData, ConstSizeVector<maxNumberOfSegments>& gapPerSegment, 
	ConstSizeVector<maxNumberOfSegments>& referenceCoordinatePerSegment, 
	ConstSizeVector<maxNumberOfSegments>& xDirectionGap, ConstSizeVector<maxNumberOfSegments>& yDirectionGap) const
{
	//circular segment, which cuts a piece with height h off the circle with radius r:
	//    alpha = 2*arccos(1-h/r);			//angle of the cut
	//    s = 2*r*sin(alpha/2);				//length of the cut
	//    A = r^2/2*(alpha - sin(alpha))	//area of the cut

	//markerData contains:
	//  vectorValue = [x0,y0, x1,y1, x2,y2, ... ] ... the x/y positions of the cable points defining the contact segments [x0,y0,x1,y1], [x1,y1,x2,y2], 

	const MarkerData& markerData0 = markerData.GetMarkerData(0); //position based marker
	const MarkerData& markerData1 = markerData.GetMarkerData(1); //ANCFCable2DShape

	//position and radius of circle:
	Vector2D circleCenter({ markerData0.position[0], markerData0.position[1] }); //center of the circle
	const Real& r = parameters.circleRadius;
	const Real& offset = parameters.offset;
	Vector2D contactVector; //vector from contact point to circle midpoint; the contact force acts in opposite direction

	Index nSeg = parameters.numberOfContactSegments;
	gapPerSegment.SetNumberOfItems(nSeg);
	referenceCoordinatePerSegment.SetNumberOfItems(nSeg);
	xDirectionGap.SetNumberOfItems(nSeg);
	yDirectionGap.SetNumberOfItems(nSeg);

	//iterate over all segments:
	for (Index i = 0; i < nSeg; i++)
	{
		//take 2 points as one segment
		Vector2D p0({ markerData1.vectorValue[i * 2], markerData1.vectorValue[i * 2 + 1] });	 //markerdata.value stores the x/y positions of the contact points
		Vector2D p1({ markerData1.vectorValue[i * 2+2], markerData1.vectorValue[i * 2 + 1+2] }); //markerdata.value stores the x/y positions of the contact points

		//compute shortest distance:
		//referenceCoordinatePerSegment is the relative position [0..1] of the shortest projected point at the line segment
		Real distance = HGeometry::ShortestDistanceEndPointsRelativePosition(p0, p1, circleCenter, referenceCoordinatePerSegment[i], contactVector);
		if (distance != 0.) { contactVector *= 1. / distance; } //computes normal vector

		gapPerSegment[i] = distance - r - offset;
		xDirectionGap[i] = -contactVector[0]; //x-component of direction of force
		yDirectionGap[i] = -contactVector[1]; //y-component of direction of force
	}
}

bool CObjectContactCircleCable2D::IsActive() const
{
	if (!parameters.activeConnector) { return false; }
	for (Index i = 0; i < parameters.numberOfContactSegments; i++)
	{
		if (GetCNode(0)->GetCurrentCoordinate(i) <= 0) { return true; }
	}
	return false;
}


//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
//  MODEL: f
void CObjectContactCircleCable2D::ComputeODE2RHS(Vector& ode2Rhs, const MarkerDataStructure& markerData) const
{
	ode2Rhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).jacobian.NumberOfColumns());
	ode2Rhs.SetAll(0.);

	if (parameters.activeConnector)
	{
		//gap>0: no contact, gap<0: contact
	//Real gap = (markerData.GetMarkerData(1).value - markerData.GetMarkerData(0).value - parameters.offset);

		ConstSizeVector<maxNumberOfSegments> gapPerSegment;
		ConstSizeVector<maxNumberOfSegments> referenceCoordinatePerSegment;
		ConstSizeVector<maxNumberOfSegments> xDirectionGap;
		ConstSizeVector<maxNumberOfSegments> yDirectionGap;
		ComputeGap(markerData, gapPerSegment, referenceCoordinatePerSegment, xDirectionGap, yDirectionGap);
		const Index maxNumberOfPoints = maxNumberOfSegments + 1;

		//xDirectionGap.SetAll(0.);
		//yDirectionGap.SetAll(1.);
		//referenceCoordinatePerSegment.SetAll(0.5);

		ConstSizeVector<maxNumberOfPoints * 2> forcePerPoint; //force (x/y) per contact point ==> used to apply forces
		forcePerPoint.SetNumberOfItems((parameters.numberOfContactSegments + 1) * 2);
		forcePerPoint.SetAll(0.);
		Vector3D forceSum({0.,0.,0.}); //sum of all forces acting on circle (without friction, all forces can be replaced at center of circle

		for (Index i = 0; i < parameters.numberOfContactSegments; i++)
		{
			//Real hasContact = 0; //1 for contact, 0 else
			if (GetCNode(0)->GetCurrentCoordinate(i) <= 0)  //this is the contact state: 1=contact/use contact force, 0=no contact
			{
				////as gap is negative in case of contact, the force needs to act in opposite direction
				forcePerPoint[i * 2] += xDirectionGap[i] * ((1. - referenceCoordinatePerSegment[i])*gapPerSegment[i] * parameters.contactStiffness); // +gap_t * parameters.contactDamping);
				forcePerPoint[(i + 1) * 2] += xDirectionGap[i] * (referenceCoordinatePerSegment[i] * gapPerSegment[i] * parameters.contactStiffness); // +gap_t * parameters.contactDamping);
				forcePerPoint[i * 2 + 1] += yDirectionGap[i] * ((1. - referenceCoordinatePerSegment[i])*gapPerSegment[i] * parameters.contactStiffness); // +gap_t * parameters.contactDamping);
				forcePerPoint[(i + 1) * 2 + 1] += yDirectionGap[i] * (referenceCoordinatePerSegment[i] * gapPerSegment[i] * parameters.contactStiffness); // +gap_t * parameters.contactDamping);
				forceSum[0] += forcePerPoint[i * 2];
				forceSum[0] += forcePerPoint[(i + 1) * 2];
				forceSum[1] += forcePerPoint[i * 2 + 1];
				forceSum[1] += forcePerPoint[(i + 1) * 2 + 1];
			}
		}

		//if (!cSystemData->isODE2RHSjacobianComputation && forcePerPoint.GetL2Norm() != 0) {
		//	pout << "forcePerPoint=" << forcePerPoint << "\n";
		//	pout << "gapPerSegment=" << gapPerSegment << "\n";
		//	pout << "referenceCoordinatePerSegment=" << referenceCoordinatePerSegment << "\n";
		//	pout << "xDirectionGap=" << xDirectionGap << "\n";
		//	pout << "yDirectionGap=" << yDirectionGap << "\n";
		//}

		//if (forceSum.GetL2Norm() != 0) pout << "forceSum=" << forceSum << "\n";

		//now link ode2Rhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Rhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), markerData.GetMarkerData(1).jacobian.NumberOfColumns());

			//positive force on marker1
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).jacobian, forcePerPoint, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv0(ode2Rhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());

			forceSum *= -1; //negative force on marker0
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, forceSum, ldv0);
		}
	}

}

void CObjectContactCircleCable2D::ComputeJacobianODE2_ODE2(ResizableMatrix& jacobian, ResizableMatrix& jacobian_ODE2_t, const MarkerDataStructure& markerData) const
{
	CHECKandTHROWstring("ERROR: illegal call to ObjectContactCircleCable2D::ComputeODE2RHSJacobian");
}

//! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
OutputVariableType CObjectContactCircleCable2D::GetOutputVariableTypes() const
{
	return OutputVariableType::Distance;
}

//! provide according output variable in "value"
void CObjectContactCircleCable2D::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	SysError("ObjectContactCircleCable2D::GetOutputVariableConnector not implemented");
}


//! function called after Newton method; returns a residual error (force); 
//! done for two different computation states in order to estimate the correct time of contact
Real CObjectContactCircleCable2D::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, PostNewtonFlags::Type& flags)
{
	//return force-type error in case of contact: in case that the assumed contact state has been wrong, 
	//  the contact force (also negative) is returned as measure of the error
	Real discontinuousError = 0;
	flags = PostNewtonFlags::_None;
	if (parameters.activeConnector)
	{
		LinkedDataVector currentState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current);	//copy, but might change values ...

		ConstSizeVector<maxNumberOfSegments> currentGapPerSegment;
		ConstSizeVector<maxNumberOfSegments> referenceCoordinatePerSegment;
		ConstSizeVector<maxNumberOfSegments> xDirectionGap;
		ConstSizeVector<maxNumberOfSegments> yDirectionGap;
		ComputeGap(markerDataCurrent, currentGapPerSegment, referenceCoordinatePerSegment, xDirectionGap, yDirectionGap);

		for (Index i = 0; i < parameters.numberOfContactSegments; i++)
		{
			//if (currentGapPerSegment[i] > 0 && currentState[i] <= 0 || currentGapPerSegment[i] <= 0 && currentState[i] > 0) //OLD: brackets missing!
			if ((currentGapPerSegment[i] > 0 && currentState[i] <= 0) || (currentGapPerSegment[i] <= 0 && currentState[i] > 0))
			{//action: state1=currentGapState, error = |currentGap*k|
				discontinuousError += fabs((currentGapPerSegment[i] - currentState[i])* parameters.contactStiffness);
				currentState[i] = currentGapPerSegment[i];
			}
		}
	}
	return discontinuousError;
}

//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
void CObjectContactCircleCable2D::PostDiscontinuousIterationStep()
{

}

