/** ***********************************************************************************************
* @class        MainObjectFFRFreducedOrderParameters
* @brief        Parameter class for MainObjectFFRFreducedOrder
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-05-05  08:06:29 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTFFRFREDUCEDORDERPARAMETERS__H
#define MAINOBJECTFFRFREDUCEDORDERPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectFFRFreducedOrder.h"

#include "Autogenerated/VisuObjectFFRFreducedOrder.h"

//! AUTO: Parameters for class MainObjectFFRFreducedOrderParameters
class MainObjectFFRFreducedOrderParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectFFRFreducedOrder
* @brief        This object is used to represent modally reduced flexible bodies using the floating frame of reference formulation (FFRF) and the component mode synthesis (CMS). It can be used to model real-life mechanical systems imported from finite element codes or python tools such as NETGEN/NGsolve, see the \texttt{FEMinterface} in \refSection{sec:FEM:FEMinterface:__init__}. It contains a RigidBodyNode (always node 0) and a NodeGenericODE2 representing the modal coordinates. Currently, equations must be defined within user functions, which are available in the FEM module, see class \texttt{ObjectFFRFreducedOrderInterface}, especially the user functions \texttt{UFmassFFRFreducedOrder} and \texttt{UFforceFFRFreducedOrder}, \refSection{sec:FEM:ObjectFFRFreducedOrderInterface:AddObjectFFRFreducedOrderWithUserFunctions}.
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

//! AUTO: MainObjectFFRFreducedOrder
class MainObjectFFRFreducedOrder: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectFFRFreducedOrder* cObjectFFRFreducedOrder; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectFFRFreducedOrder* visualizationObjectFFRFreducedOrder; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectFFRFreducedOrder()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectFFRFreducedOrder* GetCObjectFFRFreducedOrder() { return cObjectFFRFreducedOrder; }
    //! AUTO: Get const pointer to computational class
    const CObjectFFRFreducedOrder* GetCObjectFFRFreducedOrder() const { return cObjectFFRFreducedOrder; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectFFRFreducedOrder(CObjectFFRFreducedOrder* pCObjectFFRFreducedOrder) { cObjectFFRFreducedOrder = pCObjectFFRFreducedOrder; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectFFRFreducedOrder* GetVisualizationObjectFFRFreducedOrder() { return visualizationObjectFFRFreducedOrder; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectFFRFreducedOrder* GetVisualizationObjectFFRFreducedOrder() const { return visualizationObjectFFRFreducedOrder; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectFFRFreducedOrder(VisualizationObjectFFRFreducedOrder* pVisualizationObjectFFRFreducedOrder) { visualizationObjectFFRFreducedOrder = pVisualizationObjectFFRFreducedOrder; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectFFRFreducedOrder; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectFFRFreducedOrder = (CObjectFFRFreducedOrder*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectFFRFreducedOrder; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectFFRFreducedOrder = (VisualizationObjectFFRFreducedOrder*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "FFRFreducedOrder";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::_None;
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectFFRFreducedOrder->GetParameters().nodeNumbers = EPyUtils::GetArrayNodeIndexSafely(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "massMatrixReduced", cObjectFFRFreducedOrder->GetParameters().massMatrixReduced); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "stiffnessMatrixReduced", cObjectFFRFreducedOrder->GetParameters().stiffnessMatrixReduced); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "dampingMatrixReduced", cObjectFFRFreducedOrder->GetParameters().dampingMatrixReduced); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "forceUserFunction")) { if (EPyUtils::CheckForValidFunction(d["forceUserFunction"])) 
            { cObjectFFRFreducedOrder->GetParameters().forceUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,StdVector,StdVector)>>((py::function)d["forceUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectFFRFreducedOrder->GetParameters().forceUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "massMatrixUserFunction")) { if (EPyUtils::CheckForValidFunction(d["massMatrixUserFunction"])) 
            { cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction = py::cast<std::function<NumpyMatrix(const MainSystem&,Real,StdVector,StdVector)>>((py::function)d["massMatrixUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        if (EPyUtils::DictItemExists(d, "computeFFRFterms")) { cObjectFFRFreducedOrder->GetParameters().computeFFRFterms = py::cast<bool>(d["computeFFRFterms"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetNumpyMatrixSafely(d, "modeBasis", cObjectFFRFreducedOrder->GetParameters().modeBasis); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "outputVariableModeBasis")) { EPyUtils::SetNumpyMatrixSafely(d, "outputVariableModeBasis", cObjectFFRFreducedOrder->GetParameters().outputVariableModeBasis); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "outputVariableTypeModeBasis")) { cObjectFFRFreducedOrder->GetParameters().outputVariableTypeModeBasis = (OutputVariableType)py::cast<Index>(d["outputVariableTypeModeBasis"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetNumpyVectorSafely(d, "referencePositions", cObjectFFRFreducedOrder->GetParameters().referencePositions); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "mPsiTildePsi")) { EPyUtils::SetNumpyMatrixSafely(d, "mPsiTildePsi", cObjectFFRFreducedOrder->GetParameters().mPsiTildePsi); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "mPsiTildePsiTilde")) { EPyUtils::SetNumpyMatrixSafely(d, "mPsiTildePsiTilde", cObjectFFRFreducedOrder->GetParameters().mPsiTildePsiTilde); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "mPhitTPsi")) { EPyUtils::SetNumpyMatrixSafely(d, "mPhitTPsi", cObjectFFRFreducedOrder->GetParameters().mPhitTPsi); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "mPhitTPsiTilde")) { EPyUtils::SetNumpyMatrixSafely(d, "mPhitTPsiTilde", cObjectFFRFreducedOrder->GetParameters().mPhitTPsiTilde); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "mXRefTildePsi")) { EPyUtils::SetNumpyMatrixSafely(d, "mXRefTildePsi", cObjectFFRFreducedOrder->GetParameters().mXRefTildePsi); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "mXRefTildePsiTilde")) { EPyUtils::SetNumpyMatrixSafely(d, "mXRefTildePsiTilde", cObjectFFRFreducedOrder->GetParameters().mXRefTildePsiTilde); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "objectIsInitialized")) { cObjectFFRFreducedOrder->GetObjectIsInitialized() = py::cast<bool>(d["objectIsInitialized"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        cObjectFFRFreducedOrder->GetPhysicsMass() = py::cast<Real>(d["physicsMass"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "physicsInertia", cObjectFFRFreducedOrder->GetPhysicsInertia()); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetVector3DSafely(d, "physicsCenterOfMass", cObjectFFRFreducedOrder->GetPhysicsCenterOfMass()); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "physicsCenterOfMassTilde", cObjectFFRFreducedOrder->GetPhysicsCenterOfMassTilde()); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectFFRFreducedOrder->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectFFRFreducedOrder->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VtriangleMesh")) { EPyUtils::SetNumpyMatrixISafely(d, "VtriangleMesh", visualizationObjectFFRFreducedOrder->GetTriangleMesh()); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VshowNodes")) { visualizationObjectFFRFreducedOrder->GetShowNodes() = py::cast<bool>(d["VshowNodes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["nodeNumbers"] = EPyUtils::GetArrayNodeIndex(cObjectFFRFreducedOrder->GetParameters().nodeNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["massMatrixReduced"] = (PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().massMatrixReduced; //! AUTO: cast variables into python (not needed for standard types) 
        d["stiffnessMatrixReduced"] = (PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().stiffnessMatrixReduced; //! AUTO: cast variables into python (not needed for standard types) 
        d["dampingMatrixReduced"] = (PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().dampingMatrixReduced; //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectFFRFreducedOrder->GetParameters().forceUserFunction)
            {d["forceUserFunction"] = (std::function<StdVector(const MainSystem&,Real,StdVector,StdVector)>)cObjectFFRFreducedOrder->GetParameters().forceUserFunction;}
        else
            {d["forceUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction)
            {d["massMatrixUserFunction"] = (std::function<NumpyMatrix(const MainSystem&,Real,StdVector,StdVector)>)cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction;}
        else
            {d["massMatrixUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["computeFFRFterms"] = (bool)cObjectFFRFreducedOrder->GetParameters().computeFFRFterms; //! AUTO: cast variables into python (not needed for standard types) 
        d["modeBasis"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().modeBasis); //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableModeBasis"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().outputVariableModeBasis); //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableTypeModeBasis"] = (OutputVariableType)cObjectFFRFreducedOrder->GetParameters().outputVariableTypeModeBasis; //! AUTO: cast variables into python (not needed for standard types) 
        d["referencePositions"] = EPyUtils::Vector2NumPy(cObjectFFRFreducedOrder->GetParameters().referencePositions); //! AUTO: cast variables into python (not needed for standard types) 
        d["mPsiTildePsi"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPsiTildePsi); //! AUTO: cast variables into python (not needed for standard types) 
        d["mPsiTildePsiTilde"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPsiTildePsiTilde); //! AUTO: cast variables into python (not needed for standard types) 
        d["mPhitTPsi"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPhitTPsi); //! AUTO: cast variables into python (not needed for standard types) 
        d["mPhitTPsiTilde"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPhitTPsiTilde); //! AUTO: cast variables into python (not needed for standard types) 
        d["mXRefTildePsi"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mXRefTildePsi); //! AUTO: cast variables into python (not needed for standard types) 
        d["mXRefTildePsiTilde"] = EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mXRefTildePsiTilde); //! AUTO: cast variables into python (not needed for standard types) 
        d["objectIsInitialized"] = (bool)cObjectFFRFreducedOrder->GetObjectIsInitialized(); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMass"] = (Real)cObjectFFRFreducedOrder->GetPhysicsMass(); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsInertia"] = EXUmath::Matrix3DToStdArray33(cObjectFFRFreducedOrder->GetPhysicsInertia()); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsCenterOfMass"] = (std::vector<Real>)cObjectFFRFreducedOrder->GetPhysicsCenterOfMass(); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsCenterOfMassTilde"] = EXUmath::Matrix3DToStdArray33(cObjectFFRFreducedOrder->GetPhysicsCenterOfMassTilde()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempUserFunctionForce"] = EPyUtils::Vector2NumPy(cObjectFFRFreducedOrder->GetTempUserFunctionForce()); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectFFRFreducedOrder->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectFFRFreducedOrder->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VtriangleMesh"] = EPyUtils::MatrixI2NumPy(visualizationObjectFFRFreducedOrder->GetTriangleMesh()); //! AUTO: cast variables into python (not needed for standard types) 
        d["VshowNodes"] = (bool)visualizationObjectFFRFreducedOrder->GetShowNodes(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast(EPyUtils::GetArrayNodeIndex(cObjectFFRFreducedOrder->GetParameters().nodeNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("massMatrixReduced") == 0) { return py::cast((PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().massMatrixReduced);} //! AUTO: get parameter
        else if (parameterName.compare("stiffnessMatrixReduced") == 0) { return py::cast((PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().stiffnessMatrixReduced);} //! AUTO: get parameter
        else if (parameterName.compare("dampingMatrixReduced") == 0) { return py::cast((PyMatrixContainer)cObjectFFRFreducedOrder->GetParameters().dampingMatrixReduced);} //! AUTO: get parameter
        else if (parameterName.compare("forceUserFunction") == 0) { return py::cast((std::function<StdVector(const MainSystem&,Real,StdVector,StdVector)>)cObjectFFRFreducedOrder->GetParameters().forceUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("massMatrixUserFunction") == 0) { return py::cast((std::function<NumpyMatrix(const MainSystem&,Real,StdVector,StdVector)>)cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("computeFFRFterms") == 0) { return py::cast((bool)cObjectFFRFreducedOrder->GetParameters().computeFFRFterms);} //! AUTO: get parameter
        else if (parameterName.compare("modeBasis") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().modeBasis);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableModeBasis") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().outputVariableModeBasis);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableTypeModeBasis") == 0) { return py::cast((OutputVariableType)cObjectFFRFreducedOrder->GetParameters().outputVariableTypeModeBasis);} //! AUTO: get parameter
        else if (parameterName.compare("referencePositions") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRFreducedOrder->GetParameters().referencePositions);} //! AUTO: get parameter
        else if (parameterName.compare("objectIsInitialized") == 0) { return py::cast((bool)cObjectFFRFreducedOrder->GetObjectIsInitialized());} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectFFRFreducedOrder->GetPhysicsMass());} //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectFFRFreducedOrder->GetPhysicsInertia()));} //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMass") == 0) { return py::cast((std::vector<Real>)cObjectFFRFreducedOrder->GetPhysicsCenterOfMass());} //! AUTO: get parameter
        else if (parameterName.compare("mPsiTildePsi") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPsiTildePsi);} //! AUTO: get parameter
        else if (parameterName.compare("mPsiTildePsiTilde") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPsiTildePsiTilde);} //! AUTO: get parameter
        else if (parameterName.compare("mPhitTPsi") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPhitTPsi);} //! AUTO: get parameter
        else if (parameterName.compare("mPhitTPsiTilde") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mPhitTPsiTilde);} //! AUTO: get parameter
        else if (parameterName.compare("mXRefTildePsi") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mXRefTildePsi);} //! AUTO: get parameter
        else if (parameterName.compare("mXRefTildePsiTilde") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRFreducedOrder->GetParameters().mXRefTildePsiTilde);} //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMassTilde") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectFFRFreducedOrder->GetPhysicsCenterOfMassTilde()));} //! AUTO: get parameter
        else if (parameterName.compare("tempUserFunctionForce") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRFreducedOrder->GetTempUserFunctionForce());} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectFFRFreducedOrder->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectFFRFreducedOrder->GetColor());} //! AUTO: get parameter
        else if (parameterName.compare("VtriangleMesh") == 0) { return EPyUtils::MatrixI2NumPy(visualizationObjectFFRFreducedOrder->GetTriangleMesh());} //! AUTO: get parameter
        else if (parameterName.compare("VshowNodes") == 0) { return py::cast((bool)visualizationObjectFFRFreducedOrder->GetShowNodes());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectFFRFreducedOrder::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectFFRFreducedOrder->GetParameters().nodeNumbers = EPyUtils::GetArrayNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("massMatrixReduced") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRFreducedOrder->GetParameters().massMatrixReduced); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("stiffnessMatrixReduced") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRFreducedOrder->GetParameters().stiffnessMatrixReduced); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dampingMatrixReduced") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRFreducedOrder->GetParameters().dampingMatrixReduced); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("forceUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectFFRFreducedOrder->GetParameters().forceUserFunction = py::cast<std::function<StdVector(const MainSystem&,Real,StdVector,StdVector)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("massMatrixUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectFFRFreducedOrder->GetParameters().massMatrixUserFunction = py::cast<std::function<NumpyMatrix(const MainSystem&,Real,StdVector,StdVector)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("computeFFRFterms") == 0) { cObjectFFRFreducedOrder->GetParameters().computeFFRFterms = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("modeBasis") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().modeBasis); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableModeBasis") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().outputVariableModeBasis); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableTypeModeBasis") == 0) { cObjectFFRFreducedOrder->GetParameters().outputVariableTypeModeBasis = py::cast<OutputVariableType>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referencePositions") == 0) { EPyUtils::SetNumpyVectorSafely(value, cObjectFFRFreducedOrder->GetParameters().referencePositions); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("objectIsInitialized") == 0) { cObjectFFRFreducedOrder->GetObjectIsInitialized() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { cObjectFFRFreducedOrder->GetPhysicsMass() = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectFFRFreducedOrder->GetPhysicsInertia()); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMass") == 0) { EPyUtils::SetVector3DSafely(value, cObjectFFRFreducedOrder->GetPhysicsCenterOfMass()); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mPsiTildePsi") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mPsiTildePsi); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mPsiTildePsiTilde") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mPsiTildePsiTilde); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mPhitTPsi") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mPhitTPsi); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mPhitTPsiTilde") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mPhitTPsiTilde); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mXRefTildePsi") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mXRefTildePsi); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("mXRefTildePsiTilde") == 0) { EPyUtils::SetNumpyMatrixSafely(value, cObjectFFRFreducedOrder->GetParameters().mXRefTildePsiTilde); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMassTilde") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectFFRFreducedOrder->GetPhysicsCenterOfMassTilde()); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectFFRFreducedOrder->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectFFRFreducedOrder->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VtriangleMesh") == 0) { EPyUtils::SetNumpyMatrixISafely(value, visualizationObjectFFRFreducedOrder->GetTriangleMesh()); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VshowNodes") == 0) { visualizationObjectFFRFreducedOrder->GetShowNodes() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectFFRFreducedOrder::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
