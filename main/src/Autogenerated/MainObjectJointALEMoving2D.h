/** ***********************************************************************************************
* @class        MainObjectJointALEMoving2DParameters
* @brief        Parameter class for MainObjectJointALEMoving2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-02-26  13:43:56 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectJointALEMoving2D.h"

#include "Autogenerated/VisuObjectJointALEMoving2D.h"

//! AUTO: Parameters for class MainObjectJointALEMoving2DParameters
class MainObjectJointALEMoving2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectJointALEMoving2D
* @brief        A specialized axially moving joint (without rotation) in 2D between a ALE Cable2D (marker1) and a position-based marker (marker0); ALE=Arbitrary Lagrangian Eulerian; the data coordinate x[0] provides the current index in slidingMarkerNumbers, and the ODE2 coordinate q[0] provides the (given) moving coordinate in the cable element.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainObjectJointALEMoving2D
class MainObjectJointALEMoving2D: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectJointALEMoving2D* cObjectJointALEMoving2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectJointALEMoving2D* visualizationObjectJointALEMoving2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectJointALEMoving2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectJointALEMoving2D* GetCObjectJointALEMoving2D() { return cObjectJointALEMoving2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectJointALEMoving2D* GetCObjectJointALEMoving2D() const { return cObjectJointALEMoving2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectJointALEMoving2D(CObjectJointALEMoving2D* pCObjectJointALEMoving2D) { cObjectJointALEMoving2D = pCObjectJointALEMoving2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectJointALEMoving2D* GetVisualizationObjectJointALEMoving2D() { return visualizationObjectJointALEMoving2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectJointALEMoving2D* GetVisualizationObjectJointALEMoving2D() const { return visualizationObjectJointALEMoving2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectJointALEMoving2D(VisualizationObjectJointALEMoving2D* pVisualizationObjectJointALEMoving2D) { visualizationObjectJointALEMoving2D = pVisualizationObjectJointALEMoving2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectJointALEMoving2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectJointALEMoving2D = (CObjectJointALEMoving2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectJointALEMoving2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectJointALEMoving2D = (VisualizationObjectJointALEMoving2D*)pVisualizationObject; }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;

    //! AUTO:  must be checked in CheckPreAssembleConsistency(...); provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::_None;
    }

    //! AUTO:  Get type name of object (without keyword "Object"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "JointALEMoving2D";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectJointALEMoving2D->GetParameters().markerNumbers = py::cast<std::vector<Index>>(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectJointALEMoving2D->GetParameters().slidingMarkerNumbers = py::cast<std::vector<Index>>(d["slidingMarkerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectJointALEMoving2D->GetParameters().slidingMarkerOffsets = py::cast<std::vector<Real>>(d["slidingMarkerOffsets"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectJointALEMoving2D->GetParameters().slidingOffset = py::cast<Real>(d["slidingOffset"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectJointALEMoving2D->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "usePenaltyFormulation")) { cObjectJointALEMoving2D->GetParameters().usePenaltyFormulation = py::cast<bool>(d["usePenaltyFormulation"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        cObjectJointALEMoving2D->GetParameters().penaltyStiffness = py::cast<Real>(d["penaltyStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectJointALEMoving2D->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectJointALEMoving2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectJointALEMoving2D->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectJointALEMoving2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = (std::vector<Index>)cObjectJointALEMoving2D->GetParameters().markerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["slidingMarkerNumbers"] = (std::vector<Index>)cObjectJointALEMoving2D->GetParameters().slidingMarkerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["slidingMarkerOffsets"] = (std::vector<Real>)cObjectJointALEMoving2D->GetParameters().slidingMarkerOffsets; //! AUTO: cast variables into python (not needed for standard types) 
        d["slidingOffset"] = (Real)cObjectJointALEMoving2D->GetParameters().slidingOffset; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumbers"] = (std::vector<Index>)cObjectJointALEMoving2D->GetParameters().nodeNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["usePenaltyFormulation"] = (bool)cObjectJointALEMoving2D->GetParameters().usePenaltyFormulation; //! AUTO: cast variables into python (not needed for standard types) 
        d["penaltyStiffness"] = (Real)cObjectJointALEMoving2D->GetParameters().penaltyStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectJointALEMoving2D->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectJointALEMoving2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectJointALEMoving2D->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectJointALEMoving2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectJointALEMoving2D->GetParameters().markerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("slidingMarkerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectJointALEMoving2D->GetParameters().slidingMarkerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("slidingMarkerOffsets") == 0) { return py::cast((std::vector<Real>)cObjectJointALEMoving2D->GetParameters().slidingMarkerOffsets);} //! AUTO: get parameter
        else if (parameterName.compare("slidingOffset") == 0) { return py::cast((Real)cObjectJointALEMoving2D->GetParameters().slidingOffset);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast((std::vector<Index>)cObjectJointALEMoving2D->GetParameters().nodeNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("usePenaltyFormulation") == 0) { return py::cast((bool)cObjectJointALEMoving2D->GetParameters().usePenaltyFormulation);} //! AUTO: get parameter
        else if (parameterName.compare("penaltyStiffness") == 0) { return py::cast((Real)cObjectJointALEMoving2D->GetParameters().penaltyStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectJointALEMoving2D->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectJointALEMoving2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectJointALEMoving2D->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectJointALEMoving2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointALEMoving2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectJointALEMoving2D->GetParameters().markerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("slidingMarkerNumbers") == 0) { cObjectJointALEMoving2D->GetParameters().slidingMarkerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("slidingMarkerOffsets") == 0) { cObjectJointALEMoving2D->GetParameters().slidingMarkerOffsets = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("slidingOffset") == 0) { cObjectJointALEMoving2D->GetParameters().slidingOffset = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectJointALEMoving2D->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("usePenaltyFormulation") == 0) { cObjectJointALEMoving2D->GetParameters().usePenaltyFormulation = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("penaltyStiffness") == 0) { cObjectJointALEMoving2D->GetParameters().penaltyStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectJointALEMoving2D->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectJointALEMoving2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectJointALEMoving2D->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectJointALEMoving2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointALEMoving2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


