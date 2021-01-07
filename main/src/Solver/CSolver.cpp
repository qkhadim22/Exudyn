/** ***********************************************************************************************
* @brief		Implentation for computational solver CSolver structure classes
* @details		Details:
* 				- base classes for computational solver (as compared to main solver, which interacts with Python, etc.)
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

#include <pybind11/pybind11.h> //for integrated python connectivity (==>put functionality into separate file ...!!!)
#include <pybind11/eval.h>

#include "System/versionCpp.h"
#include "Linalg/BasicLinalg.h" //for Resizable Vector
#include "Main/CSystem.h"
#include "Autogenerated/CMarkerBodyPosition.h"
#include "Solver/CSolverBase.h" 
#include <fstream>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//CSolverTimer
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! compute sum of all timer parts (should be nearly the total time)
Real CSolverTimer::Sum() const
{
	return factorization + newtonIncrement + integrationFormula + ODE2RHS + AERHS + totalJacobian + massMatrix + reactionForces +
		postNewton + writeSolution + overhead + python + visualization;
}

void CSolverTimer::StartTimer(Real& value)
{
	if (useTimer) { value -= EXUstd::GetTimeInSeconds(); }
}
void CSolverTimer::StopTimer(Real& value)
{
	if (useTimer) { value += EXUstd::GetTimeInSeconds(); }
}

STDstring CSolverTimer::ToString() const
{
	if (!useTimer) { return ""; }

	std::ostringstream ostr;
	ostr.precision(3); //reduced precision for nicer output...
	ostr << "====================\nCPU-time statistics:\n";
	if (Sum() != 0.) // avoid division by zero ...
	{
		Real sum = Sum() / 100.;
		ostr << "  total time   = " << total << " seconds\n";
		ostr << "  measured time= " << Sum() << " seconds (=" << 100.*Sum() / total << "%) \n";
		ostr << "  factorization     = " << factorization / sum << "%\n";
		ostr << "  newtonIncrement   = " << newtonIncrement / sum << "%\n";
		ostr << "  integrationFormula= " << integrationFormula / sum << "%\n";
		ostr << "  ODE2RHS           = " << ODE2RHS / sum << "%\n";
		ostr << "  AERHS             = " << AERHS / sum << "%\n";
		ostr << "  totalJacobian     = " << totalJacobian / sum << "%\n";
		ostr << "  jacobianODE2_t    = " << jacobianODE2_t / sum << "%\n";
		ostr << "  jacobianODE2      = " << jacobianODE2 / sum << "%\n";
		ostr << "  jacobianAE        = " << jacobianAE / sum << "%\n";
		ostr << "  massMatrix        = " << massMatrix / sum << "%\n";
		ostr << "  reactionForces    = " << reactionForces / sum << "%\n";
		ostr << "  postNewtonStep    = " << postNewton / sum << "%\n";
		ostr << "  writeSolution     = " << writeSolution / sum << "%\n";
		ostr << "  overhead          = " << overhead / sum << "%\n";
		ostr << "  python            = " << python / sum << "%\n";
		ostr << "  visualization/user= " << visualization / sum << "%\n";
	}
	else
	{
		ostr << "  total CPU time is zero or not measured; no statistics available!\n\n";
	}
	return ostr.str();
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SolverLocalData
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! if desired, temporary data is cleaned up to safe memory
void SolverLocalData::CleanUpMemory()
{
	nODE2 = 0;
	nODE1 = 0;
	nAE = 0;
	nData = 0;
	nSys = 0;
	startAE = 0;

	//there is no other way to free memory than to assign to a new matrix
	systemJacobianDense = GeneralMatrixEXUdense();
	systemMassMatrixDense = GeneralMatrixEXUdense();
	jacobianAEdense = GeneralMatrixEXUdense();

	systemJacobianSparse.Reset();
	systemMassMatrixSparse.Reset();
	jacobianAEsparse.Reset();

	//+++++++++++++++++++++++++++
	systemResidual.Reset();
	newtonSolution.Reset();
	tempODE2.Reset();
	temp2ODE2.Reset();
	tempODE2F0.Reset();
	tempODE2F1.Reset();

	//startOfDiscIteration.Reset();
	startOfStepStateAAlgorithmic.Reset();

	aAlgorithmic.Reset();
}

//! function links system matrices to according dense/sparse versions
void SolverLocalData::SetLinearSolverType(LinearSolverType linearSolverType)
{
	if (linearSolverType == LinearSolverType::EXUdense)
	{
		systemJacobian = &systemJacobianDense;
		systemMassMatrix = &systemMassMatrixDense;
		jacobianAE = &jacobianAEdense;
	}
	else
	{
		systemJacobian = &systemJacobianSparse;
		systemMassMatrix = &systemMassMatrixSparse;
		jacobianAE = &jacobianAEsparse;
	}
}


//! convert iteration statistics to string
std::string SolverIterationData::ToString() const
{
	std::ostringstream ostr;
	ostr << "Solver iteration statistics:\n";
	ostr << "total number of steps:        " << currentStepIndex - 1 << "\n"; //last step not counted (first step=initialization)
	ostr << "total number of Newton iterations: " << newtonStepsCount << "\n";
	ostr << "total number of Newton Jacobians:  " << newtonJacobiCount << "\n";
	if (rejectedModifiedNewtonSteps)
	{
		ostr << "rejected modified Newton steps:      " << rejectedModifiedNewtonSteps << "\n";
	}

	return ostr.str();
}

