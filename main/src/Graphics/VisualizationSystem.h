/** ***********************************************************************************************
* @class        VisualizationSystem
* @brief		
* @details		Details:
 				- a visualization system, containing data and functions for visualization
*
* @author		Gerstmayr Johannes
* @date			2019-05-24 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
* *** Example code ***
*
************************************************************************************************ */
#ifndef VISUALIZATIONSYSTEM__H
#define VISUALIZATIONSYSTEM__H

//#include "autogenerated/SimulationSettings.h" 
//
//#include "Linalg/Vector.h"				//includes ReleaseAssert.h, ... and BasicFunctions.h
//#include "Linalg/SlimVector.h" 
//#include "Linalg/ConstSizeVector.h"		//includes Vector.h
//#include "Linalg/ResizableVector.h"		//includes Vector.h
//#include "Utilities/ResizableArray.h"	//includes SlimArray.h and BasicFunctions.h
//
//#include "System/CSystem.h"  //REMOVE: temporary, because lateron coupling only via visualization system!!! but include Vector
//#include "Autogenerated/VisualizationSettings.h" 
//#include "Graphics/GraphicsData.h"
////#include "Graphics/VisualizationSystemBase.h"
//#include "Graphics/VisualizationSystemData.h"
//


//class CSystem;
class VisualizationSystemContainer; //for UpdateGraphics(...) function
class MainSystem; //for backlink

class VisualizationSystem //: public VisualizationSystemBase
{
public: //declared as public for direct access via pybind
	virtual ~VisualizationSystem() {} //added for correct deletion of derived classes
	VisualizationSystemData vSystemData;//!< data structure containing the visualization items
    GraphicsData graphicsData;			//!< data to be processed by OpenGL renderer
	//VisualizationSettings settings;		//!< general settings for visualization
	PostProcessData* postProcessData;	//!< link to postProcessData of CSystem ==> communication between the two threads
	CSystemData* systemData;			//!< REMOVE: this is a temporary access, before visualization objects are introduced

	//RenderState renderState;		//!< Data linked to state variables of the OpenGL engine (e.g. zoom, transformation matrices, ...)
	const float contourPlotFlag = -2.f;	//!< this is the value of transparency used to identify contour plot values in GraphicsData items
	//static constexpr float contourPlotFlag = -2.f;	//!< needs C++17 and is therefore avoided ...

private:
	//additional data for user functions
	MainSystem* mainSystemUF;						//!< REMOVE: this is a temporary access to mainSystem for user functions
	bool renderingActive;				//!< flag, which indicates that this system shall be drawn (true) or hidden (false)
	//accessible through mainSytemUF: VisualizationSettings* visualizationSettingsUF; //!< REMOVE: set when setting postProcessData->requestUserFunctionDrawing; this is a temporary access to visualizationSettings for user functions

public:
	GraphicsData& GetGraphicsData() { return graphicsData; }
	const GraphicsData& GetGraphicsData() const { return graphicsData; }

	VisualizationSystemData& GetVisualizationSystemData() { return vSystemData; }
	const VisualizationSystemData& GetVisualizationSystemData() const { return vSystemData; }

	//VisualizationSettings& GetVisualizationSettings() { return settings; }
    //const VisualizationSettings& GetVisualizationSettings() const { return settings; }

	static const float GetContourPlotFlag() { return -2.f; }
	static const SignedIndex GetContourPlotNormFlag() { return -1; }
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//SYSTEM FUNCTIONS

	//! links to systemData of cSystem (should be REMOVED)
	void LinkToSystemData(CSystemData* systemDataInit);
	//! back link to mainSystem for user functions; (should be REMOVED)
	void LinkToMainSystem(MainSystem* mainSystemInit);
	//! link to postProcessData, which is the communication way of graphics to the computational system
	void LinkPostProcessData(PostProcessData* postProcessDataInit);
	//! set rendering true/false
	void ActivateRendering(bool flag) { renderingActive = flag; }
	//! check if rendering shall be done
	bool IsRenderingActive() { return renderingActive; }

	//! return ID of mainSystem in MainSystemContainer
	Index GetSystemID() const;

	//! get backlink to MainSystem, needed for user functions and other specialized graphics operations
	MainSystem* GetMainSystemBacklink();

	//! reset all visualization functions for new system (but keep render engine linked)
	void Reset();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//GRAPHICS FUNCTIONS

	//! OpenGL renderer calls UpdateGraphicsData (different thread) to update graphics data; update is only done, if current state has higher counter than already existing state
	virtual void UpdateGraphicsData(VisualizationSystemContainer& visualizationSystemContainer);

	//! Renderer reports to CSystem that simulation shall be interrupted
	virtual void StopSimulation();		

	//! any multi-line text message from computation to be shown in renderer (e.g. time, solver, ...)
	virtual std::string GetComputationMessage(bool solverInformation = true, bool solutionInformation = true, bool solverTime = true);

	//! if the system has changed or loaded, compute maximum box of all items and reset scene to the maximum box
	//virtual void UpdateMaximumSceneCoordinates();

	virtual void Print(std::ostream& os) const
	{
		os << "VisualizationCSystem:\n";
		os << "  VisualizationSystemData = \n" << vSystemData << "\n";
		os << "  graphicsData = \n" << graphicsData << "\n";
		os << "\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const VisualizationSystem& object)
	{
		object.Print(os);
		return os;
	}



};

#endif
