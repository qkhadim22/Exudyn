/** ***********************************************************************************************
* @class        MainMarkerSuperElementPositionParameters
* @brief        Parameter class for MainMarkerSuperElementPosition
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:21:00 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINMARKERSUPERELEMENTPOSITIONPARAMETERS__H
#define MAINMARKERSUPERELEMENTPOSITIONPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerSuperElementPosition.h"

#include "Autogenerated/VisuMarkerSuperElementPosition.h"

//! AUTO: Parameters for class MainMarkerSuperElementPositionParameters
class MainMarkerSuperElementPositionParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerSuperElementPosition
* @brief        A position marker attached to a SuperElement, such as ObjectFFRF, ObjectGenericODE2 and ObjectFFRFreducedOrder (for which it is in its current implementation inefficient for large number of meshNodeNumbers). The marker acts on the mesh (interface) nodes, not on the underlying nodes of the object.
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

//! AUTO: MainMarkerSuperElementPosition
class MainMarkerSuperElementPosition: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerSuperElementPosition* cMarkerSuperElementPosition; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerSuperElementPosition* visualizationMarkerSuperElementPosition; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerSuperElementPosition()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerSuperElementPosition* GetCMarkerSuperElementPosition() { return cMarkerSuperElementPosition; }
    //! AUTO: Get const pointer to computational class
    const CMarkerSuperElementPosition* GetCMarkerSuperElementPosition() const { return cMarkerSuperElementPosition; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerSuperElementPosition(CMarkerSuperElementPosition* pCMarkerSuperElementPosition) { cMarkerSuperElementPosition = pCMarkerSuperElementPosition; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerSuperElementPosition* GetVisualizationMarkerSuperElementPosition() { return visualizationMarkerSuperElementPosition; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerSuperElementPosition* GetVisualizationMarkerSuperElementPosition() const { return visualizationMarkerSuperElementPosition; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerSuperElementPosition(VisualizationMarkerSuperElementPosition* pVisualizationMarkerSuperElementPosition) { visualizationMarkerSuperElementPosition = pVisualizationMarkerSuperElementPosition; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerSuperElementPosition; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerSuperElementPosition = (CMarkerSuperElementPosition*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerSuperElementPosition; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerSuperElementPosition = (VisualizationMarkerSuperElementPosition*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "SuperElementPosition";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerSuperElementPosition->GetParameters().bodyNumber = EPyUtils::GetObjectIndexSafely(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerSuperElementPosition->GetParameters().meshNodeNumbers = py::cast<std::vector<Index>>(d["meshNodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerSuperElementPosition->GetParameters().weightingFactors = py::cast<std::vector<Real>>(d["weightingFactors"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerSuperElementPosition->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VshowMarkerNodes")) { visualizationMarkerSuperElementPosition->GetShowMarkerNodes() = py::cast<bool>(d["VshowMarkerNodes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (ObjectIndex)cMarkerSuperElementPosition->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["meshNodeNumbers"] = (std::vector<Index>)cMarkerSuperElementPosition->GetParameters().meshNodeNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["weightingFactors"] = (std::vector<Real>)cMarkerSuperElementPosition->GetParameters().weightingFactors; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerSuperElementPosition->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VshowMarkerNodes"] = (bool)visualizationMarkerSuperElementPosition->GetShowMarkerNodes(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((ObjectIndex)cMarkerSuperElementPosition->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumbers") == 0) { return py::cast((std::vector<Index>)cMarkerSuperElementPosition->GetParameters().meshNodeNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("weightingFactors") == 0) { return py::cast((std::vector<Real>)cMarkerSuperElementPosition->GetParameters().weightingFactors);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerSuperElementPosition->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VshowMarkerNodes") == 0) { return py::cast((bool)visualizationMarkerSuperElementPosition->GetShowMarkerNodes());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerSuperElementPosition::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cMarkerSuperElementPosition->GetParameters().bodyNumber = EPyUtils::GetObjectIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumbers") == 0) { cMarkerSuperElementPosition->GetParameters().meshNodeNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("weightingFactors") == 0) { cMarkerSuperElementPosition->GetParameters().weightingFactors = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerSuperElementPosition->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VshowMarkerNodes") == 0) { visualizationMarkerSuperElementPosition->GetShowMarkerNodes() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerSuperElementPosition::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
