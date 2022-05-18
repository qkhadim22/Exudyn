/** ***********************************************************************************************
* @class        MainNodePoint3DSlope23Parameters
* @brief        Parameter class for MainNodePoint3DSlope23
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-05-16  11:24:34 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINNODEPOINT3DSLOPE23PARAMETERS__H
#define MAINNODEPOINT3DSLOPE23PARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CNodePoint3DSlope23.h"

#include "Autogenerated/VisuNodePoint3DSlope23.h"

//! AUTO: Parameters for class MainNodePoint3DSlope23Parameters
class MainNodePoint3DSlope23Parameters // AUTO: 
{
public: // AUTO: 
    Vector9D initialCoordinates;                  //!< AUTO: initial displacement coordinates relative to reference coordinates
    Vector9D initialCoordinates_t;                //!< AUTO: initial velocity coordinates
    //! AUTO: default constructor with parameter initialization
    MainNodePoint3DSlope23Parameters()
    {
        initialCoordinates = Vector9D({0.,0.,0.,0.,0.,0.,0.,0.,0.});
        initialCoordinates_t = Vector9D({0.,0.,0.,0.,0.,0.,0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodePoint3DSlope23
* @brief        A 3D point/slope vector node for spatial, shear and cross-section deformable ANCF (absolute nodal coordinate formulation) beam elements; the node has 9 ODE2 degrees of freedom (3 for displacement of point node and 2 \f$\times\f$ 3 for the slope vectors 'slopey' and 'slopez'); all coordinates lead to second order differential equations; the slope vector defines the directional derivative w.r.t the local axial (x) coordinate, denoted as \f$()^\prime\f$; in straight configuration aligned at the global x-axis, the slopey vector reads \f$\rv_y^\prime=[0\;\;1\;\;0]^T\f$ and slopez gets \f$\rv_z^\prime=[0\;\;0\;\;1]^T\f$.
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

//! AUTO: MainNodePoint3DSlope23
class MainNodePoint3DSlope23: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodePoint3DSlope23* cNodePoint3DSlope23; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodePoint3DSlope23* visualizationNodePoint3DSlope23; //pointer to computational object (initialized in object factory) AUTO:
    MainNodePoint3DSlope23Parameters parameters; //! AUTO: contains all parameters for MainNodePoint3DSlope23

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodePoint3DSlope23()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodePoint3DSlope23* GetCNodePoint3DSlope23() { return cNodePoint3DSlope23; }
    //! AUTO: Get const pointer to computational class
    const CNodePoint3DSlope23* GetCNodePoint3DSlope23() const { return cNodePoint3DSlope23; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodePoint3DSlope23(CNodePoint3DSlope23* pCNodePoint3DSlope23) { cNodePoint3DSlope23 = pCNodePoint3DSlope23; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodePoint3DSlope23* GetVisualizationNodePoint3DSlope23() { return visualizationNodePoint3DSlope23; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodePoint3DSlope23* GetVisualizationNodePoint3DSlope23() const { return visualizationNodePoint3DSlope23; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodePoint3DSlope23(VisualizationNodePoint3DSlope23* pVisualizationNodePoint3DSlope23) { visualizationNodePoint3DSlope23 = pVisualizationNodePoint3DSlope23; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodePoint3DSlope23; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodePoint3DSlope23 = (CNodePoint3DSlope23*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodePoint3DSlope23; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodePoint3DSlope23 = (VisualizationNodePoint3DSlope23*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodePoint3DSlope23Parameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodePoint3DSlope23Parameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword 'Node'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Point3DSlope23";
    }

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
        EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(d, "referenceCoordinates", cNodePoint3DSlope23->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodePoint3DSlope23->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodePoint3DSlope23->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodePoint3DSlope23->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodePoint3DSlope23->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodePoint3DSlope23->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodePoint3DSlope23->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodePoint3DSlope23->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodePoint3DSlope23->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodePoint3DSlope23->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodePoint3DSlope23->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodePoint3DSlope23->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint3DSlope23::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(value, cNodePoint3DSlope23->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 9>(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodePoint3DSlope23->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodePoint3DSlope23->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodePoint3DSlope23->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint3DSlope23::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
