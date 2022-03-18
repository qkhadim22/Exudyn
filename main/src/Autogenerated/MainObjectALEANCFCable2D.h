/** ***********************************************************************************************
* @class        MainObjectALEANCFCable2DParameters
* @brief        Parameter class for MainObjectALEANCFCable2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-03-10  20:04:24 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTALEANCFCABLE2DPARAMETERS__H
#define MAINOBJECTALEANCFCABLE2DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectALEANCFCable2D.h"

#include "Autogenerated/VisuObjectALEANCFCable2D.h"

//! AUTO: Parameters for class MainObjectALEANCFCable2DParameters
class MainObjectALEANCFCable2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectALEANCFCable2D
* @brief        A 2D cable finite element using 2 nodes of type NodePoint2DSlope1 and a axially moving coordinate of type NodeGenericODE2, which adds additional (redundant) motion in axial direction of the beam. This allows modeling pipes but also axially moving beams. The localPosition of the beam with length \f$L\f$=physicsLength and height \f$h\f$ ranges in \f$X\f$-direction in range \f$[0, L]\f$ and in \f$Y\f$-direction in range \f$[-h/2,h/2]\f$ (which is in fact not needed in the \hac{EOM}).
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

//! AUTO: MainObjectALEANCFCable2D
class MainObjectALEANCFCable2D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectALEANCFCable2D* cObjectALEANCFCable2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectALEANCFCable2D* visualizationObjectALEANCFCable2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectALEANCFCable2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectALEANCFCable2D* GetCObjectALEANCFCable2D() { return cObjectALEANCFCable2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectALEANCFCable2D* GetCObjectALEANCFCable2D() const { return cObjectALEANCFCable2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectALEANCFCable2D(CObjectALEANCFCable2D* pCObjectALEANCFCable2D) { cObjectALEANCFCable2D = pCObjectALEANCFCable2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectALEANCFCable2D* GetVisualizationObjectALEANCFCable2D() { return visualizationObjectALEANCFCable2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectALEANCFCable2D* GetVisualizationObjectALEANCFCable2D() const { return visualizationObjectALEANCFCable2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectALEANCFCable2D(VisualizationObjectALEANCFCable2D* pVisualizationObjectALEANCFCable2D) { visualizationObjectALEANCFCable2D = pVisualizationObjectALEANCFCable2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectALEANCFCable2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectALEANCFCable2D = (CObjectALEANCFCable2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectALEANCFCable2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectALEANCFCable2D = (VisualizationObjectALEANCFCable2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ALEANCFCable2D";
    }

    //! AUTO:  node types are checked in CheckPreAssembleConsistency(...);provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::_None;
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectALEANCFCable2D->GetParameters().physicsLength = py::cast<Real>(d["physicsLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsMassPerLength = py::cast<Real>(d["physicsMassPerLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsMovingMassFactor = py::cast<Real>(d["physicsMovingMassFactor"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsBendingStiffness = py::cast<Real>(d["physicsBendingStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsAxialStiffness = py::cast<Real>(d["physicsAxialStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsBendingDamping = py::cast<Real>(d["physicsBendingDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsAxialDamping = py::cast<Real>(d["physicsAxialDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsReferenceAxialStrain = py::cast<Real>(d["physicsReferenceAxialStrain"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsReferenceCurvature = py::cast<Real>(d["physicsReferenceCurvature"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().physicsUseCouplingTerms = py::cast<bool>(d["physicsUseCouplingTerms"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex3Safely(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().useReducedOrderIntegration = py::cast<Index>(d["useReducedOrderIntegration"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectALEANCFCable2D->GetParameters().strainIsRelativeToReference = py::cast<Real>(d["strainIsRelativeToReference"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectALEANCFCable2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawHeight")) { visualizationObjectALEANCFCable2D->GetDrawHeight() = py::cast<float>(d["VdrawHeight"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectALEANCFCable2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsLength"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMassPerLength"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsMassPerLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMovingMassFactor"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsMovingMassFactor; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsBendingStiffness"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsBendingStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsAxialStiffness"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsAxialStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsBendingDamping"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsBendingDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsAxialDamping"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsAxialDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsReferenceAxialStrain"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsReferenceAxialStrain; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsReferenceCurvature"] = (Real)cObjectALEANCFCable2D->GetParameters().physicsReferenceCurvature; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsUseCouplingTerms"] = (bool)cObjectALEANCFCable2D->GetParameters().physicsUseCouplingTerms; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumbers"] = EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectALEANCFCable2D->GetParameters().nodeNumbers)); //! AUTO: cast variables into python (not needed for standard types) 
        d["useReducedOrderIntegration"] = (Index)cObjectALEANCFCable2D->GetParameters().useReducedOrderIntegration; //! AUTO: cast variables into python (not needed for standard types) 
        d["strainIsRelativeToReference"] = (Real)cObjectALEANCFCable2D->GetParameters().strainIsRelativeToReference; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectALEANCFCable2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawHeight"] = (float)visualizationObjectALEANCFCable2D->GetDrawHeight(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectALEANCFCable2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsMassPerLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMovingMassFactor") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsMovingMassFactor);} //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsBendingStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsAxialStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingDamping") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsBendingDamping);} //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialDamping") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsAxialDamping);} //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceAxialStrain") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsReferenceAxialStrain);} //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceCurvature") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().physicsReferenceCurvature);} //! AUTO: get parameter
        else if (parameterName.compare("physicsUseCouplingTerms") == 0) { return py::cast((bool)cObjectALEANCFCable2D->GetParameters().physicsUseCouplingTerms);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast(EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectALEANCFCable2D->GetParameters().nodeNumbers)));} //! AUTO: get parameter
        else if (parameterName.compare("useReducedOrderIntegration") == 0) { return py::cast((Index)cObjectALEANCFCable2D->GetParameters().useReducedOrderIntegration);} //! AUTO: get parameter
        else if (parameterName.compare("strainIsRelativeToReference") == 0) { return py::cast((Real)cObjectALEANCFCable2D->GetParameters().strainIsRelativeToReference);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectALEANCFCable2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { return py::cast((float)visualizationObjectALEANCFCable2D->GetDrawHeight());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectALEANCFCable2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectALEANCFCable2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { cObjectALEANCFCable2D->GetParameters().physicsLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { cObjectALEANCFCable2D->GetParameters().physicsMassPerLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMovingMassFactor") == 0) { cObjectALEANCFCable2D->GetParameters().physicsMovingMassFactor = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { cObjectALEANCFCable2D->GetParameters().physicsBendingStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { cObjectALEANCFCable2D->GetParameters().physicsAxialStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingDamping") == 0) { cObjectALEANCFCable2D->GetParameters().physicsBendingDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialDamping") == 0) { cObjectALEANCFCable2D->GetParameters().physicsAxialDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceAxialStrain") == 0) { cObjectALEANCFCable2D->GetParameters().physicsReferenceAxialStrain = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceCurvature") == 0) { cObjectALEANCFCable2D->GetParameters().physicsReferenceCurvature = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsUseCouplingTerms") == 0) { cObjectALEANCFCable2D->GetParameters().physicsUseCouplingTerms = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectALEANCFCable2D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex3Safely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("useReducedOrderIntegration") == 0) { cObjectALEANCFCable2D->GetParameters().useReducedOrderIntegration = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("strainIsRelativeToReference") == 0) { cObjectALEANCFCable2D->GetParameters().strainIsRelativeToReference = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectALEANCFCable2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { visualizationObjectALEANCFCable2D->GetDrawHeight() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectALEANCFCable2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectALEANCFCable2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
