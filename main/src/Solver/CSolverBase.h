/** ***********************************************************************************************
* @class		CSolverBase
* @brief		This is the new solver base class for static and time integration solution
* @details		Details:
* 				- base classes for computational solver (as compared to main solver, which interacts with Python, etc.)
*               - contains basic solver structures (Evaluation functions, jacobians, initialization, link to linear solvers, 
*				  solver statistics and output, solution files, Newton, nonlinear iteration, ...)
*
*				Solver process (what happens in the SolveSystem(...) function)
*				1) solver-specific tasks (e.g. initialize specific parameters), set solverName
*				2) InitializeSolver(): initialize files, solver data, initial values, ...
*				3) specific output to solver file (needs initialization to start output)
*				4) SolveSteps(...): iteratively solve
*				4) FinalizeSolver(...): close files, etc.
*
* @author		Gerstmayr Johannes
* @date			2019-12-11 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
************************************************************************************************ */
#ifndef CSOLVERBASE__H
#define CSOLVERBASE__H

#include "Autogenerated/SimulationSettings.h" 
#include <fstream>

#include "Linalg/LinearSolver.h" //for GeneralMatrixEXUdense
#include "Solver/CSolver.h"

class CSystem;

//! general solver for different computation tasks: static, dynamic/explicit, dynamic/implicit
class CSolverBase
{
public: //made public for simpler access via pybind; nevertheless, C++ functions should not directly access these structures from outside!!!
	CSolverTimer timer;
	SolverLocalData data;
	SolverIterationData it;
	SolverConvergenceData conv;
	SolverOutputData output;
	SolverFileData file;
	NewtonSettings newton; //copy of timeInt or staticSolver (depending on solver)
	DiscontinuousSettings discontinuous; //copy of discontinuous from timeInt or staticSolver
public:
	CSolverBase()
	{
		InitCSolverBase();
	}
	virtual ~CSolverBase() {} //added for correct deletion of derived classes

	void InitCSolverBase()
	{
		//data re-initialization procedure (total clean up)
		//general.InitializeData();
		output.InitializeData();
		file = SolverFileData(); //reset files, for safety

		//timer not initialized, because it needs information if timings are required
	}

	//! return true, if static solver; needs to be overwritten in derived class
	//! +++++ TO BE IMPLEMENTED IN DERIVED CLASS +++++
	virtual bool IsStaticSolver() const { CHECKandTHROWstring("CSolverBase::illegal call"); return false; }

	//now it.endTime is available
	////! get current simulation end time of solver (may be adapted during long simulations...)
	//virtual Real GetEndTime(const SimulationSettings& simulationSettings) const 
	//{
	//	if (IsStaticSolver()) { return simulationSettings.staticSolver.loadStepDuration + simulationSettings.staticSolver.GetLoadStepStart; }
	//	else { return simulationSettings.timeIntegration.endTime; }
	//}

	//! reduce step size (0 .. with given step size, 1..normal, 2..severe problems); return true, if reduction was successful
	virtual bool ReduceStepSize(CSystem& computationalSystem, const SimulationSettings& simulationSettings, 
		Index severity, Real suggestedStepSize = -1.)
	{
		return false;
	}

	//! increase step size if convergence is good; if suggestedStepSize == -1, a solver-specific factor will be used
	virtual void IncreaseStepSize(CSystem& computationalSystem, const SimulationSettings& simulationSettings,
		Real suggestedStepSize = -1.) {	}

	//! for static solver, this is a factor in interval [0,1]; MUST be overwritten
	virtual Real ComputeLoadFactor(const SimulationSettings& simulationSettings) const { CHECKandTHROWstring("CSolverBase::illegal call to ComputeLoadFactor"); return 0; }

	//! get solver name - needed for output file header and visualization window
	//! +++++ TO BE IMPLEMENTED IN DERIVED CLASS +++++
	virtual const STDstring GetSolverName() const { CHECKandTHROWstring("CSolverBase::illegal call to GetSolverName"); return ""; }

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//initialize routines
	
	//! initialize solverSpecific,data,it,conv; set/compute initial conditions (solver-specific!); initialize output files
	virtual bool InitializeSolver(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! pre-initialize for solver specific tasks; called at beginning of InitializeSolver, right after Solver data reset
	virtual void PreInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { }

	//! post-initialize for solver specific tasks; called at the end of InitializeSolver
	virtual void PostInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { }

	//! initialize output files; called from InitializeSolver()
	virtual void InitializeSolverOutput(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! initialize dense/sparse computation modes
	virtual bool InitializeSolverPreChecks(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! initialize all data,it,conv; called from InitializeSolver()
	virtual void InitializeSolverData(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! set/compute initial conditions (solver-specific!); called from InitializeSolver()
	virtual void InitializeSolverInitialConditions(CSystem& computationalSystem, const SimulationSettings& simulationSettings);



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//solver routines routines
	//! solve CSystem: InitializeSolver, SolveSteps, FinalizeSolver
	virtual bool SolveSystem(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! write concluding information (timer statistics, messages) and close files
	virtual void FinalizeSolver(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! main solver part: calls multiple InitializeStep(...)/DiscontinuousIteration(...)/FinishStep(...); do step reduction if necessary; return true if success, false else
	virtual bool SolveSteps(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! update currentTime (and load factor); MUST be overwritten in special solver class
	virtual void UpdateCurrentTime(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { CHECKandTHROWstring("CSolverBase::illegal call to UpdateCurrentTime"); }

	//! initialize static step / time step; python-functions; do some outputs, checks, etc.
	virtual void InitializeStep(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! finish static step / time step; write output of results to file
	virtual void FinishStep(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! perform discontinuousIteration for static step / time step; CALLS ComputeNewtonResidual 
	virtual bool DiscontinuousIteration(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//NEWTON:
	//! perform Newton method for given solver method
	virtual bool Newton(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! compute residual for Newton method (e.g. static or time step); store result vector in systemResidual and return scalar residual
	//! +++++ TO BE IMPLEMENTED IN DERIVED CLASS +++++
	virtual Real ComputeNewtonResidual(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { CHECKandTHROWstring("CSolverBase::illegal call to ComputeNewtonResidual"); return 0; }

	//! compute update for currentState from newtonSolution (decrement from residual and jacobian); for initial=true, this is the initial update with data.newtonSolution=0
	//! +++++ TO BE IMPLEMENTED IN DERIVED CLASS +++++
	virtual void ComputeNewtonUpdate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, bool initial = false) { CHECKandTHROWstring("CSolverBase::illegal call to ComputeNewtonUpdate"); }

	//! compute jacobian for newton method of given solver method; store result in systemJacobian
	//! +++++ TO BE IMPLEMENTED IN DERIVED CLASS +++++
	virtual void ComputeNewtonJacobian(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { CHECKandTHROWstring("CSolverBase::illegal call"); }

	//! function immediately called after successfull Newton(...) method; needs not to be implemented; used in generalized alpha to make special step update after Newton
	virtual void FinalizeNewton(CSystem& computationalSystem, const SimulationSettings& simulationSettings) { };

	//! function called after successfull Newton(...) / FinalizeNewton(...) method; calls CSystem PostNewton(...) and returns discontinuous error and may set stepSizeReccomendation
	virtual Real PostNewton(CSystem& computationalSystem, const SimulationSettings& simulationSettings);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! output helper functions:
	//! write unique file header, depending on static/dynamic simulation
	virtual void WriteSolutionFileHeader(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! write unique coordinates solution file
	virtual void WriteCoordinatesToFile(const CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! write unique sensor file header, depending on static/dynamic simulation
	virtual void WriteSensorsFileHeader(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! write unique sensor solution file
	virtual void WriteSensorsToFile(const CSystem& computationalSystem, const SimulationSettings& simulationSettings);
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! compute simulation end time (depends on static or time integration solver)
	virtual Real GetSimulationEndTime(const SimulationSettings& simulationSettings) const
	{
		if (IsStaticSolver()) { return simulationSettings.staticSolver.loadStepDuration; }
		else { return simulationSettings.timeIntegration.endTime; }
	}

	//! do some idle operations for update of waiting Python queues (multithreaded) or for graphics update (single-threaded)
	virtual void DoIdleOperations(CSystem& computationalSystem);

	//! return true, if file or console output is at or above the given level
	virtual bool IsVerboseCheck(Index level) const
	{ 
		return EXUstd::Maximum(output.verboseMode, output.verboseModeFile) >= level; 
	}

	//! write to console and/or file in case of level
	virtual void VerboseWrite(Index level, const STDstring& str);

	friend std::ostream& operator<<(std::ostream& os, const CSolverBase& object) {
		os << "CSolverBase";
		return os;
	}
};


#endif
