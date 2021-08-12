/** ***********************************************************************************************
* @class        MainObjectBeamGeometricallyExact2DParameters
* @brief        Parameter class for MainObjectBeamGeometricallyExact2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:20:58 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTBEAMGEOMETRICALLYEXACT2DPARAMETERS__H
#define MAINOBJECTBEAMGEOMETRICALLYEXACT2DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectBeamGeometricallyExact2D.h"

#include "Autogenerated/VisuObjectBeamGeometricallyExact2D.h"

//! AUTO: Parameters for class MainObjectBeamGeometricallyExact2DParameters
class MainObjectBeamGeometricallyExact2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectBeamGeometricallyExact2D
* @brief        A 2D geometrically exact beam finite element, currently using 2 nodes of type NodeRigidBody2D. The localPosition of the beam with length \f$L\f$=physicsLength and height \f$h\f$ ranges in \f$X\f$-direction in range \f$[-L/2, L/2]\f$ and in \f$Y\f$-direction in range \f$[-h/2,h/2]\f$ (which is in fact not needed in the \hac{EOM}).
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

//! AUTO: MainObjectBeamGeometricallyExact2D
class MainObjectBeamGeometricallyExact2D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectBeamGeometricallyExact2D* cObjectBeamGeometricallyExact2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectBeamGeometricallyExact2D* visualizationObjectBeamGeometricallyExact2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectBeamGeometricallyExact2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectBeamGeometricallyExact2D* GetCObjectBeamGeometricallyExact2D() { return cObjectBeamGeometricallyExact2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectBeamGeometricallyExact2D* GetCObjectBeamGeometricallyExact2D() const { return cObjectBeamGeometricallyExact2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectBeamGeometricallyExact2D(CObjectBeamGeometricallyExact2D* pCObjectBeamGeometricallyExact2D) { cObjectBeamGeometricallyExact2D = pCObjectBeamGeometricallyExact2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectBeamGeometricallyExact2D* GetVisualizationObjectBeamGeometricallyExact2D() { return visualizationObjectBeamGeometricallyExact2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectBeamGeometricallyExact2D* GetVisualizationObjectBeamGeometricallyExact2D() const { return visualizationObjectBeamGeometricallyExact2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectBeamGeometricallyExact2D(VisualizationObjectBeamGeometricallyExact2D* pVisualizationObjectBeamGeometricallyExact2D) { visualizationObjectBeamGeometricallyExact2D = pVisualizationObjectBeamGeometricallyExact2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectBeamGeometricallyExact2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectBeamGeometricallyExact2D = (CObjectBeamGeometricallyExact2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectBeamGeometricallyExact2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectBeamGeometricallyExact2D = (VisualizationObjectBeamGeometricallyExact2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ANCFCable2D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position2D + Node::Orientation2D);
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectBeamGeometricallyExact2D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsLength = py::cast<Real>(d["physicsLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsMassPerLength = py::cast<Real>(d["physicsMassPerLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsCrossSectionInertia = py::cast<Real>(d["physicsCrossSectionInertia"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsBendingStiffness = py::cast<Real>(d["physicsBendingStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsAxialStiffness = py::cast<Real>(d["physicsAxialStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectBeamGeometricallyExact2D->GetParameters().physicsShearStiffness = py::cast<Real>(d["physicsShearStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectBeamGeometricallyExact2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawHeight")) { visualizationObjectBeamGeometricallyExact2D->GetDrawHeight() = py::cast<float>(d["VdrawHeight"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectBeamGeometricallyExact2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["nodeNumbers"] = EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectBeamGeometricallyExact2D->GetParameters().nodeNumbers)); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsLength"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMassPerLength"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsMassPerLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsCrossSectionInertia"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsCrossSectionInertia; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsBendingStiffness"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsBendingStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsAxialStiffness"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsAxialStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsShearStiffness"] = (Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsShearStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectBeamGeometricallyExact2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawHeight"] = (float)visualizationObjectBeamGeometricallyExact2D->GetDrawHeight(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectBeamGeometricallyExact2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast(EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectBeamGeometricallyExact2D->GetParameters().nodeNumbers)));} //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsMassPerLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsCrossSectionInertia") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsCrossSectionInertia);} //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsBendingStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsAxialStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsShearStiffness") == 0) { return py::cast((Real)cObjectBeamGeometricallyExact2D->GetParameters().physicsShearStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectBeamGeometricallyExact2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { return py::cast((float)visualizationObjectBeamGeometricallyExact2D->GetDrawHeight());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectBeamGeometricallyExact2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectBeamGeometricallyExact2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsMassPerLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsCrossSectionInertia") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsCrossSectionInertia = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsBendingStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsAxialStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsShearStiffness") == 0) { cObjectBeamGeometricallyExact2D->GetParameters().physicsShearStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectBeamGeometricallyExact2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { visualizationObjectBeamGeometricallyExact2D->GetDrawHeight() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectBeamGeometricallyExact2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectBeamGeometricallyExact2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
