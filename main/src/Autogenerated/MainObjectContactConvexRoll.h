/** ***********************************************************************************************
* @class        MainObjectContactConvexRollParameters
* @brief        Parameter class for MainObjectContactConvexRoll
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

#ifndef MAINOBJECTCONTACTCONVEXROLLPARAMETERS__H
#define MAINOBJECTCONTACTCONVEXROLLPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectContactConvexRoll.h"

#include "Autogenerated/VisuObjectContactConvexRoll.h"

//! AUTO: Parameters for class MainObjectContactConvexRollParameters
class MainObjectContactConvexRollParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectContactConvexRoll
* @brief        A contact connector representing a convex roll (marker 1) on a flat surface (marker 0, ground body, not moving) in global \f$x\f$-\f$y\f$ plane. The connector is similar to ObjectConnectorRollingDiscPenalty, but includes a (strictly) convex shape of the roll defined by a polynomial. It is based on a penalty formulation and adds friction and slipping. The formulation is still under development and needs further testing. Note that the rolling body must have the reference point at the center of the disc.
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

//! AUTO: MainObjectContactConvexRoll
class MainObjectContactConvexRoll: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectContactConvexRoll* cObjectContactConvexRoll; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectContactConvexRoll* visualizationObjectContactConvexRoll; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectContactConvexRoll()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectContactConvexRoll* GetCObjectContactConvexRoll() { return cObjectContactConvexRoll; }
    //! AUTO: Get const pointer to computational class
    const CObjectContactConvexRoll* GetCObjectContactConvexRoll() const { return cObjectContactConvexRoll; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectContactConvexRoll(CObjectContactConvexRoll* pCObjectContactConvexRoll) { cObjectContactConvexRoll = pCObjectContactConvexRoll; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectContactConvexRoll* GetVisualizationObjectContactConvexRoll() { return visualizationObjectContactConvexRoll; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectContactConvexRoll* GetVisualizationObjectContactConvexRoll() const { return visualizationObjectContactConvexRoll; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectContactConvexRoll(VisualizationObjectContactConvexRoll* pVisualizationObjectContactConvexRoll) { visualizationObjectContactConvexRoll = pVisualizationObjectContactConvexRoll; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectContactConvexRoll; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectContactConvexRoll = (CObjectContactConvexRoll*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectContactConvexRoll; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectContactConvexRoll = (VisualizationObjectContactConvexRoll*)pVisualizationObject; }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::GenericData;
    }

    //! AUTO:  Get type name of node (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ContactConvexRoll";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectContactConvexRoll->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactConvexRoll->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactConvexRoll->GetParameters().contactStiffness = py::cast<Real>(d["contactStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "contactDamping")) { cObjectContactConvexRoll->GetParameters().contactDamping = py::cast<Real>(d["contactDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "dynamicFriction")) { cObjectContactConvexRoll->GetParameters().dynamicFriction = py::cast<Real>(d["dynamicFriction"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "staticFrictionOffset")) { cObjectContactConvexRoll->GetParameters().staticFrictionOffset = py::cast<Real>(d["staticFrictionOffset"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "viscousFriction")) { cObjectContactConvexRoll->GetParameters().viscousFriction = py::cast<Real>(d["viscousFriction"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "exponentialDecayStatic")) { cObjectContactConvexRoll->GetParameters().exponentialDecayStatic = py::cast<Real>(d["exponentialDecayStatic"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "frictionProportionalZone")) { cObjectContactConvexRoll->GetParameters().frictionProportionalZone = py::cast<Real>(d["frictionProportionalZone"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "rollLength")) { cObjectContactConvexRoll->GetParameters().rollLength = py::cast<Real>(d["rollLength"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "coefficientsHull")) { EPyUtils::SetNumpyVectorSafely(d, "coefficientsHull", cObjectContactConvexRoll->GetParameters().coefficientsHull); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectContactConvexRoll->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "rBoundingSphere")) { cObjectContactConvexRoll->GetRBoundingSphere() = py::cast<Real>(d["rBoundingSphere"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectContactConvexRoll->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectContactConvexRoll->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectContactConvexRoll->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (NodeIndex)cObjectContactConvexRoll->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactStiffness"] = (Real)cObjectContactConvexRoll->GetParameters().contactStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactDamping"] = (Real)cObjectContactConvexRoll->GetParameters().contactDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["dynamicFriction"] = (Real)cObjectContactConvexRoll->GetParameters().dynamicFriction; //! AUTO: cast variables into python (not needed for standard types) 
        d["staticFrictionOffset"] = (Real)cObjectContactConvexRoll->GetParameters().staticFrictionOffset; //! AUTO: cast variables into python (not needed for standard types) 
        d["viscousFriction"] = (Real)cObjectContactConvexRoll->GetParameters().viscousFriction; //! AUTO: cast variables into python (not needed for standard types) 
        d["exponentialDecayStatic"] = (Real)cObjectContactConvexRoll->GetParameters().exponentialDecayStatic; //! AUTO: cast variables into python (not needed for standard types) 
        d["frictionProportionalZone"] = (Real)cObjectContactConvexRoll->GetParameters().frictionProportionalZone; //! AUTO: cast variables into python (not needed for standard types) 
        d["rollLength"] = (Real)cObjectContactConvexRoll->GetParameters().rollLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["coefficientsHull"] = EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetParameters().coefficientsHull); //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectContactConvexRoll->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["coefficientsHullDerivative"] = EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetCoefficientsHullDerivative()); //! AUTO: cast variables into python (not needed for standard types) 
        d["coefficientsHullDDerivative"] = EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetCoefficientsHullDDerivative()); //! AUTO: cast variables into python (not needed for standard types) 
        d["rBoundingSphere"] = (Real)cObjectContactConvexRoll->GetRBoundingSphere(); //! AUTO: cast variables into python (not needed for standard types) 
        d["pContact"] = (std::vector<Real>)cObjectContactConvexRoll->GetPContact(); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectContactConvexRoll->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectContactConvexRoll->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectContactConvexRoll->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((NodeIndex)cObjectContactConvexRoll->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().contactStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().contactDamping);} //! AUTO: get parameter
        else if (parameterName.compare("dynamicFriction") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().dynamicFriction);} //! AUTO: get parameter
        else if (parameterName.compare("staticFrictionOffset") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().staticFrictionOffset);} //! AUTO: get parameter
        else if (parameterName.compare("viscousFriction") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().viscousFriction);} //! AUTO: get parameter
        else if (parameterName.compare("exponentialDecayStatic") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().exponentialDecayStatic);} //! AUTO: get parameter
        else if (parameterName.compare("frictionProportionalZone") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().frictionProportionalZone);} //! AUTO: get parameter
        else if (parameterName.compare("rollLength") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetParameters().rollLength);} //! AUTO: get parameter
        else if (parameterName.compare("coefficientsHull") == 0) { return EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetParameters().coefficientsHull);} //! AUTO: get parameter
        else if (parameterName.compare("coefficientsHullDerivative") == 0) { return EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetCoefficientsHullDerivative());} //! AUTO: get parameter
        else if (parameterName.compare("coefficientsHullDDerivative") == 0) { return EPyUtils::Vector2NumPy(cObjectContactConvexRoll->GetCoefficientsHullDDerivative());} //! AUTO: get parameter
        else if (parameterName.compare("rBoundingSphere") == 0) { return py::cast((Real)cObjectContactConvexRoll->GetRBoundingSphere());} //! AUTO: get parameter
        else if (parameterName.compare("pContact") == 0) { return py::cast((std::vector<Real>)cObjectContactConvexRoll->GetPContact());} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectContactConvexRoll->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectContactConvexRoll->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectContactConvexRoll->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectContactConvexRoll::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectContactConvexRoll->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectContactConvexRoll->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { cObjectContactConvexRoll->GetParameters().contactStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { cObjectContactConvexRoll->GetParameters().contactDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dynamicFriction") == 0) { cObjectContactConvexRoll->GetParameters().dynamicFriction = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("staticFrictionOffset") == 0) { cObjectContactConvexRoll->GetParameters().staticFrictionOffset = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("viscousFriction") == 0) { cObjectContactConvexRoll->GetParameters().viscousFriction = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("exponentialDecayStatic") == 0) { cObjectContactConvexRoll->GetParameters().exponentialDecayStatic = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("frictionProportionalZone") == 0) { cObjectContactConvexRoll->GetParameters().frictionProportionalZone = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rollLength") == 0) { cObjectContactConvexRoll->GetParameters().rollLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("coefficientsHull") == 0) { EPyUtils::SetNumpyVectorSafely(value, cObjectContactConvexRoll->GetParameters().coefficientsHull); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rBoundingSphere") == 0) { cObjectContactConvexRoll->GetRBoundingSphere() = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectContactConvexRoll->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectContactConvexRoll->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectContactConvexRoll->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectContactConvexRoll::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
