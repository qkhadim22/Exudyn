/** ***********************************************************************************************
* @class        MainNodeRigidBodyEPParameters
* @brief        Parameter class for MainNodeRigidBodyEP
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
#include "Autogenerated/CNodeRigidBodyEP.h"

#include "Autogenerated/VisuNodeRigidBodyEP.h"

//! AUTO: Parameters for class MainNodeRigidBodyEPParameters
class MainNodeRigidBodyEPParameters // AUTO: 
{
public: // AUTO: 
    Vector7D initialCoordinates;                  //!< AUTO: initial displacement coordinates: ux,uy,uz and 4 Euler parameters relative to reference coordinates
    Vector7D initialCoordinates_t;                //!< AUTO: initial velocity coordinate: time derivatives of ux,uy,uz and 4 Euler parameters
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBodyEPParameters()
    {
        initialCoordinates = Vector7D({0.,0.,0., 0.,0.,0.,0.});
        initialCoordinates_t = Vector7D({0.,0.,0., 0.,0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodeRigidBodyEP
* @brief        A 3D rigid body node based on Euler parameters for rigid bodies or beams; the node has 3 displacement coordinates (displacements of center of mass - COM: ux,uy,uz) and four rotation coordinates (Euler parameters = quaternions); all coordinates lead to second order differential equations; additionally there is one constraint equation for quaternions; The rotation matrix \f$\Am\f$, transforming local (body-fixed) 3D positions \f$\pv_{loc} = [p^x_{loc}\;\;p^y_{loc}\;\;p^z_{loc}]^T\f$ to global 3D positions \f$\pv_{glob} = [p^x_{glob}\;\;p^y_{glob}\;\;p^z_{glob}]^T\f$, \f[ \pv_{glob} = \Am \pv_{loc}, \f] is defined according to the book of Shabana, same with the transformation matrix \f$\mathbf{G}\f$ between time derivatives of Euler parameters and angular velocities.
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

//! AUTO: MainNodeRigidBodyEP
class MainNodeRigidBodyEP: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodeRigidBodyEP* cNodeRigidBodyEP; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodeRigidBodyEP* visualizationNodeRigidBodyEP; //pointer to computational object (initialized in object factory) AUTO:
    MainNodeRigidBodyEPParameters parameters; //! AUTO: contains all parameters for MainNodeRigidBodyEP

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBodyEP()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodeRigidBodyEP* GetCNodeRigidBodyEP() { return cNodeRigidBodyEP; }
    //! AUTO: Get const pointer to computational class
    const CNodeRigidBodyEP* GetCNodeRigidBodyEP() const { return cNodeRigidBodyEP; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodeRigidBodyEP(CNodeRigidBodyEP* pCNodeRigidBodyEP) { cNodeRigidBodyEP = pCNodeRigidBodyEP; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodeRigidBodyEP* GetVisualizationNodeRigidBodyEP() { return visualizationNodeRigidBodyEP; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodeRigidBodyEP* GetVisualizationNodeRigidBodyEP() const { return visualizationNodeRigidBodyEP; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodeRigidBodyEP(VisualizationNodeRigidBodyEP* pVisualizationNodeRigidBodyEP) { visualizationNodeRigidBodyEP = pVisualizationNodeRigidBodyEP; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodeRigidBodyEP; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodeRigidBodyEP = (CNodeRigidBodyEP*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodeRigidBodyEP; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodeRigidBodyEP = (VisualizationNodeRigidBodyEP*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodeRigidBodyEPParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodeRigidBodyEPParameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword "Node"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "RigidBodyEP";
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
        EPyUtils::SetVector7DSafely(d, "referenceCoordinates", cNodeRigidBodyEP->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetVector7DSafely(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetVector7DSafely(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodeRigidBodyEP->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodeRigidBodyEP->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodeRigidBodyEP->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodeRigidBodyEP->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodeRigidBodyEP->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodeRigidBodyEP->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodeRigidBodyEP->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodeRigidBodyEP->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodeRigidBodyEP->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodeRigidBodyEP->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodeRigidBodyEP->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBodyEP::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetVector7DSafely(value, cNodeRigidBodyEP->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetVector7DSafely(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetVector7DSafely(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodeRigidBodyEP->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodeRigidBodyEP->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodeRigidBodyEP->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBodyEP::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


