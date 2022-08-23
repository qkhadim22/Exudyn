/** ***********************************************************************************************
* @class        MainObjectJointGenericParameters
* @brief        Parameter class for MainObjectJointGeneric
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-07-21  20:27:19 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTJOINTGENERICPARAMETERS__H
#define MAINOBJECTJOINTGENERICPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectJointGeneric.h"

#include "Autogenerated/VisuObjectJointGeneric.h"

//! AUTO: Parameters for class MainObjectJointGenericParameters
class MainObjectJointGenericParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectJointGeneric
* @brief        A generic joint in 3D; constrains components of the absolute position and rotations of two points given by PointMarkers or RigidMarkers. An additional local rotation (rotationMarker) can be used to adjust the three rotation axes and/or sliding axes.
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

//! AUTO: MainObjectJointGeneric
class MainObjectJointGeneric: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectJointGeneric* cObjectJointGeneric; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectJointGeneric* visualizationObjectJointGeneric; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectJointGeneric()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectJointGeneric* GetCObjectJointGeneric() { return cObjectJointGeneric; }
    //! AUTO: Get const pointer to computational class
    const CObjectJointGeneric* GetCObjectJointGeneric() const { return cObjectJointGeneric; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectJointGeneric(CObjectJointGeneric* pCObjectJointGeneric) { cObjectJointGeneric = pCObjectJointGeneric; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectJointGeneric* GetVisualizationObjectJointGeneric() { return visualizationObjectJointGeneric; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectJointGeneric* GetVisualizationObjectJointGeneric() const { return visualizationObjectJointGeneric; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectJointGeneric(VisualizationObjectJointGeneric* pVisualizationObjectJointGeneric) { visualizationObjectJointGeneric = pVisualizationObjectJointGeneric; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectJointGeneric; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectJointGeneric = (CObjectJointGeneric*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectJointGeneric; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectJointGeneric = (VisualizationObjectJointGeneric*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "JointGeneric";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectJointGeneric->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "constrainedAxes")) { cObjectJointGeneric->GetParameters().constrainedAxes = py::cast<std::vector<Index>>(d["constrainedAxes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetConstMatrixTemplateSafely<3,3>(d, "rotationMarker0", cObjectJointGeneric->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetConstMatrixTemplateSafely<3,3>(d, "rotationMarker1", cObjectJointGeneric->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectJointGeneric->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetSlimVectorTemplateSafely<Real, 6>(d, "offsetUserFunctionParameters", cObjectJointGeneric->GetParameters().offsetUserFunctionParameters); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "offsetUserFunction")) { if (EPyUtils::CheckForValidFunction(d["offsetUserFunction"])) 
            { cObjectJointGeneric->GetParameters().offsetUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>>((py::function)d["offsetUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectJointGeneric->GetParameters().offsetUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "offsetUserFunction_t")) { if (EPyUtils::CheckForValidFunction(d["offsetUserFunction_t"])) 
            { cObjectJointGeneric->GetParameters().offsetUserFunction_t = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>>((py::function)d["offsetUserFunction_t"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectJointGeneric->GetParameters().offsetUserFunction_t = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectJointGeneric->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VaxesRadius")) { visualizationObjectJointGeneric->GetAxesRadius() = py::cast<float>(d["VaxesRadius"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VaxesLength")) { visualizationObjectJointGeneric->GetAxesLength() = py::cast<float>(d["VaxesLength"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectJointGeneric->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary(bool addGraphicsData=false) const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectJointGeneric->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["constrainedAxes"] = (std::vector<Index>)cObjectJointGeneric->GetParameters().constrainedAxes; //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker0"] = EPyUtils::Matrix2NumPyTemplate(cObjectJointGeneric->GetParameters().rotationMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker1"] = EPyUtils::Matrix2NumPyTemplate(cObjectJointGeneric->GetParameters().rotationMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectJointGeneric->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["offsetUserFunctionParameters"] = EPyUtils::SlimVector2NumPy(cObjectJointGeneric->GetParameters().offsetUserFunctionParameters); //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectJointGeneric->GetParameters().offsetUserFunction)
            {d["offsetUserFunction"] = (std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>)cObjectJointGeneric->GetParameters().offsetUserFunction;}
        else
            {d["offsetUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectJointGeneric->GetParameters().offsetUserFunction_t)
            {d["offsetUserFunction_t"] = (std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>)cObjectJointGeneric->GetParameters().offsetUserFunction_t;}
        else
            {d["offsetUserFunction_t"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectJointGeneric->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VaxesRadius"] = (float)visualizationObjectJointGeneric->GetAxesRadius(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VaxesLength"] = (float)visualizationObjectJointGeneric->GetAxesLength(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectJointGeneric->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectJointGeneric->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("constrainedAxes") == 0) { return py::cast((std::vector<Index>)cObjectJointGeneric->GetParameters().constrainedAxes);} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { return EPyUtils::Matrix2NumPyTemplate(cObjectJointGeneric->GetParameters().rotationMarker0);} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { return EPyUtils::Matrix2NumPyTemplate(cObjectJointGeneric->GetParameters().rotationMarker1);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectJointGeneric->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunctionParameters") == 0) { return EPyUtils::SlimVector2NumPy(cObjectJointGeneric->GetParameters().offsetUserFunctionParameters);} //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction") == 0) { return py::cast((std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>)cObjectJointGeneric->GetParameters().offsetUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction_t") == 0) { return py::cast((std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>)cObjectJointGeneric->GetParameters().offsetUserFunction_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectJointGeneric->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VaxesRadius") == 0) { return py::cast((float)visualizationObjectJointGeneric->GetAxesRadius());} //! AUTO: get parameter
        else if (parameterName.compare("VaxesLength") == 0) { return py::cast((float)visualizationObjectJointGeneric->GetAxesLength());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectJointGeneric->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointGeneric::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectJointGeneric->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("constrainedAxes") == 0) { cObjectJointGeneric->GetParameters().constrainedAxes = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { EPyUtils::SetConstMatrixTemplateSafely<3,3>(value, cObjectJointGeneric->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { EPyUtils::SetConstMatrixTemplateSafely<3,3>(value, cObjectJointGeneric->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectJointGeneric->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunctionParameters") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 6>(value, cObjectJointGeneric->GetParameters().offsetUserFunctionParameters); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectJointGeneric->GetParameters().offsetUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("offsetUserFunction_t") == 0) { if (py::isinstance<py::function>(value)) {cObjectJointGeneric->GetParameters().offsetUserFunction_t = py::cast<std::function<StdVector(const MainSystem&,Real,Index,StdVector6D)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectJointGeneric->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VaxesRadius") == 0) { visualizationObjectJointGeneric->GetAxesRadius() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VaxesLength") == 0) { visualizationObjectJointGeneric->GetAxesLength() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectJointGeneric->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointGeneric::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
