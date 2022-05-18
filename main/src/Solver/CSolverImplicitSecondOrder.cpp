/** ***********************************************************************************************
* @brief		Implentation for CSolverImplicitSecondOrderTimeInt
*
* @author		Gerstmayr Johannes
* @date			2021-01-27 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
************************************************************************************************ */

#include <pybind11/pybind11.h> //for integrated python connectivity (==>put functionality into separate file ...!!!)
#include <pybind11/eval.h>
#include <fstream>

#include "Linalg/BasicLinalg.h" //for Resizable Vector
#include "Main/CSystem.h"
#include "Autogenerated/CMarkerBodyPosition.h"
//#include "Solver/CSolverBase.h" 
#include "Solver/CSolverImplicitSecondOrder.h" 


namespace py = pybind11;	//for py::object


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++   IMPLICIT SECOND ORDER SOLVER   ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! reduce step size (1..normal, 2..severe problems); return true, if reduction was successful
bool CSolverImplicitSecondOrderTimeInt::ReduceStepSize(CSystem& computationalSystem, const SimulationSettings& simulationSettings, 
	Index severity, Real suggestedStepSize)
{
	//it.currentTime is the only important value to be updated in order to reset the step time:
	it.currentTime = computationalSystem.GetSystemData().GetCData().currentState.time;

	if (it.currentStepSize > it.minStepSize)
	{
		if (it.recommendedStepSize != -1.)
		{
			it.currentStepSize = EXUstd::Minimum(0.75*it.currentStepSize, it.recommendedStepSize); //0.75: enforce some reduction in step size, because otherwise iterations could get stuck
		}
		else
		{
			it.currentStepSize *= simulationSettings.timeIntegration.adaptiveStepDecrease;
			//OLD: it.currentStepSize *= 0.5;
		}

		it.currentStepSize = EXUstd::Maximum(it.minStepSize, it.currentStepSize);
		return true;
	}

	return false;
}

//! increase step size if convergence is good; if suggestedStepSize == -1, a solver-specific factor will be used
void CSolverImplicitSecondOrderTimeInt::IncreaseStepSize(CSystem& computationalSystem, const SimulationSettings& simulationSettings,
	Real suggestedStepSize)
{
	if ((it.currentStepSize != it.maxStepSize))
	{
		it.currentStepSize = EXUstd::Minimum(it.maxStepSize, simulationSettings.timeIntegration.adaptiveStepIncrease*it.currentStepSize);

		if ((IsVerboseCheck(1) && (output.stepInformation & StepInfo::stepIncreaseInfo)) || IsVerboseCheck(2))
		{
			if (it.currentTime != it.endTime)
			{
				STDstring str = STDstring("  Solve steps: adaptive increase to step size = ") + EXUstd::ToString(it.currentStepSize) + " due to fast convergence";
				if (IsStaticSolver()) { str += ", load factor = " + EXUstd::ToString(computationalSystem.GetSolverData().loadFactor); }
				else { str += ", time = " + EXUstd::ToString(it.currentTime); }
				VerboseWrite(1, str + "\n");
			}
		}
	}
}

//! set/compute initial conditions (solver-specific!); called from InitializeSolver()
void CSolverImplicitSecondOrderTimeInt::InitializeSolverInitialConditions(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	//pout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++\nTHIS IS THE NEW (EXPERIMENTAL) second order solver!\n+++++++++++++++++++++++++++++++++++++++++++++++++++\n";

	//call base class for general tasks
	CSolverBase::InitializeSolverInitialConditions(computationalSystem, simulationSettings); //set currentState = initialState


	Vector& solutionODE2_tt = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_tt;
	//Vector& initialODE1_t = computationalSystem.GetSystemData().GetCData().initialState.ODE1Coords_t;
	Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;

	bool computeInitialAccelerations = simulationSettings.timeIntegration.generalizedAlpha.computeInitialAccelerations;
	//+++++++++++++++++++++++++++++++++++++++++
	//compute initial values for accelerations:
	//to be fully consistent the initial accelerations must be computed with Lagrange multipliers

	if (computeInitialAccelerations)
	{
		//initial accelerations can be computed, if the system is written in acceleration form
		//qODE1_t terms currently not considered! (could be considered via ODE1 jacobian?)
		//[ M    C_q^T][q_tt  ]   [             -ODE2RHS                 ]   [0]
		//|    0      ||qODE1 | + |               qODE1                  | = |qODE1_init|
		//[C_q   0    ][lambda]   [ C_tt + 2(C_q)_t*q_t + (C_q*q_t)_q*q_t]   [0]
		//==> constraints need to be transformed to acceleration level
		//==> velocity constraints Ct give: d(Ct)/dt = Ct_t + Ct_qt * q_tt ==> only Ct_t is necessary (if existing) ==> set Ct_qt * q_tt = 0
		//for now, the following terms are neglegted: C_tt = 0, 2(C_q)_t*q_t = 0
		//   ==> no explicit dependence of constraints w.r.t. time, or at least requiring this dependence to be zero at initialization
		//[ M    C_q^T][q_tt  ]   [   -ODE2RHS     ]   [0]
		//|           ||      | + |                | = | |
		//[C_q   0    ][lambda]   [ (C_q*q_t)_q*q_t]   [0]
		//the term (C_q*q_t)_q*q_t is computed numerically and only exists, if initial velocities are != 0

		//size of systemJacobian already set!
		data.systemJacobian->SetAllZero(); //entries are not set to zero inside jacobian computation!
		Real factorAE_ODE2 = 1.;			//for position level constraints: depends, if index reduction is used
		Real factorAE_ODE2_t = 1.;			//for velocity constraints ==> C_qt*q_tt term from velocity level, if C=C(q_t)
		//bool fillIntoSystemMatrix = true;
		bool velocityLevel = false;

		//+++++++++++++++++++++++++++++
		//Jacobian of algebraic equations
		//Real factorAE = EXUstd::Square(stepSize) * newmarkBeta; //Index3
		computationalSystem.JacobianAE(data.tempCompData, newton, *(data.systemJacobian), factorAE_ODE2, factorAE_ODE2_t, velocityLevel);// , fillIntoSystemMatrix);

		//Mass matrix - may also be directly filled into data.systemJacobian?
		if (!hasConstantMassMatrix) //not yet set at this point, done in PostInitializeSolverSpecific; default=false
		{
			data.systemMassMatrix->SetAllZero();
			computationalSystem.ComputeMassMatrix(data.tempCompData, *(data.systemMassMatrix));
		}
		data.systemJacobian->AddSubmatrix(*(data.systemMassMatrix));

		//add unit matrix for ODE1 components (nothing to be solved for)
		ConstSizeVector<1> unit(1, 1.);
		LinkedDataVector linkedUnit(unit);

		for (Index i = 0; i < data.nODE1; i++)
		{
			data.systemJacobian->AddColumnVector(data.nODE2 + i, linkedUnit, data.nODE2 + i); //add single components
		}

		//compute RHS
		Vector systemRHS(data.nSys);
		systemRHS.SetAll(0.);
		LinkedDataVector ode2RHS(systemRHS, 0, data.nODE2);
		LinkedDataVector ode1RHS(systemRHS, data.nODE2, data.nODE1);
		LinkedDataVector aeRHS(systemRHS, data.startAE, data.nAE);

		//compute system RHS for initial conditions:
		computationalSystem.ComputeSystemODE2RHS(data.tempCompDataArray, ode2RHS);
		ode1RHS.SetAll(0); //nothing to be computed for ODE1
		aeRHS.SetAll(0);

		//.... for velocity level constraints: add dg/dq*\dot q and ODE1 term in future

		if (IsVerbose(3)) { Verbose(3, "    initial accelerations update Jacobian: Jac    = " + EXUstd::ToString(*(data.systemJacobian)) + "\n"); }

		if (computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_t.GetL2Norm() > 1e-10)
		{
			if (simulationSettings.linearSolverType != LinearSolverType::EXUdense)
			{
				PyWarning("Generalized alpha: initial accelerations due to initial velocities can only be computed in dense matrix mode!");
			}
			else
			{

				Index rowOffset = 0;
				Index columnOffset = 0;
				Real factor = -1.; //(C_q*q_t)_q*q_t put on RHS
				Vector& vInitial = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_t; //=initialState! for consistency here, only currentState is used
				data.jacobianAE->SetNumberOfRowsAndColumns(data.nAE, data.nODE2);
				computationalSystem.ComputeConstraintJacobianDerivative(data.tempCompData, newton.numericalDifferentiation, 
					data.tempODE2F0, data.tempODE2F1, vInitial, *(data.jacobianAE), factor, rowOffset, columnOffset);

				Vector Cqv2(data.nAE);
				data.jacobianAE->MultMatrixVector(vInitial, Cqv2);
				aeRHS += Cqv2;

				if (IsVerbose(3)) { Verbose(3, STDstring("vInitial = ") + EXUstd::ToString(vInitial) + "\n"); }
				if (IsVerbose(3)) { Verbose(3, STDstring("Cqv2     = ") + EXUstd::ToString(Cqv2) + "\n"); }
			}
		}

		data.systemJacobian->FinalizeMatrix();

		bool ignoreRedundantEquations = false;
		Index redundantEqStart = 0;
		if (simulationSettings.linearSolverSettings.ignoreSingularJacobian || simulationSettings.linearSolverSettings.ignoreRedundantConstraints)
		{
			ignoreRedundantEquations = true;
			if (simulationSettings.linearSolverSettings.ignoreRedundantConstraints && !simulationSettings.linearSolverSettings.ignoreSingularJacobian)
			{
				redundantEqStart = data.startAE;
			}
		}

		Index factorizeOutput = data.systemJacobian->FactorizeNew(ignoreRedundantEquations, redundantEqStart);

		if (factorizeOutput != -1)
		{
			std::string s = "CSolverImplicitSecondOrder::InitializeSolverInitialConditions: System Jacobian seems to be singular / not invertible!";
			if (factorizeOutput < data.systemJacobian->NumberOfRows())
			{
				s += "The solver returned the causing system equation number (coordinate number) = " + EXUstd::ToString(factorizeOutput) + "\n";
			}
			s += "\n******************************************\n";
			s += "\nWARNING: using zero initial accelerations!\n";
			s += "\n******************************************\n";
			PyWarning(s);
			solutionODE2_tt.SetAll(0.);
		}
		else
		{
			Vector systemInitialValues(data.nSys);
			systemInitialValues.SetAll(0.);
			LinkedDataVector ode2InitialValues(systemInitialValues, 0, data.nODE2);
			data.systemJacobian->Solve(systemRHS, systemInitialValues);
			solutionODE2_tt.CopyFrom(ode2InitialValues); //initial lagrange multipliers are not considered! Should we?
		}
		//Vector& solutionODE2_tt = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_tt;
		//Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;

		////these vectors are used in time stepping from previous step
		//solutionODE2_tt = data.u_tt0;
		//solutionAE.SetAll(0.); 
		//data.aAlgorithmic.CopyFrom(solutionODE2_tt); //this may be improved for spectralRadius != 0, see Martin Arnold's paper

	}
	else
	{
		solutionODE2_tt.SetAll(0.);
	}

	//these vectors are used in time stepping from previous step
	solutionAE.SetAll(0.);
	data.aAlgorithmic.CopyFrom(solutionODE2_tt);

	if (IsVerbose(3)) { Verbose(3, STDstring("initial accelerations = ") + EXUstd::ToString(solutionODE2_tt) + "\n"); }

}

//! initialize static step / time step: do some outputs, checks, etc.
void CSolverImplicitSecondOrderTimeInt::UpdateCurrentTime(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (!it.adaptiveStep)
	{
		it.currentTime = it.currentStepIndex * it.currentStepSize + it.startTime; //use this to avoid round-off errors in time 
	}
	else
	{
		if (it.currentTime + it.currentStepSize > it.endTime)
		{
			it.currentStepSize = it.endTime - it.currentTime;
		}
		it.currentTime += it.currentStepSize;
	}

}

//! initialize things at the very beginning of initialize
void CSolverImplicitSecondOrderTimeInt::PreInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	const TimeIntegrationSettings& timeint = simulationSettings.timeIntegration;

	hasConstantMassMatrix = false;

	//do solver-specific tasks and initialization:
	newmarkBeta = timeint.generalizedAlpha.newmarkBeta; //0.25 ... trapezoidal rule
	newmarkGamma = timeint.generalizedAlpha.newmarkGamma; //0.5  ... trapezoidal rule
	factJacAlgorithmic = 1.; //factor for jacobian in case of generalized-alpha due to algorithmic accelerations

	if (!timeint.generalizedAlpha.useNewmark) //use generalized-alpha
	{
		spectralRadius = timeint.generalizedAlpha.spectralRadius;
		alphaM = (2 * spectralRadius - 1) / (spectralRadius + 1);	//alphaM = 0.5 for rho=1
		alphaF = spectralRadius / (spectralRadius + 1);				//alphaF = 0.5 for rho=1
		newmarkGamma = 0.5 + alphaF - alphaM;
		newmarkBeta = 0.25*EXUstd::Square(newmarkGamma + 0.5);
		factJacAlgorithmic = (1. - alphaF) / (1. - alphaM);
	}
	else
	{
		spectralRadius = 1; //no damping, implicit trapezoidal rule
		alphaM = (2 * spectralRadius - 1) / (spectralRadius + 1);	//alphaM = 0.5 for rho=1
		alphaF = spectralRadius / (spectralRadius + 1);				//alphaF = 0.5 for rho=1
		factJacAlgorithmic = (1. - alphaF) / (1. - alphaM);
	}
	useScaling = true; 


	//std::cout << "spectralRadius=" << spectralRadius << "\n";
	//std::cout << "alphaM=" << alphaM << "\n";
	//std::cout << "alphaF=" << alphaF << "\n";
	//std::cout << "newmarkGamma=" << newmarkGamma << "\n";
	//std::cout << "newmarkBeta=" << newmarkBeta << "\n";

	//useIndex2Constraints = timeint.generalizedAlpha.useIndex2Constraints; //==> now directly linked to simulationSettings;
}

//! post-initialize for solver specific tasks; called at the end of InitializeSolver
void CSolverImplicitSecondOrderTimeInt::PostInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	//now implemented
	//if (data.nODE1 != 0) { SysError("SolverImplicitSecondOrder cannot solve first order differential equations (ODE1) for now", file.solverFile); }

	if (IsVerbose(2))
	{
		if (simulationSettings.timeIntegration.generalizedAlpha.useNewmark)
		{
			Verbose(2, STDstring("  NEWMARK: beta=") + EXUstd::ToString(newmarkBeta) + ", gamma=" + EXUstd::ToString(newmarkGamma) + "\n");
		}
		else
		{
			Verbose(2, STDstring("  Generalized-alpha: spectralRadius=") + EXUstd::ToString(spectralRadius) +
				", alphaM=" + EXUstd::ToString(alphaM) + ", alphaF=" + EXUstd::ToString(alphaF) +
				", beta=" + EXUstd::ToString(newmarkBeta) + ", gamma=" + EXUstd::ToString(newmarkGamma) +
				", factJacA=" + EXUstd::ToString(factJacAlgorithmic) + "\n");
		}
	}
	//++++++++++++++++++++++++++++++++++++++
	//initialize special for implicit solver:
	hasConstantMassMatrix = computationalSystem.HasConstantMassMatrix(); //this should not be called all time, because it costs some time

	if (IsVerbose(3))
	{
		Verbose(3, STDstring("  has constant mass matrix=") + EXUstd::ToString(hasConstantMassMatrix) + "\n");
	}

	if (!simulationSettings.timeIntegration.reuseConstantMassMatrix) { hasConstantMassMatrix = false; }

	if (hasConstantMassMatrix)
	{
		//compute mass matrix
		STARTTIMER(timer.massMatrix);
		data.systemMassMatrix->SetAllZero();
		computationalSystem.ComputeMassMatrix(data.tempCompData, *(data.systemMassMatrix));
		STOPTIMER(timer.massMatrix);
	}
}


//! compute residual for Newton method (e.g. static or time step)
//! INPUT: 
//!       - end of last step: [u0, u_t0, u_tt0, aAlgorithmic0 [, lambda0]]; (lambda0 not used in integration scheme)
//!       - end of this step: [solutionODE2_tt]
//! INTERMEDIATE:
//!       - aAlgorithmic is the acceleration in the integration scheme (==solutionODE2_tt for pure Newmark)
//!       - solutionODE2 and solutionODE2_t are computed from integration formulas, based on [u0, u_t0, u_tt0, aAlgorithmic0] and aAlgorithmic
//! OUTPUT: data.systemResidual is updated
Real CSolverImplicitSecondOrderTimeInt::ComputeNewtonResidual(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	//std::cout << "ComputeNewtonResidual\n";
	//const TimeIntegrationSettings& timeint = simulationSettings.timeIntegration;

	LinkedDataVector ode2Residual(data.systemResidual, 0, data.nODE2); //link ODE2 coordinates
	LinkedDataVector ode1Residual(data.systemResidual, data.nODE2, data.nODE1); //link ODE1 coordinates
	LinkedDataVector aeResidual(data.systemResidual, data.startAE, data.nAE); //link ae coordinates

	////link current system vectors for ODE2
	//Vector& solutionODE2 = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords;
	//Vector& solutionODE2_t = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_t;
	Vector& solutionODE2_tt = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_tt;
	//Vector& solutionODE1 = computationalSystem.GetSystemData().GetCData().currentState.ODE1Coords;
	Vector& solutionODE1_t = computationalSystem.GetSystemData().GetCData().currentState.ODE1Coords_t;
	//Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//ODE2:
	//now compute the new residual with updated system vectors:
	if (!hasConstantMassMatrix)
	{
		STARTTIMER(timer.massMatrix);
		data.systemMassMatrix->SetAllZero();
		computationalSystem.ComputeMassMatrix(data.tempCompData, *(data.systemMassMatrix));
		STOPTIMER(timer.massMatrix);
	}

	STARTTIMER(timer.ODE2RHS);
	computationalSystem.ComputeSystemODE2RHS(data.tempCompDataArray, data.tempODE2); //tempODE2 contains ODE2 RHS (linear case: tempODE2 = F_applied - K*u - D*v)
	STOPTIMER(timer.ODE2RHS);
	//systemMassMatrix.FinalizeMatrix(); //MultMatrixVector is faster? if directly applied to triplets ...
	data.systemMassMatrix->MultMatrixVector(solutionODE2_tt, ode2Residual);
	//EXUmath::MultMatrixVector(systemMassMatrix, solutionODE2_tt, ode2Residual);
	ode2Residual -= data.tempODE2; //systemResidual contains residual (linear: residual = M*a + K*u+D*v-F )

	Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;
	//compute CqT*lambda:
	STARTTIMER(timer.reactionForces);
	computationalSystem.ComputeODE2ProjectedReactionForces(data.tempCompData, solutionAE, ode2Residual); //add the forces directly!
	STOPTIMER(timer.reactionForces);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//ODE1:
	STARTTIMER(timer.ODE1RHS);
	computationalSystem.ComputeSystemODE1RHS(data.tempCompData, ode1Residual); //tempODE1 contains ODE1 RHS (linear case: tempODE1 = F_ODE1(q_ODE1, t) )
	STOPTIMER(timer.ODE1RHS);
	ode1Residual -= solutionODE1_t; //ode1Residual contains F_ODE1(q_ODE1, t) - q_ODE1_t (=0); different from ODE2 !!!

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//AE:
	STARTTIMER(timer.AERHS);
	computationalSystem.ComputeAlgebraicEquations(data.tempCompData, aeResidual, simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints);
	STOPTIMER(timer.AERHS);

	Real scalarResidual = data.systemResidual.GetL2Norm(); //compute residual BEFORE scaling, otherwise residual shrinks with h^2!!!
	if (useScaling)
	{
		Real scalODE2 = newmarkBeta * EXUstd::Square(it.currentStepSize);
		ode2Residual *= scalODE2; //r_bar = D_L*r
	}
	return scalarResidual / conv.errorCoordinateFactor;
}

void CSolverImplicitSecondOrderTimeInt::ComputeNewtonUpdate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, bool initial)
{
	//std::cout << "ComputeNewtonUpdate(" << initial << ")\n";

	STARTTIMER(timer.integrationFormula);
	Vector& solutionODE2 = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords;
	Vector& solutionODE2_t = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_t;
	Vector& solutionODE2_tt = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_tt;
	Vector& solutionODE1 = computationalSystem.GetSystemData().GetCData().currentState.ODE1Coords;
	Vector& solutionODE1_t = computationalSystem.GetSystemData().GetCData().currentState.ODE1Coords_t;
	Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;

	LinkedDataVector newtonSolutionODE2(data.newtonSolution, 0, data.nODE2); //temporary subvector for ODE2 solution
	LinkedDataVector newtonSolutionODE1(data.newtonSolution, data.nODE2, data.nODE1); //temporary subvector for ODE1 solution
	LinkedDataVector newtonSolutionAE(data.newtonSolution, data.startAE, data.nAE); //temporary subvector for ODE2 solution


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//following Arnold and Bruls, Multibody System Dynamics, 2007, Algorithm 1
	//ODE1 part is integrated with trapezoidal rule
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Real betaPrime = (1. - alphaM) / (EXUstd::Square(it.currentStepSize)*newmarkBeta*(1.-alphaF));
	Real gammaPrime = newmarkGamma / (it.currentStepSize*newmarkBeta);


	if (initial)
	{
		//solutionODE2_tt must contain initial accelerations !!!
		//solutionAE.SetAll(0.); //already done in Newton
		solutionODE2.MultAdd(it.currentStepSize, solutionODE2_t);
		if (simulationSettings.timeIntegration.generalizedAlpha.useNewmark)
		{
			//predictor for displacements and velocities:
			solutionODE2.MultAdd(EXUstd::Square(it.currentStepSize)*(0.5 - newmarkBeta), solutionODE2_tt); 
			solutionODE2_t.MultAdd(it.currentStepSize*(1. - newmarkGamma), solutionODE2_tt);
		}
		else
		{
			//std::cout << "sol_tt=" << solutionODE2_tt << "\n";
			//std::cout << "aa=" << data.aAlgorithmic << "\n";
			solutionODE2.MultAdd(EXUstd::Square(it.currentStepSize)*(0.5 - newmarkBeta), data.aAlgorithmic); //aAlgorithmic equals the startOfStepState
			solutionODE2_t.MultAdd(it.currentStepSize*(1. - newmarkGamma), data.aAlgorithmic);

			data.aAlgorithmic *= -alphaM / (1 - alphaM);
			data.aAlgorithmic.MultAdd(alphaF / (1 - alphaM), solutionODE2_tt);

			solutionODE2.MultAdd(EXUstd::Square(it.currentStepSize)*newmarkBeta, data.aAlgorithmic);
			solutionODE2_t.MultAdd(it.currentStepSize*newmarkGamma, data.aAlgorithmic);
		}
		//as the algorithmic accelerations are computed, now set back acc for Newton
		solutionODE2_tt.SetAll(0.); //initial guess for Newton
		//std::cout << "sol0   =" << solutionODE2 << "\n";
		//std::cout << "sol0_t =" << solutionODE2_t << "\n";
		//std::cout << "sol0_tt=" << solutionODE2_tt << "\n";

		solutionODE1.MultAdd(it.currentStepSize*0.5, solutionODE1_t); //initial part for trapezoidal rule: uT=u0 + h/2*v0 + h/2*vT
		solutionODE1_t.SetAll(0); //start with zero, same as in ODE2 accelerations

	}
	else
	{
		//now only add increments
		solutionODE2 -= newtonSolutionODE2; //Delta q in Arnold/Bruls is (-1)*Delta q here
		solutionODE2_t.MultAdd(-gammaPrime, newtonSolutionODE2); //Delta q in Arnold/Bruls is (-1)*Delta q here
		solutionODE2_tt.MultAdd(-betaPrime, newtonSolutionODE2); //Delta q in Arnold/Bruls is (-1)*Delta q here
		if (useScaling)
		{
			Real scalAE = 1. / (newmarkBeta * EXUstd::Square(it.currentStepSize));
			solutionAE.MultAdd(-scalAE, newtonSolutionAE); //Delta lambda in Arnold/Bruls is (-1)*Delta lambda here
		}
		else
		{
			solutionAE -= newtonSolutionAE; //Delta lambda in Arnold/Bruls is (-1)*Delta lambda here
		}

		solutionODE1 -= newtonSolutionODE1;
		solutionODE1_t.MultAdd(-2. / it.currentStepSize, newtonSolutionODE1); //2/h =^= gammaPrime = gamma/(h*beta) = 0.5/(h*0.25)

		//std::cout << "newtonODE2=" << newtonSolutionODE2 << "\n";
		//std::cout << "newtonODE1=" << newtonSolutionODE1 << "\n";
		//std::cout << "solODE2   =" << solutionODE2 << "\n";
		//std::cout << "solODE2_t =" << solutionODE2_t << "\n";
		//std::cout << "solODE2_tt=" << solutionODE2_tt << "\n";
		//std::cout << "solODE1   =" << solutionODE1 << "\n";
		//std::cout << "solODE1_t =" << solutionODE1_t << "\n";

	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	STOPTIMER(timer.integrationFormula);
}

//! compute jacobian for newton method of given solver method
void CSolverImplicitSecondOrderTimeInt::ComputeNewtonJacobian(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	//std::cout << "ComputeNewtonJacobian\n";
	STARTTIMER(timer.totalJacobian);
	data.systemJacobian->SetAllZero(); //entries are not set to zero inside jacobian computation!

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//following Arnold and Bruls, Multibody System Dynamics, 2007, Algorithm 1
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	Real scalODE2 = 1; //scaling factors for ODE2 part
	Real scalAE = 1;   //scaling factors for AE part
	if (useScaling)
	{
		scalODE2 = newmarkBeta * EXUstd::Square(it.currentStepSize);
		scalAE = 1 / scalODE2;
	}

	Real betaPrime = (1. - alphaM) / (EXUstd::Square(it.currentStepSize)*newmarkBeta*(1. - alphaF));
	Real gammaPrime = newmarkGamma / (it.currentStepSize*newmarkBeta);

	STARTTIMER(timer.massMatrix);
	//M*betaPrime:
	//mass matrix is not updated for jacobian ...! //add a flag?
	data.systemJacobian->AddSubmatrix(*(data.systemMassMatrix)); //systemMassMatrix used from initial step or from previous step; 
	data.systemJacobian->MultiplyWithFactor(betaPrime * scalODE2);
	STOPTIMER(timer.massMatrix);

	//+++++++++++++++++++++++++++++
	//compute jacobian (w.r.t. U ==> also add V)
	STARTTIMER(timer.jacobianODE2);
	//Tangent stiffness
	//K-Matrix; has no factor
	computationalSystem.JacobianODE2RHS(data.tempCompDataArray, newton.numericalDifferentiation, *(data.systemJacobian), -1. * scalODE2, -gammaPrime * scalODE2); //RHS ==> -K
	STOPTIMER(timer.jacobianODE2);

	////+++++++++++++++++++++++++++++
	////compute jacobian (w.r.t. U ==> also add V)
	//STARTTIMER(timer.jacobianODE2);
	////Tangent stiffness
	////K-Matrix; has no factor
	//computationalSystem.NumericalJacobianODE2RHS(data.tempCompData, newton.numericalDifferentiation, 
	//	data.tempODE2F0, data.tempODE2F1, *(data.systemJacobian), -1. * scalODE2); //RHS ==> -K
	//STOPTIMER(timer.jacobianODE2);

	////+++++++++++++++++++++++++++++
	////'Damping' and gyroscopic terms
	//STARTTIMER(timer.jacobianODE2_t);
	////Arnold/Bruls: C_t*gammaPrime
	//computationalSystem.NumericalJacobianODE2RHS_t(data.tempCompData, newton.numericalDifferentiation, data.tempODE2F0, 
	//	data.tempODE2F1, *(data.systemJacobian), -gammaPrime * scalODE2); //d(ODE2)/dq_t for damping terms; //RHS ==> -D
	//STOPTIMER(timer.jacobianODE2_t);

	//+++++++++++++++++++++++++++++
	//compute ODE1 jacobian
	STARTTIMER(timer.jacobianODE1);
	//Tangent stiffness
	//K-Matrix; has no factor
	computationalSystem.NumericalJacobianODE1RHS(data.tempCompData, newton.numericalDifferentiation,
		data.tempODE1F0, data.tempODE1F1, *(data.systemJacobian), 1.); //RHS ==> K_ODE1; no scaling for now
	data.systemJacobian->AddDiagonalMatrix(-2./ it.currentStepSize, data.nODE1, data.nODE2, data.nODE2); //for qODE1_t part
	STOPTIMER(timer.jacobianODE1);

	//+++++++++++++++++++++++++++++
	//Jacobian of algebraic euqations
	//Real factorAE = EXUstd::Square(stepSize) * newmarkBeta; //Index3
	Real factorAE_ODE2 = 1;		//for position level constraints; dC/dq
	Real factorAE_ODE2_t = gammaPrime;  //for velocity constraints ==> same for index 2 and index 3: dC_t/d(Delta q) = dC_t/dq_t * dq_t/dq = dC/dq*gammaPrime
	Real factorODE2_AE = scalAE * scalODE2;
	Real factorAE_AE = scalAE;
	if (simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints) 
	{ 
		factorAE_ODE2 = gammaPrime; //Index2: dC_t/d(Delta q) = dC_t/dq_t * dq_t/dq = dC/dq*gammaPrime
	} 

	STARTTIMER(timer.jacobianAE);
	//add jacobian algebraic equations part to system jacobian:
	computationalSystem.JacobianAE(data.tempCompData, newton, *(data.systemJacobian), factorAE_ODE2, factorAE_ODE2_t, false, factorODE2_AE, factorAE_AE);
	STOPTIMER(timer.jacobianAE);

	computationalSystem.GetSolverData().signalJacobianUpdate = false; //as jacobian has been computed, no further update is necessary

	if (IsVerbose(3)) { Verbose(3, "    update Jacobian: Jac    = " + EXUstd::ToString(*(data.systemJacobian)) + "\n"); }
	else if (IsVerbose(2)) { Verbose(2, "    update Jacobian\n"); }

	STOPTIMER(timer.totalJacobian);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

//! compute jacobian for newton method of given solver method
void CSolverImplicitSecondOrderTimeInt::FinalizeNewton(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	const Vector& solutionODE2_tt = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords_tt;
	data.aAlgorithmic.MultAdd((1. - alphaF) / (1. - alphaM), solutionODE2_tt); //for next step
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//USER FUNCTIONS override:

void CSolverImplicitSecondOrderTimeIntUserFunction::UpdateCurrentTime(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionUpdateCurrentTime == 0) { CSolverImplicitSecondOrderTimeInt::UpdateCurrentTime(computationalSystem, simulationSettings); }
	else { userFunctionUpdateCurrentTime(*mainSolver, *mainSystem, simulationSettings); }
}

void CSolverImplicitSecondOrderTimeIntUserFunction::InitializeStep(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionInitializeStep == 0) { CSolverImplicitSecondOrderTimeInt::InitializeStep(computationalSystem, simulationSettings); }
	else { userFunctionInitializeStep(*mainSolver, *mainSystem, simulationSettings); }
}

void CSolverImplicitSecondOrderTimeIntUserFunction::FinishStep(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionFinishStep == 0) { CSolverImplicitSecondOrderTimeInt::FinishStep(computationalSystem, simulationSettings); }
	else { userFunctionFinishStep(*mainSolver, *mainSystem, simulationSettings); }
}

bool CSolverImplicitSecondOrderTimeIntUserFunction::DiscontinuousIteration(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionDiscontinuousIteration == 0) { return CSolverImplicitSecondOrderTimeInt::DiscontinuousIteration(computationalSystem, simulationSettings); }
	else { return userFunctionDiscontinuousIteration(*mainSolver, *mainSystem, simulationSettings); }
}

bool CSolverImplicitSecondOrderTimeIntUserFunction::Newton(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionNewton == 0) { return CSolverImplicitSecondOrderTimeInt::Newton(computationalSystem, simulationSettings); }
	else { return userFunctionNewton(*mainSolver, *mainSystem, simulationSettings); }
}

Real CSolverImplicitSecondOrderTimeIntUserFunction::ComputeNewtonResidual(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionComputeNewtonResidual == 0) { return CSolverImplicitSecondOrderTimeInt::ComputeNewtonResidual(computationalSystem, simulationSettings); }
	else { return userFunctionComputeNewtonResidual(*mainSolver, *mainSystem, simulationSettings); }
}

void CSolverImplicitSecondOrderTimeIntUserFunction::ComputeNewtonUpdate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, bool initial)
{
	if (userFunctionComputeNewtonUpdate == 0) { CSolverImplicitSecondOrderTimeInt::ComputeNewtonUpdate(computationalSystem, simulationSettings, initial); }
	else { userFunctionComputeNewtonUpdate(*mainSolver, *mainSystem, simulationSettings, initial); }
}

void CSolverImplicitSecondOrderTimeIntUserFunction::ComputeNewtonJacobian(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionComputeNewtonJacobian == 0) { CSolverImplicitSecondOrderTimeInt::ComputeNewtonJacobian(computationalSystem, simulationSettings); }
	else { userFunctionComputeNewtonJacobian(*mainSolver, *mainSystem, simulationSettings); }
}

Real CSolverImplicitSecondOrderTimeIntUserFunction::PostNewton(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	if (userFunctionComputeNewtonResidual == 0) { return CSolverImplicitSecondOrderTimeInt::PostNewton(computationalSystem, simulationSettings); }
	else { return userFunctionPostNewton(*mainSolver, *mainSystem, simulationSettings); }
}

