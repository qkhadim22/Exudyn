/** ***********************************************************************************************
* @class        MainObjectANCFBeam3DParameters
* @brief        Parameter class for MainObjectANCFBeam3D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-05-16  19:50:50 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTANCFBEAM3DPARAMETERS__H
#define MAINOBJECTANCFBEAM3DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CObjectANCFBeam3D.h"

#include "Autogenerated/VisuObjectANCFBeam3D.h"
#include "Autogenerated/PyStructuralElementsDataStructures.h"

//! AUTO: Parameters for class MainObjectANCFBeam3DParameters
class MainObjectANCFBeam3DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectANCFBeam3D
* @brief        OBJECT UNDER CONSTRUCTION: A 3D beam finite element based on the absolute nodal coordinate formulation, using two . The localPosition \f$x\f$ of the beam ranges from \f$-L/2\f$ (at node 0) to \f$L/2\f$ (at node 1). The axial coordinate is \f$x\f$ (first coordinate) and the cross section is spanned by local \f$y\f$/\f$z\f$ axes; assuming dimensions \f$w_y\f$ and \f$w_z\f$ in cross section, the local position range is \f$\in [[-L/2,L/2],\, [-wy/2,wy/2],\, [-wz/2,wz/2] ]\f$.
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

//! AUTO: MainObjectANCFBeam3D
class MainObjectANCFBeam3D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectANCFBeam3D* cObjectANCFBeam3D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectANCFBeam3D* visualizationObjectANCFBeam3D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectANCFBeam3D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectANCFBeam3D* GetCObjectANCFBeam3D() { return cObjectANCFBeam3D; }
    //! AUTO: Get const pointer to computational class
    const CObjectANCFBeam3D* GetCObjectANCFBeam3D() const { return cObjectANCFBeam3D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectANCFBeam3D(CObjectANCFBeam3D* pCObjectANCFBeam3D) { cObjectANCFBeam3D = pCObjectANCFBeam3D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectANCFBeam3D* GetVisualizationObjectANCFBeam3D() { return visualizationObjectANCFBeam3D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectANCFBeam3D* GetVisualizationObjectANCFBeam3D() const { return visualizationObjectANCFBeam3D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectANCFBeam3D(VisualizationObjectANCFBeam3D* pVisualizationObjectANCFBeam3D) { visualizationObjectANCFBeam3D = pVisualizationObjectANCFBeam3D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectANCFBeam3D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectANCFBeam3D = (CObjectANCFBeam3D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectANCFBeam3D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectANCFBeam3D = (VisualizationObjectANCFBeam3D*)pVisualizationObject; }

    void SetInternalBeamSection(const py::object& pyObject); //! AUTO: special function which writes pyObject into local data
    PyBeamSection GetInternalBeamSection() const; //! AUTO: special function which returns BeamSection converted from local data
    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ObjectANCFBeam3D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::Point3DSlope23);
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectANCFBeam3D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFBeam3D->GetParameters().physicsLength = py::cast<Real>(d["physicsLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(d, "crossSectionPenaltyFactor", cObjectANCFBeam3D->GetParameters().crossSectionPenaltyFactor); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(d, "testBeamRectangularSize", cObjectANCFBeam3D->GetParameters().testBeamRectangularSize); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        SetInternalBeamSection(d["sectionData"]); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectANCFBeam3D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VsectionGeometry")) { visualizationObjectANCFBeam3D->GetSectionGeometry() = py::cast<BeamSectionGeometry>(d["VsectionGeometry"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectANCFBeam3D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["nodeNumbers"] = EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectANCFBeam3D->GetParameters().nodeNumbers)); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsLength"] = (Real)cObjectANCFBeam3D->GetParameters().physicsLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["crossSectionPenaltyFactor"] = (std::vector<Real>)cObjectANCFBeam3D->GetParameters().crossSectionPenaltyFactor; //! AUTO: cast variables into python (not needed for standard types) 
        d["testBeamRectangularSize"] = (std::vector<Real>)cObjectANCFBeam3D->GetParameters().testBeamRectangularSize; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["sectionData"] = GetInternalBeamSection(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectANCFBeam3D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VsectionGeometry"] = (BeamSectionGeometry)visualizationObjectANCFBeam3D->GetSectionGeometry(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectANCFBeam3D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast(EPyUtils::GetArrayNodeIndex(ArrayIndex(cObjectANCFBeam3D->GetParameters().nodeNumbers)));} //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { return py::cast((Real)cObjectANCFBeam3D->GetParameters().physicsLength);} //! AUTO: get parameter
        else if (parameterName.compare("sectionData") == 0) { return py::cast(GetInternalBeamSection());} //! AUTO: get parameter
        else if (parameterName.compare("crossSectionPenaltyFactor") == 0) { return py::cast((std::vector<Real>)cObjectANCFBeam3D->GetParameters().crossSectionPenaltyFactor);} //! AUTO: get parameter
        else if (parameterName.compare("testBeamRectangularSize") == 0) { return py::cast((std::vector<Real>)cObjectANCFBeam3D->GetParameters().testBeamRectangularSize);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectANCFBeam3D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VsectionGeometry") == 0) { return py::cast((BeamSectionGeometry)visualizationObjectANCFBeam3D->GetSectionGeometry());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectANCFBeam3D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFBeam3D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectANCFBeam3D->GetParameters().nodeNumbers = EPyUtils::GetNodeIndex2Safely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { cObjectANCFBeam3D->GetParameters().physicsLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("sectionData") == 0) { SetInternalBeamSection(value); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("crossSectionPenaltyFactor") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 3>(value, cObjectANCFBeam3D->GetParameters().crossSectionPenaltyFactor); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("testBeamRectangularSize") == 0) { EPyUtils::SetSlimVectorTemplateSafely<Real, 2>(value, cObjectANCFBeam3D->GetParameters().testBeamRectangularSize); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectANCFBeam3D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VsectionGeometry") == 0) { visualizationObjectANCFBeam3D->GetSectionGeometry() = py::cast<BeamSectionGeometry>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectANCFBeam3D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFBeam3D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
