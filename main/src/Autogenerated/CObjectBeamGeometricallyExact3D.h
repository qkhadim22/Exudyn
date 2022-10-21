/** ***********************************************************************************************
* @class        CObjectBeamGeometricallyExact3DParameters
* @brief        Parameter class for CObjectBeamGeometricallyExact3D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2022-10-17  16:28:11 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef COBJECTBEAMGEOMETRICALLYEXACT3DPARAMETERS__H
#define COBJECTBEAMGEOMETRICALLYEXACT3DPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include "Main/StructuralElementsDataStructures.h"
#include "Autogenerated/BeamSectionGeometry.h"

//! AUTO: Parameters for class CObjectBeamGeometricallyExact3DParameters
class CObjectBeamGeometricallyExact3DParameters // AUTO: 
{
public: // AUTO: 
    Index2 nodeNumbers;                           //!< AUTO: two node numbers for beam element
    Real physicsLength;                           //!< AUTO:  [SI:m] reference length of beam; such that the total volume (e.g. for volume load) gives \f$\rho A L\f$; must be positive
    Real physicsMassPerLength;                    //!< AUTO:  [SI:kg/m] mass per length of beam; this data is used internally for computation
    Matrix3D physicsCrossSectionInertia;          //!< AUTO:  [SI:kg m] cross section mass moment of inertia tensor; this data is used internally for computation
    Vector3D physicsTorsionalBendingStiffness;    //!< AUTO:  [SI:Nm\f$^2\f$] bending and torsional stiffness vector;
    Vector3D physicsAxialShearStiffness;          //!< AUTO:  [SI:N] axial and shear stiffness;
    //! AUTO: default constructor with parameter initialization
    CObjectBeamGeometricallyExact3DParameters()
    {
        nodeNumbers = Index2({EXUstd::InvalidIndex, EXUstd::InvalidIndex});
        physicsLength = 0.;
        physicsMassPerLength = 0.;
        physicsCrossSectionInertia = EXUmath::zeroMatrix3D;
        physicsTorsionalBendingStiffness = 0.;
        physicsAxialShearStiffness = 0.;
    };
};


/** ***********************************************************************************************
* @class        CObjectBeamGeometricallyExact3D
* @brief        OBJECT UNDER CONSTRUCTION: A 3D geometrically exact beam finite element, currently using two 3D rigid body nodes. The localPosition \f$x\f$ of the beam ranges from \f$-L/2\f$ (at node 0) to \f$L/2\f$ (at node 1). The axial coordinate is \f$x\f$ (first coordinate) and the cross section is spanned by local \f$y\f$/\f$z\f$ axes.
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

//! AUTO: CObjectBeamGeometricallyExact3D
class CObjectBeamGeometricallyExact3D: public CObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectBeamGeometricallyExact3DParameters parameters; //! AUTO: contains all parameters for CObjectBeamGeometricallyExact3D

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CObjectBeamGeometricallyExact3DParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CObjectBeamGeometricallyExact3DParameters& GetParameters() const { return parameters; }

    //! AUTO:  Computational function: compute mass matrix
    virtual void ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const override;

    //! AUTO:  Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to 'ode2Lhs'
    virtual void ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const override;

    //! AUTO:  Computational function: compute jacobian (dense or sparse mode, see parent CObject function)
    virtual void ComputeJacobianODE2_ODE2(EXUmath::MatrixContainer& jacobianODE2, JacobianTemp& temp, Real factorODE2, Real factorODE2_t, Index objectNumber, const ArrayIndex& ltg) const override;

    //! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags
    virtual JacobianType::Type GetAvailableJacobians() const override
    {
        return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t + JacobianType::ODE2_ODE2_function + JacobianType::ODE2_ODE2_t_function);
    }

    //! AUTO:  Flags to determine, which access (forces, moments, connectors, ...) to object are possible
    virtual AccessFunctionType GetAccessFunctionTypes() const override;

    //! AUTO:  provide Jacobian at localPosition in 'value' according to object access
    virtual void GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const override;

    //! AUTO:  provide according output variable in 'value'
    virtual void GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value, Index objectNumber) const override;

    //! AUTO:  return the (global) position of 'localPosition' according to configuration type
    virtual Vector3D GetPosition(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return the (global) displacement of 'localPosition' according to configuration type
    virtual Vector3D GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return the (global) velocity of 'localPosition' according to configuration type
    virtual Vector3D GetVelocity(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Matrix, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
    virtual Matrix3D GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
    virtual Vector3D GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Get global node number (with local node index); needed for every object ==> does local mapping
    virtual Index GetNodeNumber(Index localIndex) const override
    {
        release_assert(localIndex <= 1);
        return parameters.nodeNumbers[localIndex];
    }

    //! AUTO:  number of nodes; needed for every object
    virtual Index GetNumberOfNodes() const override
    {
        return 2;
    }

    //! AUTO:  number of \hac{ODE2} coordinates; needed for object?
    virtual Index GetODE2Size() const override;

    //! AUTO:  Get type of object, e.g. to categorize and distinguish during assembly and computation
    virtual CObjectType GetType() const override
    {
        return (CObjectType)((Index)CObjectType::Body + (Index)CObjectType::MultiNoded);
    }

    //! AUTO:  return true if object has time and coordinate independent (=constant) mass matrix
    virtual bool HasConstantMassMatrix() const override
    {
        return false;
    }

    //! AUTO:  This flag is reset upon change of parameters; says that mass matrix (future: other pre-computed values) need to be recomputed
    virtual void ParametersHaveChanged() override
    {
        ;
    }

    //! AUTO:  map two vectors q0 and q1 at nodes 0 and 1 onto shape vectors SV; if SV=SV(x), it returns Vector of interpolated coordinates at certain position x
    Vector3D MapVectors(const Vector2D& SV, const Vector3D& q0, const Vector3D& q1) const;

    //! AUTO:  Get frame as homogeneous transformation at some localPosition[0], using correct interpolation according to Lie groups
    HomogeneousTransformation GetLocalPositionFrame(const Vector3D& localPosition, ConfigurationType configuration) const;

    //! AUTO:  get compressed shape function vector \f$\Sm_v\f$, depending local position \f$x \in [0,L]\f$
    Vector2D ComputeShapeFunctions(Real x) const;

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::Rotation +
            (Index)OutputVariableType::StrainLocal +
            (Index)OutputVariableType::CurvatureLocal );
    }

};



#endif //#ifdef include once...
