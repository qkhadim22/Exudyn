/** ***********************************************************************************************
* @class        MainLoadForceVectorParameters
* @brief        Parameter class for MainLoadForceVector
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-05-16  11:24:37 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINLOADFORCEVECTORPARAMETERS__H
#define MAINLOADFORCEVECTORPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CLoadForceVector.h"

#include "Autogenerated/VisuLoadForceVector.h"

//! AUTO: Parameters for class MainLoadForceVectorParameters
class MainLoadForceVectorParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainLoadForceVector
* @brief        Load with (3D) force vector; attached to position-based marker.
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

//! AUTO: MainLoadForceVector
class MainLoadForceVector: public MainLoad // AUTO: 
{
protected: // AUTO: 
    CLoadForceVector* cLoadForceVector; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationLoadForceVector* visualizationLoadForceVector; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainLoadForceVector()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CLoadForceVector* GetCLoadForceVector() { return cLoadForceVector; }
    //! AUTO: Get const pointer to computational class
    const CLoadForceVector* GetCLoadForceVector() const { return cLoadForceVector; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCLoadForceVector(CLoadForceVector* pCLoadForceVector) { cLoadForceVector = pCLoadForceVector; }

    //! AUTO: Get pointer to visualization class
    VisualizationLoadForceVector* GetVisualizationLoadForceVector() { return visualizationLoadForceVector; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationLoadForceVector* GetVisualizationLoadForceVector() const { return visualizationLoadForceVector; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationLoadForceVector(VisualizationLoadForceVector* pVisualizationLoadForceVector) { visualizationLoadForceVector = pVisualizationLoadForceVector; }

    //! AUTO: Get const pointer to computational base class object
    virtual CLoad* GetCLoad() const { return cLoadForceVector; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCLoad(CLoad* pCLoad) { cLoadForceVector = (CLoadForceVector*)pCLoad; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationLoad* GetVisualizationLoad() const { return visualizationLoadForceVector; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationLoad(VisualizationLoad* pVisualizationLoad) { visualizationLoadForceVector = (VisualizationLoadForceVector*)pVisualizationLoad; }

    //! AUTO:  Get type name of load (without keyword 'Load'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ForceVector";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cLoadForceVector->GetParameters().markerNumber = EPyUtils::GetMarkerIndexSafely(d["markerNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "loadVector", cLoadForceVector->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "bodyFixed")) { cLoadForceVector->GetParameters().bodyFixed = py::cast<bool>(d["bodyFixed"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "loadVectorUserFunction")) { if (EPyUtils::CheckForValidFunction(d["loadVectorUserFunction"])) 
            { cLoadForceVector->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,StdVector3D)>>((py::function)d["loadVectorUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cLoadForceVector->GetParameters().loadVectorUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationLoadForceVector->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["loadType"] = (std::string)GetTypeName();
        d["markerNumber"] = (MarkerIndex)cLoadForceVector->GetParameters().markerNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["loadVector"] = (std::vector<Real>)cLoadForceVector->GetParameters().loadVector; //! AUTO: cast variables into python (not needed for standard types) 
        d["bodyFixed"] = (bool)cLoadForceVector->GetParameters().bodyFixed; //! AUTO: cast variables into python (not needed for standard types) 
        if (cLoadForceVector->GetParameters().loadVectorUserFunction)
            {d["loadVectorUserFunction"] = (std::function<StdVector(const MainSystem&,Real,StdVector3D)>)cLoadForceVector->GetParameters().loadVectorUserFunction;}
        else
            {d["loadVectorUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationLoadForceVector->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { return py::cast((MarkerIndex)cLoadForceVector->GetParameters().markerNumber);} //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { return py::cast((std::vector<Real>)cLoadForceVector->GetParameters().loadVector);} //! AUTO: get parameter
        else if (parameterName.compare("bodyFixed") == 0) { return py::cast((bool)cLoadForceVector->GetParameters().bodyFixed);} //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { return py::cast((std::function<StdVector(const MainSystem&,Real,StdVector3D)>)cLoadForceVector->GetParameters().loadVectorUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationLoadForceVector->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("LoadForceVector::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { cLoadForceVector->GetParameters().markerNumber = EPyUtils::GetMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cLoadForceVector->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyFixed") == 0) { cLoadForceVector->GetParameters().bodyFixed = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cLoadForceVector->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,StdVector3D)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationLoadForceVector->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("LoadForceVector::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
