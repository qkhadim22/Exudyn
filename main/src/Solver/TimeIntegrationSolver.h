/** ***********************************************************************************************
* @brief		Old solver header files; replaced by CSolverBase and derived classes
* @details		Details:
 				- a solver for time integration
                - in future, this file will collect time integration solvers
*
* @author		Gerstmayr Johannes
* @date			2018-05-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
************************************************************************************************ */
#ifndef TIMEINTEGRATIONSOLVER__H
#define TIMEINTEGRATIONSOLVER__H

//#include "Utilities/ReleaseAssert.h"
//#include <initializer_list>
//#include "Utilities/BasicDefinitions.h" //defines Real
//#include "Utilities/ObjectContainer.h" 
//
//#include "Linalg/Vector.h" 
#include "Linalg/LinearSolver.h" //for GeneralMatrixEXUdense

#include "Autogenerated/SimulationSettings.h" 
#include <fstream>
//#include <omp.h> //for num_threads
#include "Solver/CSolver.h" //for solvertimer and AssignParallelizationParameters --> could be decoupled

class CSystem;



class SolverRK1
{
public:
//    SolverRK1() {};

    //! solve CSystem with time integration, return true if success, false else
	bool SolveSystem(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! solver template for dense/sparse computation (cannot be called from pybind module directly; why?):
	template<class TGeneralMatrix = GeneralMatrixEXUdense>
	bool SolveSystemTemplate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, TGeneralMatrix& massMatrix);

    friend std::ostream& operator<<(std::ostream& os, const SolverRK1& object) {
        os << "SolverRK1";
        return os;
    }
};

class SolverStatic
{
public:
	//! solve CSystem with static solver, return true if success, false else
	bool SolveSystem(CSystem& computationalSystem, const SimulationSettings& simulationSettings);// , TGeneralMatrix& jacobian);

	//! solver template for dense/sparse computation (cannot be called from pybind module directly; why?):
	template<class TGeneralMatrix = GeneralMatrixEXUdense>
	bool SolveSystemTemplate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, TGeneralMatrix& systemJacobian);

	friend std::ostream& operator<<(std::ostream& os, const SolverStatic& object) {
		os << "SolverStatic";
		return os;
	}
};


class SolverGeneralizedAlpha
{
public:
	//! solve CSystem with static solver, return true if success, false else
	bool SolveSystem(CSystem& computationalSystem, const SimulationSettings& simulationSettings);

	//! solver template for dense/sparse computation (cannot be called from pybind module directly; why?):
	template<class TGeneralMatrix = GeneralMatrixEXUdense>
	bool SolveSystemTemplate(CSystem& computationalSystem, const SimulationSettings& simulationSettings, TGeneralMatrix& systemJacobian);

	friend std::ostream& operator<<(std::ostream& os, const SolverGeneralizedAlpha& object) {
		os << "SolverGeneralizedAlpha";
		return os;
	}
};


//! timer for computational times of various functions
class SolverTimer
{
public:
	//! reset all timings before measurement
	void Reset()
	{
		total = 0;
		factorization = 0;
		newtonIncrement = 0;
		integrationFormula = 0;
		ODE2RHS = 0;
		AERHS = 0;
		totalJacobian = 0;
		jacobianODE2 = 0;
		jacobianODE2_t = 0;
		jacobianAE = 0;
		massMatrix = 0;
		reactionForces = 0;
		postNewton = 0;
		writeSolution = 0;
		overhead = 0;
		python = 0;
		visualization = 0;
	}
	
	//! compute sum of all timer parts (should be nearly the total time)
	Real Sum() const 
	{ 
		return factorization + newtonIncrement + integrationFormula + ODE2RHS + AERHS + totalJacobian + massMatrix + reactionForces +
			postNewton + writeSolution + overhead + python + visualization;
	}

	void Print()
	{
		pout << "====================\nCPU-time statistics:\n";
		if (total != 0.) // avoid division by zero ...
		{
			Real sum = total / 100.;
			pout << "  total time= " << total << " seconds\n";
			pout << "  factorization = " << factorization / sum << "%\n";
			pout << "  newtonIncrement = " << newtonIncrement / sum << "%\n";
			pout << "  integrationFormula = " << integrationFormula / sum << "%\n";
			pout << "  ODE2RHS       = " << ODE2RHS / sum << "%\n";
			pout << "  AERHS         = " << AERHS / sum << "%\n";
			pout << "  totalJacobian = " << totalJacobian / sum << "%\n";
			pout << "  jacobianODE2_t= " << jacobianODE2_t / sum << "%\n";
			pout << "  jacobianODE2  = " << jacobianODE2 / sum << "%\n";
			pout << "  jacobianAE    = " << jacobianAE / sum << "%\n";
			pout << "  massMatrix    = " << massMatrix / sum << "%\n";
			pout << "  reactionForces= " << reactionForces / sum << "%\n";
			pout << "  postNewtonStep= " << postNewton / sum << "%\n";
			pout << "  writeSolution = " << writeSolution / sum << "%\n";
			pout << "  overhead      = " << overhead / sum << "%\n";
			pout << "  python        = " << python / sum << "%\n";
			pout << "  visualization = " << visualization / sum << "%\n";
			pout << "  === sum = " << Sum() / sum << "%\n\n"; //sum of all subitems
		}
		else
		{
			pout << "  total CPU time is zero; no statistics available!\n\n";
		}
	}

	Real total;				//total time measured between start and end of computation (static/dynamics)
	Real factorization;		//solve or inverse
	Real newtonIncrement;	//Jac^-1 * RHS; backsubstitution
	Real integrationFormula;//time spent for evaluation of integration formulas
	Real ODE2RHS;
	Real AERHS;
	Real totalJacobian;		//all jacobian computations
	Real jacobianODE2;		//jacobian of ODE2 equations (not counted in sum)
	Real jacobianODE2_t;	//jacobian of ODE2 equations (not counted in sum)
	Real jacobianAE;		//jacobian of algebraic equations (not counted in sum)
	Real massMatrix;		//mass matrix evaluation
	Real reactionForces;	//CqT * lambda
	Real postNewton;		//post newton step
	Real writeSolution;		//time for writing solution
	Real overhead;			//overhead, such as initialization, copying and some matrix-vector multiplication
	Real python;			//time spent for python functions
	Real visualization;		//time spent for visualization at computation thread

};

//! helper functions for Exudyn solver
namespace HSolver {

	void WriteCoordinatesToFile(Real& lastSolutionWritten, const SolutionSettings& solutionSettings,
		std::ofstream& file, const CSystem& computationalSystem);

	void WriteSolutionFileHeader(CSystem& computationalSystem, const SimulationSettings& simulationSettings, std::ofstream& file,
		const STDstring& solverName, bool isStatic);
}

#endif
