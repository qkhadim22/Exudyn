/** ***********************************************************************************************
* @class        CNodeRigidBodyRotVecLGParameters
* @brief        Parameter class for CNodeRigidBodyRotVecLG
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-02-05  00:03:34 (last modfied)
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


//! AUTO: Parameters for class CNodeRigidBodyRotVecLGParameters
class CNodeRigidBodyRotVecLGParameters // AUTO: 
{
public: // AUTO: 
    Vector6D referenceCoordinates;                //!< AUTO: reference coordinates (x-pos,y-pos,z-pos and rotation vector) of node ==> e.g. ref. coordinates for finite elements or reference position of rigid body (e.g. for definition of joints)
    //! AUTO: default constructor with parameter initialization
    CNodeRigidBodyRotVecLGParameters()
    {
        referenceCoordinates = Vector6D({0.,0.,0., 0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        CNodeRigidBodyRotVecLG
* @brief        A 3D rigid body node based on rotation vector and Lie group methods for rigid bodies or beams; the node has 3 displacement coordinates (displacements of center of mass - COM: \f$[u_x,u_y,u_z]\f$) and three rotation coordinates (rotation vector \f$\mathbf{v} = [v_x,v_y,v_z]^T = \varphi \cdot \mathbf{n}\f$, defining the rotation axis \f$\mathbf{n}\f$ and the angle \f$\varphi\f$ for rotations around x,y, and z-axis); the velocity coordinates are based on the translational (global) velocity and the (local/body-fixed) angular velocity vector; this node can only be integrated using special Lie group integrators; NOTE that this node has a singularity if the rotation is zero or multiple of \f$2\pi\f$.
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

//! AUTO: CNodeRigidBodyRotVecLG
class CNodeRigidBodyRotVecLG: public CNodeRigidBody // AUTO: 
{
protected: // AUTO: 
    static const Index nRotationCoordinates = 3;
    static const Index nDisplacementCoordinates = 3;
    CNodeRigidBodyRotVecLGParameters parameters; //! AUTO: contains all parameters for CNodeRigidBodyRotVecLG

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CNodeRigidBodyRotVecLGParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CNodeRigidBodyRotVecLGParameters& GetParameters() const { return parameters; }

    //! AUTO:  return number of second order diff. eq. coordinates
    virtual Index GetNumberOfODE2Coordinates() const override
    {
        return 6;
    }

    //! AUTO:  return number of displacement coordinates
    virtual Index GetNumberOfDisplacementCoordinates() const override
    {
        return nDisplacementCoordinates;
    }

    //! AUTO:  return number of rotation coordinates
    virtual Index GetNumberOfRotationCoordinates() const override
    {
        return nRotationCoordinates;
    }

    //! AUTO:  return node type (for node treatment in computation)
    virtual Node::Type GetType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::RigidBody + Node::RotationRotationVector + Node::RotationLieGroup);
    }

    //! AUTO:  return node group, which is special because of algebraic equations
    virtual CNodeGroup GetNodeGroup() const override
    {
        return CNodeGroup::ODE2variables;
    }

    //! AUTO:  return configuration dependent position of node; returns always a 3D Vector
    virtual Vector3D GetPosition(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent velocity of node; returns always a 3D Vector
    virtual Vector3D GetVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Vector
    virtual Matrix3D GetRotationMatrix(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent local (=body-fixed) angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocityLocal(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return internally stored reference coordinates of node
    virtual LinkedDataVector GetReferenceCoordinateVector() const override
    {
        return parameters.referenceCoordinates;
    }

    //! AUTO:  provide according output variable in "value"; used e.g. for postprocessing and sensors
    virtual void GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const override;

    //! AUTO:  Compute vector to of 4 Euler Parameters from reference and configuration coordinates
    virtual ConstSizeVector<maxRotationCoordinates> GetRotationParameters(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute vector to time derivative of 4 Euler Parameters in given configuration
    virtual LinkedDataVector GetRotationParameters_t(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
    virtual void GetG(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute local G matrix for given configuration
    virtual void GetGlocal(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
    virtual void GetG_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute local G matrix for given configuration
    virtual void GetGlocal_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::RotationMatrix +
            (Index)OutputVariableType::Rotation +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::AngularVelocity +
            (Index)OutputVariableType::AngularVelocityLocal +
            (Index)OutputVariableType::Coordinates +
            (Index)OutputVariableType::Coordinates_t );
    }

};

