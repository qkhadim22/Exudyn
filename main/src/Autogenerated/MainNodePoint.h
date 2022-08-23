/** ***********************************************************************************************
* @class        MainNodePointParameters
* @brief        Parameter class for MainNodePoint
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

#ifndef MAINNODEPOINTPARAMETERS__H
#define MAINNODEPOINTPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CNodePoint.h"

#include "Autogenerated/VisuNodePoint.h"

//! AUTO: Parameters for class MainNodePointParameters
class MainNodePointParameters // AUTO: 
{
public: // AUTO: 
    Vector3D initialCoordinates;                  //!< AUTO: initial displacement coordinate
    Vector3D initialCoordinates_t;                //!< AUTO: initial velocity coordinate
    //! AUTO: default constructor with parameter initialization
    MainNodePointParameters()
    {
        initialCoordinates = Vector3D({0.,0.,0.});
        initialCoordinates_t = Vector3D({0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodePoint
* @brief        A 3D point node for point masses or solid finite elements which has 3 displacement degrees of freedom for \acf{ODE2}.
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

//! AUTO: MainNodePoint
class MainNodePoint: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodePoint* cNodePoint; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodePoint* visualizationNodePoint; //pointer to computational object (initialized in object factory) AUTO:
    MainNodePointParameters parameters; //! AUTO: contains all parameters for MainNodePoint

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodePoint()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodePoint* GetCNodePoint() { return cNodePoint; }
    //! AUTO: Get const pointer to computational class
    const CNodePoint* GetCNodePoint() const { return cNodePoint; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodePoint(CNodePoint* pCNodePoint) { cNodePoint = pCNodePoint; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodePoint* GetVisualizationNodePoint() { return visualizationNodePoint; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodePoint* GetVisualizationNodePoint() const { return visualizationNodePoint; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodePoint(VisualizationNodePoint* pVisualizationNodePoint) { visualizationNodePoint = pVisualizationNodePoint; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodePoint; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodePoint = (CNodePoint*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodePoint; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodePoint = (VisualizationNodePoint*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodePointParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodePointParameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword 'Node'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Point";
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
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "referenceCoordinates", cNodePoint->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodePoint->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodePoint->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodePoint->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = EPyUtils::SlimVector2NumPy(cNodePoint->GetParameters().referenceCoordinates); //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates); //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates_t); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodePoint->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodePoint->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodePoint->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return EPyUtils::SlimVector2NumPy(cNodePoint->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return EPyUtils::SlimVector2NumPy(GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodePoint->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodePoint->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodePoint->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cNodePoint->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodePoint->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodePoint->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodePoint->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
