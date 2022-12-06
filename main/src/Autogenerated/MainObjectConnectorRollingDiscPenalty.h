/** ***********************************************************************************************
* @class        MainObjectConnectorRollingDiscPenaltyParameters
* @brief        Parameter class for MainObjectConnectorRollingDiscPenalty
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-11-04  16:58:02 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTCONNECTORROLLINGDISCPENALTYPARAMETERS__H
#define MAINOBJECTCONNECTORROLLINGDISCPENALTYPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectConnectorRollingDiscPenalty.h"

#include "Autogenerated/VisuObjectConnectorRollingDiscPenalty.h"

//! AUTO: Parameters for class MainObjectConnectorRollingDiscPenaltyParameters
class MainObjectConnectorRollingDiscPenaltyParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorRollingDiscPenalty
* @brief        A (flexible) connector representing a rolling rigid disc (marker 1) on a flat surface (marker 0, ground body, not moving) in global \f$x\f$-\f$y\f$ plane. The connector is based on a penalty formulation and adds friction and slipping. The contraints works for discs as long as the disc axis and the plane normal vector are not parallel. Parameters may need to be adjusted for better convergence (e.g., dryFrictionProportionalZone). The formulation for the arbitrary disc axis is still under development and needs further testing. Note that the rolling body must have the reference point at the center of the disc.
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

//! AUTO: MainObjectConnectorRollingDiscPenalty
class MainObjectConnectorRollingDiscPenalty: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorRollingDiscPenalty* cObjectConnectorRollingDiscPenalty; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorRollingDiscPenalty* visualizationObjectConnectorRollingDiscPenalty; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorRollingDiscPenalty()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorRollingDiscPenalty* GetCObjectConnectorRollingDiscPenalty() { return cObjectConnectorRollingDiscPenalty; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorRollingDiscPenalty* GetCObjectConnectorRollingDiscPenalty() const { return cObjectConnectorRollingDiscPenalty; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorRollingDiscPenalty(CObjectConnectorRollingDiscPenalty* pCObjectConnectorRollingDiscPenalty) { cObjectConnectorRollingDiscPenalty = pCObjectConnectorRollingDiscPenalty; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorRollingDiscPenalty* GetVisualizationObjectConnectorRollingDiscPenalty() { return visualizationObjectConnectorRollingDiscPenalty; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorRollingDiscPenalty* GetVisualizationObjectConnectorRollingDiscPenalty() const { return visualizationObjectConnectorRollingDiscPenalty; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorRollingDiscPenalty(VisualizationObjectConnectorRollingDiscPenalty* pVisualizationObjectConnectorRollingDiscPenalty) { visualizationObjectConnectorRollingDiscPenalty = pVisualizationObjectConnectorRollingDiscPenalty; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorRollingDiscPenalty; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorRollingDiscPenalty = (CObjectConnectorRollingDiscPenalty*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorRollingDiscPenalty; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorRollingDiscPenalty = (VisualizationObjectConnectorRollingDiscPenalty*)pVisualizationObject; }

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
        return "ConnectorRollingDiscPenalty";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorRollingDiscPenalty->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorRollingDiscPenalty->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorRollingDiscPenalty->GetParameters().discRadius = py::cast<Real>(d["discRadius"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "discAxis")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "discAxis", cObjectConnectorRollingDiscPenalty->GetParameters().discAxis); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "planeNormal")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "planeNormal", cObjectConnectorRollingDiscPenalty->GetParameters().planeNormal); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "dryFrictionAngle")) { cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionAngle = py::cast<Real>(d["dryFrictionAngle"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        cObjectConnectorRollingDiscPenalty->GetParameters().contactStiffness = py::cast<Real>(d["contactStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "contactDamping")) { cObjectConnectorRollingDiscPenalty->GetParameters().contactDamping = py::cast<Real>(d["contactDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "dryFriction")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(d, "dryFriction", cObjectConnectorRollingDiscPenalty->GetParameters().dryFriction); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "dryFrictionProportionalZone")) { cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionProportionalZone = py::cast<Real>(d["dryFrictionProportionalZone"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "viscousFriction")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(d, "viscousFriction", cObjectConnectorRollingDiscPenalty->GetParameters().viscousFriction); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "rollingFrictionViscous")) { cObjectConnectorRollingDiscPenalty->GetParameters().rollingFrictionViscous = py::cast<Real>(d["rollingFrictionViscous"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "useLinearProportionalZone")) { cObjectConnectorRollingDiscPenalty->GetParameters().useLinearProportionalZone = py::cast<bool>(d["useLinearProportionalZone"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorRollingDiscPenalty->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorRollingDiscPenalty->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdiscWidth")) { visualizationObjectConnectorRollingDiscPenalty->GetDiscWidth() = py::cast<float>(d["VdiscWidth"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorRollingDiscPenalty->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectConnectorRollingDiscPenalty->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (NodeIndex)cObjectConnectorRollingDiscPenalty->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["discRadius"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().discRadius; //! AUTO: cast variables into python (not needed for standard types) 
        d["discAxis"] = EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().discAxis); //! AUTO: cast variables into python (not needed for standard types) 
        d["planeNormal"] = EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().planeNormal); //! AUTO: cast variables into python (not needed for standard types) 
        d["dryFrictionAngle"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionAngle; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactStiffness"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().contactStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactDamping"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().contactDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["dryFriction"] = EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().dryFriction); //! AUTO: cast variables into python (not needed for standard types) 
        d["dryFrictionProportionalZone"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionProportionalZone; //! AUTO: cast variables into python (not needed for standard types) 
        d["viscousFriction"] = EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().viscousFriction); //! AUTO: cast variables into python (not needed for standard types) 
        d["rollingFrictionViscous"] = (Real)cObjectConnectorRollingDiscPenalty->GetParameters().rollingFrictionViscous; //! AUTO: cast variables into python (not needed for standard types) 
        d["useLinearProportionalZone"] = (bool)cObjectConnectorRollingDiscPenalty->GetParameters().useLinearProportionalZone; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorRollingDiscPenalty->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorRollingDiscPenalty->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdiscWidth"] = (float)visualizationObjectConnectorRollingDiscPenalty->GetDiscWidth(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorRollingDiscPenalty->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectConnectorRollingDiscPenalty->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((NodeIndex)cObjectConnectorRollingDiscPenalty->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("discRadius") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().discRadius);} //! AUTO: get parameter
        else if (parameterName.compare("discAxis") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().discAxis);} //! AUTO: get parameter
        else if (parameterName.compare("planeNormal") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().planeNormal);} //! AUTO: get parameter
        else if (parameterName.compare("dryFrictionAngle") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionAngle);} //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().contactStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().contactDamping);} //! AUTO: get parameter
        else if (parameterName.compare("dryFriction") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().dryFriction);} //! AUTO: get parameter
        else if (parameterName.compare("dryFrictionProportionalZone") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionProportionalZone);} //! AUTO: get parameter
        else if (parameterName.compare("viscousFriction") == 0) { return EPyUtils::SlimVector2NumPy(cObjectConnectorRollingDiscPenalty->GetParameters().viscousFriction);} //! AUTO: get parameter
        else if (parameterName.compare("rollingFrictionViscous") == 0) { return py::cast((Real)cObjectConnectorRollingDiscPenalty->GetParameters().rollingFrictionViscous);} //! AUTO: get parameter
        else if (parameterName.compare("useLinearProportionalZone") == 0) { return py::cast((bool)cObjectConnectorRollingDiscPenalty->GetParameters().useLinearProportionalZone);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorRollingDiscPenalty->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorRollingDiscPenalty->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdiscWidth") == 0) { return py::cast((float)visualizationObjectConnectorRollingDiscPenalty->GetDiscWidth());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorRollingDiscPenalty->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorRollingDiscPenalty::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("discRadius") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().discRadius = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("discAxis") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectConnectorRollingDiscPenalty->GetParameters().discAxis); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("planeNormal") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectConnectorRollingDiscPenalty->GetParameters().planeNormal); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dryFrictionAngle") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionAngle = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().contactStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().contactDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dryFriction") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(value, cObjectConnectorRollingDiscPenalty->GetParameters().dryFriction); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dryFrictionProportionalZone") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().dryFrictionProportionalZone = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("viscousFriction") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(value, cObjectConnectorRollingDiscPenalty->GetParameters().viscousFriction); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rollingFrictionViscous") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().rollingFrictionViscous = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("useLinearProportionalZone") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().useLinearProportionalZone = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorRollingDiscPenalty->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorRollingDiscPenalty->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdiscWidth") == 0) { visualizationObjectConnectorRollingDiscPenalty->GetDiscWidth() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorRollingDiscPenalty->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorRollingDiscPenalty::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
