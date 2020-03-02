/** ***********************************************************************************************
* @class        MainNodePoint2DSlope1Parameters
* @brief        Parameter class for MainNodePoint2DSlope1
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-02-21  22:45:24 (last modfied)
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
#include "Autogenerated/CNodePoint2DSlope1.h"

#include "Autogenerated/VisuNodePoint2DSlope1.h"

//! AUTO: Parameters for class MainNodePoint2DSlope1Parameters
class MainNodePoint2DSlope1Parameters // AUTO: 
{
public: // AUTO: 
    Vector4D initialCoordinates;                  //!< AUTO: initial displacement coordinates: ux, uy and x/y "displacements" of slopex
    Vector4D initialCoordinates_t;                //!< AUTO: initial velocity coordinates
    //! AUTO: default constructor with parameter initialization
    MainNodePoint2DSlope1Parameters()
    {
        initialCoordinates = Vector4D({0.,0.,0.,0.});
        initialCoordinates_t = Vector4D({0.,0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodePoint2DSlope1
* @brief        A 2D point/slope vector node for planar Bernoulli-Euler ANCF (absolute nodal coordinate formulation) beam elements; the node has 4 displacement degrees of freedom (2 for displacement of point node and 2 for the slope vector 'slopex'); all coordinates lead to second order differential equations; the slope vector defines the directional derivative w.r.t the local axial (x) coordinate, denoted as \f$()^\prime\f$; in straight configuration aligned at the global x-axis, the slope vector reads \f$\rv^\prime=[r_x^\prime\;\;r_y^\prime]^T=[1\;\;0]^T\f$.
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

//! AUTO: MainNodePoint2DSlope1
class MainNodePoint2DSlope1: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodePoint2DSlope1* cNodePoint2DSlope1; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodePoint2DSlope1* visualizationNodePoint2DSlope1; //pointer to computational object (initialized in object factory) AUTO:
    MainNodePoint2DSlope1Parameters parameters; //! AUTO: contains all parameters for MainNodePoint2DSlope1

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodePoint2DSlope1()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodePoint2DSlope1* GetCNodePoint2DSlope1() { return cNodePoint2DSlope1; }
    //! AUTO: Get const pointer to computational class
    const CNodePoint2DSlope1* GetCNodePoint2DSlope1() const { return cNodePoint2DSlope1; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodePoint2DSlope1(CNodePoint2DSlope1* pCNodePoint2DSlope1) { cNodePoint2DSlope1 = pCNodePoint2DSlope1; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodePoint2DSlope1* GetVisualizationNodePoint2DSlope1() { return visualizationNodePoint2DSlope1; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodePoint2DSlope1* GetVisualizationNodePoint2DSlope1() const { return visualizationNodePoint2DSlope1; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodePoint2DSlope1(VisualizationNodePoint2DSlope1* pVisualizationNodePoint2DSlope1) { visualizationNodePoint2DSlope1 = pVisualizationNodePoint2DSlope1; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodePoint2DSlope1; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodePoint2DSlope1 = (CNodePoint2DSlope1*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodePoint2DSlope1; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodePoint2DSlope1 = (VisualizationNodePoint2DSlope1*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodePoint2DSlope1Parameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodePoint2DSlope1Parameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword "Node"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Point2DSlope1";
    }

    //! AUTO:  Call a specific node function ==> automatically generated in future
    virtual py::object CallFunction(STDstring functionName, py::dict args) const override;

    //! AUTO:  return internally stored initial coordinates (displacements) of node
    virtual LinkedDataVector GetInitialVector() const override
    {
        return parameters.initialCoordinates;
    }

    //! AUTO:  return internally stored initial coordinates (velocities) of node
    virtual LinkedDataVector GetInitialVector_t() const override
    {
        return parameters.initialCoordinates_t;
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        EPyUtils::SetVector4DSafely(d, "referenceCoordinates", cNodePoint2DSlope1->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetVector4DSafely(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetVector4DSafely(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodePoint2DSlope1->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodePoint2DSlope1->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodePoint2DSlope1->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodePoint2DSlope1->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodePoint2DSlope1->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodePoint2DSlope1->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodePoint2DSlope1->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodePoint2DSlope1->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodePoint2DSlope1->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodePoint2DSlope1->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodePoint2DSlope1->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint2DSlope1::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetVector4DSafely(value, cNodePoint2DSlope1->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetVector4DSafely(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetVector4DSafely(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodePoint2DSlope1->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodePoint2DSlope1->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodePoint2DSlope1->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint2DSlope1::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


