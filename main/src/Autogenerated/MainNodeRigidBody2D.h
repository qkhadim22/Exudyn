/** ***********************************************************************************************
* @class        MainNodeRigidBody2DParameters
* @brief        Parameter class for MainNodeRigidBody2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-07-21  19:29:25 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINNODERIGIDBODY2DPARAMETERS__H
#define MAINNODERIGIDBODY2DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CNodeRigidBody2D.h"

#include "Autogenerated/VisuNodeRigidBody2D.h"

//! AUTO: Parameters for class MainNodeRigidBody2DParameters
class MainNodeRigidBody2DParameters // AUTO: 
{
public: // AUTO: 
    Vector3D initialCoordinates;                  //!< AUTO: initial displacement coordinates and angle (relative to reference coordinates)
    Vector3D initialCoordinates_t;                //!< AUTO: initial velocity coordinates
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBody2DParameters()
    {
        initialCoordinates = Vector3D({0.,0.,0.});
        initialCoordinates_t = Vector3D({0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodeRigidBody2D
* @brief        A 2D rigid body node for rigid bodies or beams; the node has 2 displacement degrees of freedom and one rotation coordinate (rotation around z-axis: uphi). All coordinates are \hac{ODE2}, used for second order differetial equations.
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

//! AUTO: MainNodeRigidBody2D
class MainNodeRigidBody2D: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodeRigidBody2D* cNodeRigidBody2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodeRigidBody2D* visualizationNodeRigidBody2D; //pointer to computational object (initialized in object factory) AUTO:
    MainNodeRigidBody2DParameters parameters; //! AUTO: contains all parameters for MainNodeRigidBody2D

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBody2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodeRigidBody2D* GetCNodeRigidBody2D() { return cNodeRigidBody2D; }
    //! AUTO: Get const pointer to computational class
    const CNodeRigidBody2D* GetCNodeRigidBody2D() const { return cNodeRigidBody2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodeRigidBody2D(CNodeRigidBody2D* pCNodeRigidBody2D) { cNodeRigidBody2D = pCNodeRigidBody2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodeRigidBody2D* GetVisualizationNodeRigidBody2D() { return visualizationNodeRigidBody2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodeRigidBody2D* GetVisualizationNodeRigidBody2D() const { return visualizationNodeRigidBody2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodeRigidBody2D(VisualizationNodeRigidBody2D* pVisualizationNodeRigidBody2D) { visualizationNodeRigidBody2D = pVisualizationNodeRigidBody2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodeRigidBody2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodeRigidBody2D = (CNodeRigidBody2D*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodeRigidBody2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodeRigidBody2D = (VisualizationNodeRigidBody2D*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodeRigidBody2DParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodeRigidBody2DParameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword 'Node'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "RigidBody2D";
    }

    //! AUTO:  return internally stored initial coordinates (displacements) of node
    virtual LinkedDataVector GetInitialCoordinateVector() const override
    {
        return parameters.initialCoordinates;
    }

    //! AUTO:  return internally stored initial coordinates (velocities) of node
    virtual LinkedDataVector GetInitialCoordinateVector_t() const override
    {
        return parameters.initialCoordinates_t;
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "referenceCoordinates", cNodeRigidBody2D->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodeRigidBody2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodeRigidBody2D->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodeRigidBody2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = EPyUtils::SlimVector2NumPy(cNodeRigidBody2D->GetParameters().referenceCoordinates); //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates); //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates_t); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodeRigidBody2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodeRigidBody2D->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodeRigidBody2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return EPyUtils::SlimVector2NumPy(cNodeRigidBody2D->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodeRigidBody2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodeRigidBody2D->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodeRigidBody2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBody2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cNodeRigidBody2D->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodeRigidBody2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodeRigidBody2D->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodeRigidBody2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBody2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
