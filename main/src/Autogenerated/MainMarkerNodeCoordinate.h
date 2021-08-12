/** ***********************************************************************************************
* @class        MainMarkerNodeCoordinateParameters
* @brief        Parameter class for MainMarkerNodeCoordinate
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:20:59 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINMARKERNODECOORDINATEPARAMETERS__H
#define MAINMARKERNODECOORDINATEPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerNodeCoordinate.h"

#include "Autogenerated/VisuMarkerNodeCoordinate.h"

//! AUTO: Parameters for class MainMarkerNodeCoordinateParameters
class MainMarkerNodeCoordinateParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerNodeCoordinate
* @brief        A node-Marker attached to a \hac{ODE2} coordinate of a node; this marker allows to connect a coordinate-based constraint or connector to a nodal coordinate (also NodeGround); for \hac{ODE1} coordinates use MarkerNodeODE1Coordinate.
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

//! AUTO: MainMarkerNodeCoordinate
class MainMarkerNodeCoordinate: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerNodeCoordinate* cMarkerNodeCoordinate; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerNodeCoordinate* visualizationMarkerNodeCoordinate; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerNodeCoordinate()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerNodeCoordinate* GetCMarkerNodeCoordinate() { return cMarkerNodeCoordinate; }
    //! AUTO: Get const pointer to computational class
    const CMarkerNodeCoordinate* GetCMarkerNodeCoordinate() const { return cMarkerNodeCoordinate; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerNodeCoordinate(CMarkerNodeCoordinate* pCMarkerNodeCoordinate) { cMarkerNodeCoordinate = pCMarkerNodeCoordinate; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerNodeCoordinate* GetVisualizationMarkerNodeCoordinate() { return visualizationMarkerNodeCoordinate; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerNodeCoordinate* GetVisualizationMarkerNodeCoordinate() const { return visualizationMarkerNodeCoordinate; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerNodeCoordinate(VisualizationMarkerNodeCoordinate* pVisualizationMarkerNodeCoordinate) { visualizationMarkerNodeCoordinate = pVisualizationMarkerNodeCoordinate; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerNodeCoordinate; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerNodeCoordinate = (CMarkerNodeCoordinate*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerNodeCoordinate; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerNodeCoordinate = (VisualizationMarkerNodeCoordinate*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "NodeCoordinate";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerNodeCoordinate->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerNodeCoordinate->GetParameters().coordinate = py::cast<Index>(d["coordinate"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerNodeCoordinate->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["nodeNumber"] = (NodeIndex)cMarkerNodeCoordinate->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["coordinate"] = (Index)cMarkerNodeCoordinate->GetParameters().coordinate; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerNodeCoordinate->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((NodeIndex)cMarkerNodeCoordinate->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("coordinate") == 0) { return py::cast((Index)cMarkerNodeCoordinate->GetParameters().coordinate);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerNodeCoordinate->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerNodeCoordinate::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cMarkerNodeCoordinate->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("coordinate") == 0) { cMarkerNodeCoordinate->GetParameters().coordinate = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerNodeCoordinate->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerNodeCoordinate::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
