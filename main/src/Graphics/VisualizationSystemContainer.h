/** ***********************************************************************************************
* @class        VisualizationSystemContainer
* @brief		
* @details		Details:
 				- a container for visualization systems; handles all communication with OpenGL renderer (glfwclient)
*
* @author		Gerstmayr Johannes
* @date			2019-06-11 (generated)
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
#ifndef VISUALIZATIONSYSTEMCONTAINER__H
#define VISUALIZATIONSYSTEMCONTAINER__H

#include "Linalg/BasicLinalg.h"		//includes all Matrix and Vector classes

#include "Main/CSystem.h"  //REMOVE: temporary, because lateron coupling only via visualization system!!! but include Vector
#include "Autogenerated/VisualizationSettings.h" 
#include "Graphics/GraphicsData.h"
#include "Graphics/VisualizationSystemContainerBase.h"
#include "Graphics/VisualizationSystemData.h"
#include "Graphics/VisualizationSystem.h"


//MOVE to separate .h / .cpp file:

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/functional.h>
namespace py = pybind11;            //! namespace 'py' used throughout in code

//! object graphics data for ground objects, rigid bodies and mass points
class BodyGraphicsData
{
public:
	ResizableArray<GLLine> glLines;				//!< lines to be displayed
	ResizableArray<GLCircleXY> glCirclesXY;		//!< circles to be displayed
	ResizableArray<GLText> glTexts;				//!< texts to be displayed
	ResizableArray<GLTriangle> glTriangles;	//!< triangles to be displayed

	//! clear lists (keep allocated data) and deallocate data of texts, allocated with new
	void FlushData()
	{
		for (auto item : glTexts)
		{
			delete[] item.text; //must be allocated by procedure which generates the texts;
		}
		glLines.SetNumberOfItems(0);
		//glPoints.SetNumberOfItems(0);
		glCirclesXY.SetNumberOfItems(0);
		glTexts.SetNumberOfItems(0);
		glTriangles.SetNumberOfItems(0);
	}

	~BodyGraphicsData()
	{
		FlushData(); //delete allocated strings !
	}
};

//moved to EXUvis, VisualizationPrimitives.h //! copy bodyGraphicsData (of body) into global graphicsData (of system); add position offset and transform with rotation matrix
//void AddBodyGraphicsData(const BodyGraphicsData& bodyGraphicsData, GraphicsData& graphicsData, const Float3& position, const Matrix3DF& rotation);

//! python function to read BodyGraphicsData from dictionary, e.g. for body or ground graphics
bool PyWriteBodyGraphicsData(const py::dict& d, const char* item, BodyGraphicsData& data);

//! python function to read BodyGraphicsData from py::object, which must be a list of graphicsData dictionaries
bool PyWriteBodyGraphicsData(const py::object object, BodyGraphicsData& data);

//! python function to write BodyGraphicsData to dictionary, e.g. for testing; 
py::dict PyGetBodyGraphicsDataDictionary(const BodyGraphicsData& data);


class CSystem;



class VisualizationSystemContainer: public VisualizationSystemContainerBase
{
public: //declared as public for direct access via pybind
	ResizableArray<GraphicsData*> graphicsDataList; //!< list of GraphicsData of every visualization System; this list is shared with OpenGL renderer and must be linked to GraphicsData of every VisualizationSystem
	//GraphicsData graphicsData;			//!< data to be processed by OpenGL renderer
	VisualizationSettings settings;		//!< general settings for visualization
	RenderState renderState;		//!< Data linked to state variables of the OpenGL engine (e.g. zoom, transformation matrices, ...)
	ResizableArray<VisualizationSystem*> visualizationSystems; //! linked to all visualization systems (placed in MainSystem); links need to be kept up-to-date by MainSystem Add/Delete
	bool zoomAllRequest;				//! used to perform UpdateMaximumSceneCoordinates()
	bool updateGraphicsDataNow;			//! renderer signals to update the graphics data, e.g. if settings have changed; reset to false after UpdateGraphicsData(...) is called
	std::string computationMessage;		//! message of computation to be shown in renderer window
	bool saveImage;						//!< set true: signal to save the current state shall be rendered and saved to a given image (with consecutive number); will be set false, as soon as frame is saved
	bool saveImageOpenGL;				//!< set true, as soon as graphics is updated (prevents that a frame is saved prior to updating to current visualization state); set false, as soon as frame is saved
private:
	bool stopSimulationFlag;			//! used to wait for user to terminate simulation or render engine
	bool updateGraphicsDataNowInternal; //! internal signal to update the graphics data; reset to false after redraw 

public:
	VisualizationSystemContainer()
	{
		zoomAllRequest = false;
		saveImage = false;
		saveImageOpenGL = false;
		updateGraphicsDataNowInternal = false;
		updateGraphicsDataNow = false;
		stopSimulationFlag = false;

		//now done in MainSystemContainer: AttachToRenderEngine(); //links to render engine at the very beginning of the creation of the VisualizationSystemContainer
	}

	//! list of GraphicsData of all visualizationSystems; linked from glfwclient!
	ResizableArray<GraphicsData*>& GetGraphicsDataList() { return graphicsDataList; }
	const ResizableArray<GraphicsData*>& GetGraphicsDataList() const { return graphicsDataList; }

	VisualizationSettings& GetVisualizationSettings() { return settings; }
	const VisualizationSettings& GetVisualizationSettings() const { return settings; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//SYSTEM FUNCTIONS
	void Append(VisualizationSystem* vSystem) 
	{
		visualizationSystems.Append(vSystem);
		graphicsDataList.Append(&vSystem->GetGraphicsData());
	}

	//void LinkToSystemData(CSystemData* systemDataInit);
	//void LinkPostProcessData(PostProcessData* postProcessDataInit);

	//! this function links the VisualizationSystem to a render engine, such that the changes in the graphics structure drawn upon updates, etc.
	bool AttachToRenderEngine();

	//! this function releases the VisualizationSystemContainer from the render engine (but not other VisualizationSystemContainers!);
	bool DetachFromRenderEngine(VisualizationSystemContainer* detachingVisualizationSystemContainer);

	//! this function waits for the stop flag in the render engine;
	bool WaitForRenderEngineStopFlag();

	//! check GLFW if renderer is running
	bool RendererIsRunning() const;

	//! reset all visualization functions for new system (but keep render engine linked)
	void Reset()
	{
		for (auto item : visualizationSystems)
		{
			item->Reset();
		}
		//anyway done in item->Reset()
		//for (auto item : graphicsDataList)
		//{
		//	item->FlushData();
		//}
		visualizationSystems.Flush();
		graphicsDataList.Flush();

	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//GRAPHICS FUNCTIONS, OVERRIDE from VisualizationSystemContainerBase

	//! OpenGL renderer sends message that graphics shall be updated; update is only done, if current state has higher counter than already existing state
	//  update is sent to all attached visualization systems
	virtual void UpdateGraphicsData() override;
	
	//! Renderer reports to CSystem that simulation shall be interrupted
	virtual void StopSimulation() override;		

	//! renderer reports to simulation that simulation can be continued
	virtual void ContinueSimulation() override;

	////! renderer signals that visualizationIsRunning flag should be set to "flag"; used to know whether WaitForUserToContinue or UpdatePostProcessData shall be called by solver
	//virtual void SetVisualizationIsRunning(bool flag = true) override;

	//! if the system has changed or loaded, compute maximum box of all items and reset scene to the maximum box
	virtual void UpdateMaximumSceneCoordinates() override;

	//! renderer signals to update the graphics data, e.g. if settings have changed
	virtual void UpdateGraphicsDataNow() override
	{
		updateGraphicsDataNow = true;
	}

	//! renderer signals that frame has been grabed and saved
	virtual void SaveImageFinished() override
	{
		saveImage = false;
		saveImageOpenGL = false;
	}

	//! signal renderer that a frame shall be recorded
	virtual bool SaveImageRequest() override
	{
		return (saveImage && saveImageOpenGL);
	}

	//! get zoom all request and reset to false
	virtual bool GetAndResetZoomAllRequest() override
	{
		if (zoomAllRequest) { zoomAllRequest = false; return true; }
		return false;
	}

	//! any multi-line text message from computation to be shown in renderer (e.g. time, solver, ...)
	virtual std::string GetComputationMessage(bool solverInformation = true, 
		bool solutionInformation = true, bool solverTime = true) override;

	//! REMOVE: get backlink of ith main system (0 if not existing), temporary for selection
	virtual MainSystem* GetMainSystemBacklink(Index iSystem) override; 

	//! REMOVE: get backlink to number of main systems, temporary for selection
	virtual Index NumberOFMainSystemsBacklink() const override;

	//! this function does any idle operations (execute some python commands) and returns false if stop flag in the render engine, otherwise true;
	virtual bool DoIdleOperations() override;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! internal signal to update the graphics data; reset to false after redraw 
	virtual bool UpdateGraphicsDataNowInternal() const { return updateGraphicsDataNowInternal; }

	//! perform render update and save the current openGL window to file using the visualization settings
	virtual void RedrawAndSaveImage();

	virtual void Print(std::ostream& os) const
	{
		os << "VisualizationSystemContainer:\n";
		os << "  VisualizationSettings = \n" << settings << "\n";
		Index cnt = 0;
		for (auto item : graphicsDataList)
		{
			os << "  graphicsData " << cnt++ << " = \n" << *item << "\n";
		}
		cnt = 0;
		for (auto item : visualizationSystems)
		{
			os << "  visualization system" << cnt++ << " = \n" << *item << "\n";
		}
		os << "\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const VisualizationSystemContainer& object)
	{
		object.Print(os);
		return os;
	}

};

#endif
