/** ***********************************************************************************************
* @brief        implementation for MarkerSuperElementRigid
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
#include "Utilities/RigidBodyMath.h"

//#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
//#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
//namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerSuperElementRigid.h"

//! return parameters of underlying floating frame node (or default values for case that no frame exists)
void CMarkerSuperElementRigid::GetFloatingFrameNodeData(const CSystemData& cSystemData, Vector3D& framePosition, Matrix3D& frameRotationMatrix,
							  Vector3D& frameVelocity, Vector3D& frameAngularVelocityLocal, ConfigurationType configuration) const
{
	const CObjectSuperElement& cObjectSuperElement = (const CObjectSuperElement&)(*cSystemData.GetCObjects()[parameters.bodyNumber]); 
	Index localRigidBodyNodeNumber; //node number in superelement!
	if (cObjectSuperElement.HasReferenceFrame(localRigidBodyNodeNumber))
	{
		const CNodeRigidBody& cNodeRigid = ((const CNodeRigidBody&)cSystemData.GetCNode(cObjectSuperElement.GetNodeNumber(localRigidBodyNodeNumber)));// ->GetPosition(configuration)

		framePosition = cNodeRigid.GetPosition(configuration);
		frameRotationMatrix = cNodeRigid.GetRotationMatrix(configuration);
		frameVelocity = cNodeRigid.GetVelocity(configuration);
		frameAngularVelocityLocal = cNodeRigid.GetAngularVelocityLocal(configuration);
	}
	else
	{
		framePosition.SetAll(0);
		frameRotationMatrix.SetScalarMatrix(3, 1.); //unit matrix
		frameVelocity.SetAll(0);
		frameAngularVelocityLocal.SetAll(0);
	}
}

//! return weighted (linearized) rotation from local mesh displacements
void CMarkerSuperElementRigid::GetWeightedRotations(const CSystemData& cSystemData, Vector3D& weightedRotations, ConfigurationType configuration) const
{
	const ArrayIndex& nodeNumbers = parameters.meshNodeNumbers;
	const CObjectSuperElement& cObject = (const CObjectSuperElement&)(cSystemData.GetCObjectBody(GetObjectNumber())); //always possible

																													  //compute rotations according to given formula
	Real factor = 0; //sum w_i * |pRef_i|^2
	Matrix3D factorMatrix(3, 3, 0.); //W in docu

	weightedRotations.SetAll(0); //sum w_i * pRef_i x u_i
	Vector3D pRef; //mesh node local reference position

	for (Index i = 0; i < nodeNumbers.NumberOfItems(); i++)
	{
		pRef = cObject.GetMeshNodeLocalPosition(nodeNumbers[i], ConfigurationType::Reference);
		if (useRefPosWeightedRot) { pRef -= parameters.referencePosition; }

		if (useAlternativeApproach)
		{
			factorMatrix -= parameters.weightingFactors[i] * RigidBodyMath::Vector2SkewMatrix(pRef) * RigidBodyMath::Vector2SkewMatrix(pRef); //negative sign!
		}
		else
		{
			factor += parameters.weightingFactors[i] * pRef.GetL2NormSquared();
		}

		weightedRotations += parameters.weightingFactors[i] * pRef.CrossProduct(cObject.GetMeshNodeLocalPosition(nodeNumbers[i], configuration) -
			cObject.GetMeshNodeLocalPosition(nodeNumbers[i], ConfigurationType::Reference));
	}
	//already checked: CHECKandTHROW(factor != 0., "MarkerSuperElementRigid::GetWeightedRotations obtained singular mesh node weighting matrix ==> check your interface nodes");

	if (useAlternativeApproach)
	{
		weightedRotations = factorMatrix.GetInverse()*weightedRotations;
		//weightedRotations *= -1.;
	}
	else
	{
		weightedRotations /= factor;
	}
}

//! return weighted angular velocity from local mesh velocities
void CMarkerSuperElementRigid::GetWeightedAngularVelocity(const CSystemData& cSystemData, Vector3D& weightedAngularVelocity, ConfigurationType configuration) const
{
	const ArrayIndex& nodeNumbers = parameters.meshNodeNumbers;
	const CObjectSuperElement& cObject = (const CObjectSuperElement&)(cSystemData.GetCObjectBody(GetObjectNumber())); //always possible

																													  //compute rotations according to given formula
	Real factor = 0; //sum w_i * |pRef_i|^2
	weightedAngularVelocity.SetAll(0); //sum w_i * pRef_i x u_i
	Matrix3D factorMatrix(3, 3, 0.);   //W in docu
	Vector3D pRef; //mesh node local reference position

	for (Index i = 0; i < nodeNumbers.NumberOfItems(); i++)
	{
		pRef = cObject.GetMeshNodeLocalPosition(nodeNumbers[i], ConfigurationType::Reference);
		if (useRefPosWeightedRot) { pRef -= parameters.referencePosition; }
		if (useAlternativeApproach)
		{
			factorMatrix -= parameters.weightingFactors[i] * RigidBodyMath::Vector2SkewMatrix(pRef) * RigidBodyMath::Vector2SkewMatrix(pRef); //negative sign!
		}
		else
		{
			factor += parameters.weightingFactors[i] * pRef.GetL2NormSquared();
		}
		weightedAngularVelocity += parameters.weightingFactors[i] * pRef.CrossProduct(cObject.GetMeshNodeLocalVelocity(nodeNumbers[i], configuration));
	}
	//already checked: CHECKandTHROW(factor != 0., "MarkerSuperElementRigid::GetWeightedAngularVelocity obtained singular mesh node weighting matrix ==> check your interface nodes");

	if (useAlternativeApproach)
	{
		weightedAngularVelocity = factorMatrix.GetInverse()*weightedAngularVelocity;
	}
	else
	{
		weightedAngularVelocity /= factor;
	}
}


void CMarkerSuperElementRigid::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	const ArrayIndex& nodeNumbers = parameters.meshNodeNumbers;
	const CObjectSuperElement& cObject = (const CObjectSuperElement&)(cSystemData.GetCObjectBody(GetObjectNumber())); //always possible

	Vector3D framePosition;
	Matrix3D frameRotationMatrix;
	Vector3D frameVelocity;
	Vector3D frameAngularVelocityLocal;
	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal, configuration);

	//compute local position:
	position = parameters.referencePosition;
	for (Index i = 0; i < nodeNumbers.NumberOfItems(); i++)
	{
		//add weighted displacements: could be optimized if SuperElement has additional GetMeshNodeLocalDisplacement(...)
		position += parameters.weightingFactors[i] * (cObject.GetMeshNodeLocalPosition(nodeNumbers[i], configuration) -
														   cObject.GetMeshNodeLocalPosition(nodeNumbers[i], ConfigurationType::Reference));
	}

	//transform to global position:
	position = framePosition + frameRotationMatrix * position;
}

void CMarkerSuperElementRigid::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
{
	const ArrayIndex& nodeNumbers = parameters.meshNodeNumbers;
	const CObjectSuperElement& cObject = (const CObjectSuperElement&)(cSystemData.GetCObjectBody(GetObjectNumber())); //always possible

	Vector3D framePosition;
	Matrix3D frameRotationMatrix;
	Vector3D frameVelocity;
	Vector3D frameAngularVelocityLocal;
	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal, configuration);

	Vector3D localDisplacement({ 0,0,0 });
	velocity.SetAll(0);
	for (Index i = 0; i < nodeNumbers.NumberOfItems(); i++)
	{
		velocity += parameters.weightingFactors[i] * cObject.GetMeshNodeVelocity(nodeNumbers[i], configuration);
		
		//add weighted displacements: 
		localDisplacement += parameters.weightingFactors[i] * (cObject.GetMeshNodeLocalPosition(nodeNumbers[i], configuration) -
			cObject.GetMeshNodeLocalPosition(nodeNumbers[i], ConfigurationType::Reference));
	}

	velocity += frameAngularVelocityLocal.CrossProduct(parameters.referencePosition + localDisplacement);
	velocity = frameVelocity + frameRotationMatrix * velocity;
}


void CMarkerSuperElementRigid::GetRotationMatrix(const CSystemData& cSystemData, Matrix3D& rotationMatrix, ConfigurationType configuration) const
{
	Vector3D framePosition;
	Matrix3D frameRotationMatrix;
	Vector3D frameVelocity;
	Vector3D frameAngularVelocityLocal;
	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal, configuration);

	Vector3D weightedRotations;
	GetWeightedRotations(cSystemData, weightedRotations, configuration);
	rotationMatrix = frameRotationMatrix * (EXUmath::unitMatrix3D + RigidBodyMath::Vector2SkewMatrix(weightedRotations)); //linearized rotation matrix reads: I+skew(rotVec)
}

void CMarkerSuperElementRigid::GetAngularVelocity(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	Vector3D framePosition;
	Matrix3D frameRotationMatrix;
	Vector3D frameVelocity;
	Vector3D frameAngularVelocityLocal;
	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal, configuration);

	Vector3D weightedAngularVelocity;
	GetWeightedAngularVelocity(cSystemData, weightedAngularVelocity, configuration);
	angularVelocity = frameRotationMatrix * (frameAngularVelocityLocal + weightedAngularVelocity);
}

void CMarkerSuperElementRigid::GetAngularVelocityLocal(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	Vector3D framePosition;
	Matrix3D frameRotationMatrix;
	Vector3D frameVelocity;
	Vector3D frameAngularVelocityLocal;
	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal, configuration);

	Vector3D weightedAngularVelocity;
	GetWeightedAngularVelocity(cSystemData, weightedAngularVelocity, configuration);
	angularVelocity = frameAngularVelocityLocal + weightedAngularVelocity;
}




void CMarkerSuperElementRigid::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
{
	GetPosition(cSystemData, markerData.position, ConfigurationType::Current);
	GetVelocity(cSystemData, markerData.velocity, ConfigurationType::Current);

	GetRotationMatrix(cSystemData, markerData.orientation, ConfigurationType::Current);
	GetAngularVelocityLocal(cSystemData, markerData.angularVelocityLocal, ConfigurationType::Current);
	markerData.velocityAvailable = true;

//#define verboseCMarkerSuperElementRigid
#ifdef verboseCMarkerSuperElementRigid
	pout << "markerdata:\n";
	pout << "  markerdata.position=" << markerData.position << "\n";
	pout << "  markerdata.velocity=" << markerData.velocity << "\n";
	pout << "  markerdata.orientation=" << markerData.orientation << "\n";
	pout << "  markerdata.angularVelocityLocal=" << markerData.angularVelocityLocal << "\n";
#endif // verboseCMarkerSuperElementRigid
	//if (!computeJacobian)
	//{
	//	//pout << "markerData.orientation.XYZ=" << RigidBodyMath::RotationMatrix2RotXYZ(markerData.orientation) << "\n";

	//	//Vector3D weightedRotations;
	//	//GetWeightedRotations(cSystemData, weightedRotations);
	//	//pout << "  GetWeightedRotations=" << weightedRotations << "\n";
	//	Vector3D framePosition;
	//	Matrix3D frameRotationMatrix;
	//	Vector3D frameVelocity;
	//	Vector3D frameAngularVelocityLocal;
	//	GetFloatingFrameNodeData(cSystemData, framePosition, frameRotationMatrix, frameVelocity, frameAngularVelocityLocal);
	//	pout << "  frameRotationMatrix=" << RigidBodyMath::RotationMatrix2RotXYZ(frameRotationMatrix) << "\n";
	//	pout << "  angularVelocityLocal=" << markerData.angularVelocityLocal << "\n";
	//	
	//	pout << "  framePosition=" << framePosition << "\n";
	//}


	if (computeJacobian)
	{
		//const ArrayIndex& nodeNumbers = parameters.meshNodeNumbers;
		const CObjectSuperElement& cObject = (const CObjectSuperElement&)(cSystemData.GetCObjectBody(GetObjectNumber())); //always possible

		//will be done in AccessFunctions ...
		//markerData.positionJacobian.SetNumberOfRowsAndColumns(3, cObject.GetODE2Size());
		//markerData.positionJacobian.SetAll(0.);

		Index nw = parameters.weightingFactors.NumberOfItems();

		LinkedDataMatrix weightingMatrix(parameters.weightingFactors.GetDataPointer(), nw, 1);

		cObject.GetAccessFunctionSuperElement((AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::SuperElement),
			weightingMatrix, parameters.meshNodeNumbers, markerData.positionJacobian);

		cObject.GetAccessFunctionSuperElement((AccessFunctionType)((Index)AccessFunctionType::AngularVelocity_qt + (Index)AccessFunctionType::SuperElement),
			weightingMatrix, parameters.meshNodeNumbers, markerData.rotationJacobian);

#ifdef verboseCMarkerSuperElementRigid
		pout << "  markerdata.positionJacobian=" << markerData.positionJacobian << "\n";
		pout << "  markerdata.rotationJacobian=" << markerData.rotationJacobian << "\n";
#endif // verboseCMarkerSuperElementRigid
	}
}

