/** ***********************************************************************************************
* @brief		Implementation of class VisualizationSystemContainer
* @details		Details:
 				- a visualization system container; links to OpenGL renderer and contains several visualization systems
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

//#include "System/CSystem.h"						//included in VisualizationSystem.h
//#include "Autogenerated/VisualizationSettings.h"  //included in VisualizationSystem.h
//#include "Graphics/GraphicsData.h"				//included in VisualizationSystem.h
//#include "Graphics/VisualizationSystem.h"
//#include "Graphics/VisualizationSystemContainerBase.h"
#include "Graphics/VisualizationSystemContainer.h"
#include "Graphics/VisualizationPrimitives.h"

#include "Graphics/GlfwClient.h" //in order to link to graphics engine


//#ifdef USE_GLFW_GRAPHICS
//#endif

bool VisualizationSystemContainer::LinkToRenderEngine()
{
#ifdef USE_GLFW_GRAPHICS

	glfwRenderer.DetachVisualizationSystem(); //means, that every new systemcontainer links to the render engine and the old container is lost; necessary if an old systemcontainer is still linked

	if (!glfwRenderer.LinkVisualizationSystem(&graphicsDataList, &settings, this, &renderState)) 
		//(&graphicsData, &settings, this, &renderState))
	{
		SysError("VisualizationSystem::LinkToRenderEngine: Visualization cannot be linked to several systems at the same time yet!");
		return false;
	}
	return true;
#else
	return false;
#endif
}

bool VisualizationSystemContainer::DetachRenderEngine()
{
#ifdef USE_GLFW_GRAPHICS
	return glfwRenderer.DetachVisualizationSystem();
#else
	return false;
#endif
	
}


//! OpenGL renderer sends message that graphics shall be updated ==> update graphics data
void VisualizationSystemContainer::UpdateGraphicsData()
{
	if (zoomAllRequest)
	{
		zoomAllRequest = false; //this is not fully thread safe, but should not happen very often ==> user needs to zoom manually then ...
		//pout << "zoomAllrequest1\n";
		UpdateMaximumSceneCoordinates();
		//pout << "zoomAllrequest2\n";
	}
	if (updateGraphicsDataNow) { updateGraphicsDataNowInternal = true; updateGraphicsDataNow = false; } //enables immediate new set of updateGraphicsDataNow
	if (saveImage) { saveImageOpenGL = true; } //as graphics are updated now, the saveImageOpenGL flag can be set

	Index cnt = 0;
	for (auto item : visualizationSystems)
	{
		//pout << "UpdateGraphicsData1\n";
		item->UpdateGraphicsData(*this);
		//pout << "UpdateGraphicsData2\n";
		if (cnt == 0 && settings.general.drawWorldBasis)
		{
			EXUvis::DrawOrthonormalBasis(Vector3D({ 0,0,0 }), EXUmath::unitMatrix3D, settings.general.worldBasisSize,
				0.005*settings.general.worldBasisSize, item->GetGraphicsData());
		}
		cnt++;
	}

	updateGraphicsDataNowInternal = false; //only valid for one run; may not be earlier, as item->UpdateGraphicsData(...) needs this flag!
}

// put this to SystemContainer ...
//! perform render update and save the current openGL window to file using the visualization settings
void VisualizationSystemContainer::RedrawAndSaveImage()
{
	//now a new saveImage message can be sent
	saveImage = true;			//flag initiates saveImageOpenGL at next UpdateGraphicsData() called from Renderer
	saveImageOpenGL = false;	//after graphics update, the scene is saved and flags (saveImage, saveImageOpenGL) are set to false
	UpdateGraphicsDataNow();	//if a current redraw is performed, it will also initiate a second redraw operation ...

	Index timeOut = 500; //max iterations to wait, before frame is redrawn and saved
	Index timerMilliseconds = settings.exportImages.saveImageTimeOut / timeOut;
	if (timerMilliseconds == 0) { timerMilliseconds = 1; } //min wait time per iteration

	//now wait until the saveImage flag has been deleted by the current redraw operation
	Index i = 0;
	while (i++ < timeOut && (saveImageOpenGL || saveImage)) //wait timeOut*timerMilliseconds seconds for last operation to finish
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(timerMilliseconds));
	}
	if (saveImageOpenGL || saveImage)
	{
		PyWarning("PostProcessData::RedrawAndSaveImage: save frame to image file did not finish; increase timeout parameter");
	}
}


//! Renderer reports to simulation that simulation shall be interrupted
void VisualizationSystemContainer::StopSimulation()
{
	//as we do not know, which simulation is executed, all system computations are interrupted
	stopSimulationFlag = true; //tell also VisualizationSystemContainer
	for (auto item : visualizationSystems)
	{
		item->postProcessData->stopSimulation = true;
	}

}

//! Renderer reports to simulation that simulation can be continued
void VisualizationSystemContainer::ContinueSimulation()
{
	//as we do not know, which simulation is executed, all system computations are interrupted
	for (auto item : visualizationSystems)
	{
		item->postProcessData->simulationPaused = false;
	}

}

//! this function waits for the stop flag in the render engine;
bool VisualizationSystemContainer::WaitForRenderEngineStopFlag()
{
#ifdef USE_GLFW_GRAPHICS

	stopSimulationFlag = false; //initialize the flag, if used several times; this is thread safe
	while (!stopSimulationFlag && glfwRenderer.WindowIsInitialized())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		PyProcessExecuteQueue(); //use time to execute incoming python tasks
		for (auto item : visualizationSystems)
		{
			item->postProcessData->ProcessUserFunctionDrawing(); //check if user functions to be drawn and do user function evaluations
		}
	}
#endif
	for (auto item : visualizationSystems)
	{
		item->postProcessData->visualizationIsRunning = false; //signal, that visualization is stopped now
	}

	return true;
}

//! if the system has changed or loaded, compute maximum box of all items and reset scene to the maximum box
void VisualizationSystemContainer::UpdateMaximumSceneCoordinates()
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	renderState.centerPoint = settings.openGL.initialCenterPoint; //this is the initial centerPoint; hereafter it can be changed!
	//renderState.sceneCenterPoint = settings.openGL.initialCenterPoint; //this is the initial centerPoint; hereafter it can be changed!
	renderState.zoom = settings.openGL.initialZoom;
	renderState.maxSceneSize = settings.openGL.initialMaxSceneSize;
	renderState.currentWindowSize = settings.window.renderWindowSize;

	//set modelRotation to identity matrix (4x4); Use rotation part only from Float9 initialModelRotation
	renderState.modelRotation.SetAll(0.f);
	renderState.modelRotation[0] = settings.openGL.initialModelRotation[0][0];
	renderState.modelRotation[1] = settings.openGL.initialModelRotation[0][1];
	renderState.modelRotation[2] = settings.openGL.initialModelRotation[0][2];
	renderState.modelRotation[4] = settings.openGL.initialModelRotation[1][0];
	renderState.modelRotation[5] = settings.openGL.initialModelRotation[1][1];
	renderState.modelRotation[6] = settings.openGL.initialModelRotation[1][2];
	renderState.modelRotation[8] = settings.openGL.initialModelRotation[2][0];
	renderState.modelRotation[9] = settings.openGL.initialModelRotation[2][1];
	renderState.modelRotation[10] = settings.openGL.initialModelRotation[2][2];
	renderState.modelRotation[15] = 1.;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (settings.general.autoFitScene)
	{
		//for now, use nodal reference coordinates to estimate the maximum zoom level
		Vector3D pmax({ -1e30,-1e30,-1e30 });
		Vector3D pmin({ 1e30,1e30,1e30 });
		for (auto visSystem : visualizationSystems)
		{
			//! @todo extend VisualizationSystemContainer::UpdateMaximumSceneCoordinates for objects, markers and loads; maybe better to first draw all and zoom to full region?
			for (auto item : visSystem->systemData->GetCNodes())
			{
				//if ((item->GetType() == CNodeType::Point) || (item->GetType() == CNodeType::RigidBody))
				if ((Index)item->GetNodeGroup() & (Index)CNodeGroup::ODE2variables)
				{
					//LinkedDataVector pref = item->GetReferenceCoordinateVector();
					Vector3D pref = ((CNodeODE2*)item)->GetPosition(ConfigurationType::Reference);

					pmax[0] = EXUstd::Maximum(pref[0], pmax[0]);
					pmax[1] = EXUstd::Maximum(pref[1], pmax[1]);
					pmax[2] = EXUstd::Maximum(pref[2], pmax[2]);
					pmin[0] = EXUstd::Minimum(pref[0], pmin[0]);
					pmin[1] = EXUstd::Minimum(pref[1], pmin[1]);
					pmin[2] = EXUstd::Minimum(pref[2], pmin[2]);
				}

			}
		}
		Vector3D center = 0.5*(pmin + pmax);

		if (renderState.centerPoint[0] == 0 && renderState.centerPoint[1] == 0 && renderState.centerPoint[2] == 0) {
			renderState.centerPoint.CopyFrom(center);
		}

		renderState.maxSceneSize = (float)((pmax - pmin).GetL2Norm());
		if (renderState.maxSceneSize < settings.general.minSceneSize) 
		{ 
			renderState.maxSceneSize = settings.general.minSceneSize; 
		}
		renderState.zoom = 0.5f*renderState.maxSceneSize;
	}
}

//! any multi-line text message from computation to be shown in renderer (e.g. time, solver, ...)
std::string VisualizationSystemContainer::GetComputationMessage()
{
	//workaround: take message of first system to be shown
	if (visualizationSystems.NumberOfItems())
	{
		return visualizationSystems[0]->GetComputationMessage();
	}
	return std::string();
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! copy bodyGraphicsData (of body) into global graphicsData (of system)
void AddBodyGraphicsData(const BodyGraphicsData& bodyGraphicsData, GraphicsData& graphicsData, const Float3& position, const Matrix3DF& rotation)
{
	bool applyRotation = true;
	if (rotation(0, 0) == 1.f && rotation(1, 1) == 1.f && rotation(2, 2) == 1.f) { applyRotation = false; }

	for (GLLine item : bodyGraphicsData.glLines) //copy objects
	{
		if (applyRotation)
		{
			EXUmath::RigidBodyTransformation(rotation, position, item.point1, item.point1);
			EXUmath::RigidBodyTransformation(rotation, position, item.point2, item.point2);
		}
		else
		{
			item.point1 += position;
			item.point2 += position;
		}
		graphicsData.glLines.Append(item);
	}

	for (GLCircleXY item : bodyGraphicsData.glCirclesXY) //copy objects
	{
		if (applyRotation)
		{
			EXUmath::RigidBodyTransformation(rotation, position, item.point, item.point);
		}
		else
		{
			item.point += position;
		}
		graphicsData.glCirclesXY.Append(item);
	}

	for (GLText item : bodyGraphicsData.glTexts) //copy objects, but string pointers are just assigned!
	{
		if (applyRotation)
		{
			EXUmath::RigidBodyTransformation(rotation, position, item.point, item.point);
		}
		else
		{
			item.point += position;
		}

		Index len = strlen(item.text); 
		//int i = (int)strlen("x");
		char* temp = new char[len+1]; //needs to be copied, because string is destroyed everytime it is updated! ==> SLOW for large number of texts (node numbers ...)
		//strcpy_s(temp, len + 1, item.text); //not working with gcc
		strcpy(temp, item.text); //item.text will be destroyed upon deletion of BodyGraphicsData!
		item.text = temp;
		graphicsData.glTexts.Append(item);
	}

	for (GLTriangle item : bodyGraphicsData.glTriangles) //copy objects
	{
		if (applyRotation)
		{
			for (Index i = 0; i < 3; i++)
			{
				EXUmath::RigidBodyTransformation(rotation, position, item.points[i], item.points[i]);
				EXUmath::RigidBodyTransformation(rotation, position, item.normals[i], item.normals[i]);
			}
		}
		else
		{
			for (Index i = 0; i < 3; i++)
			{
				item.points[i] += position;
			}
		}
		graphicsData.glTriangles.Append(item);
	}

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! object graphics data for ground objects, rigid bodies and mass points
bool PyWriteBodyGraphicsData(const py::dict& d, const char* item, BodyGraphicsData& data)
{
	data.FlushData(); //this is the body data, which cannot be filled incrementally
	if (d.contains(item))
	{
		py::object other = d[item]; //this is necessary to make isinstance work
		return PyWriteBodyGraphicsData(other, data);
	}//if "GraphicsData" does not exist, no error is displayed
	return true;
}
//! python function to read BodyGraphicsData from py::object, which must be a list of graphicsData dictionaries
bool PyWriteBodyGraphicsData(const py::object object, BodyGraphicsData& data)
{
	if (py::isinstance<py::list>(object)) //must be a list of graphicsData dictionaries
	{
		py::list list = (py::list)(object);

		for (auto graphicsItem : list)
		{
			//now read out dictionaries, containing graphics structures:
			//{'type':'Line', 'color':[1,0,0,1], 'data':[x1,y1,z1, x2,y2,z2, ...]}
			//{'type':'Circle', 'color':[1,0,0,1], 'radius': r, 'position': [x,y,z], 'normal': [x,y,z]}
			//{'type':'Text', 'color':[1,0,0,1], 'text':"sample text", 'position':[x,y,z]}

			if (py::isinstance<py::dict>(graphicsItem)) //must be a dictionary of graphicsData
			{
				const py::dict& gDict = (py::dict&)graphicsItem;
				if (gDict.contains("type"))
				{
					py::object pyType = gDict["type"]; //this is necessary to make isinstance work
					if (py::isinstance<py::str>(pyType))
					{
						std::string pyTypeStr = py::cast<std::string>(pyType); //! read out dictionary and cast to C++ type

						//add lines, circles, text, and triangles ....
						if (pyTypeStr == "Line")
						{

							GLLine line;
							line.color1 = line.color2 = EXUvis::defaultColorFloat4;
							if (gDict.contains("color"))
							{
								py::object gColor = gDict["color"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gColor)) //must be a list of graphicsData dictionaries
								{
									py::list colorList = (py::list)(gColor);
									std::vector<float> stdColorList = py::cast<std::vector<float>>(colorList); //! # read out dictionary and cast to C++ type

									if (stdColorList.size() == 4)
									{
										line.color1 = line.color2 = Float4(stdColorList);
									}
									else { PyError("GraphicsData Line: color must be a float vector with 4 components"); return false; }
								}
								else { PyError("GraphicsData Line: color must be a float vector with 4 components"); return false; }
							}
							if (gDict.contains("data"))
							{
								py::object gData = gDict["data"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gData)) //must be a list of graphicsData dictionaries
								{
									py::list dataList = (py::list)(gData);
									std::vector<float> gd = py::cast<std::vector<float>>(dataList); //! # read out dictionary and cast to C++ type

									Index n = gd.size() / 3;
									if (n * 3 != gd.size() || n < 2)
									{
										PyError("GraphicsData Line: data must be a float vector with exactly 3*n components and n > 1"); return false;
									}

									for (Index k = 1; k < n; k++)
									{
										line.point1 = Float3({ gd[3 * (k - 1)],gd[3 * (k - 1) + 1] ,gd[3 * (k - 1) + 2] });
										line.point2 = Float3({ gd[3 * k],gd[3 * k + 1] ,gd[3 * k + 2] });
										data.glLines.Append(line);
									}
								}
								else { PyError("GraphicsData Line: data must be a float vector with 3*n components"); return false; }

							}
							else { PyError("GraphicsData Line: must contain 'data' with (x1,y1,z1,...) line coordinates "); return false; }
						} //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						else if (pyTypeStr == "Circle")
						{
							GLCircleXY circle;
							circle.color = EXUvis::defaultColorFloat4;
							circle.numberOfSegments = 0; //use default

							if (gDict.contains("color"))
							{
								py::object gColor = gDict["color"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gColor)) //must be a list of graphicsData dictionaries
								{
									py::list colorList = (py::list)(gColor);
									std::vector<float> stdColorList = py::cast<std::vector<float>>(colorList); //! # read out dictionary and cast to C++ type

									if (stdColorList.size() == 4)
									{
										circle.color = Float4(stdColorList);
									}
									else { PyError("GraphicsData Circle: color must be a float vector with 4 components"); return false; }
								}
								else { PyError("GraphicsData Circle: color must be a float vector with 4 components"); return false; }
							}

							if (gDict.contains("radius"))
							{
								py::object gData = gDict["radius"]; //this is necessary to make isinstance work
								if (py::isinstance<py::float_>(gData) || py::isinstance<py::int_>(gData)) //must be a scalar value
								{
									circle.radius = (py::float_)(gData);
								}
								else { PyError("GraphicsData Circle: radius must be a scalar value"); return false; }

							}
							else { PyError("GraphicsData Circle: must contain 'radius'"); return false; }

							if (gDict.contains("position"))
							{
								py::object gData = gDict["position"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gData))  //must be a list of 3 coordinates
								{
									py::list dataList = (py::list)(gData);
									std::vector<float> gd = py::cast<std::vector<float>>(dataList); //! # read out dictionary and cast to C++ type

									if (gd.size() != 3)
									{
										PyError("GraphicsData Circle: position must be a float vector with 3 components"); return false;
									}

									circle.point = Float3(gd);
								}
								else { PyError("GraphicsData Circle: position must be a float vector with 3 components"); return false; }

							}
							else { PyError("GraphicsData Circle: must contain 'position'"); return false; }

							data.glCirclesXY.Append(circle);

						} //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						else if (pyTypeStr == "Text")
						{
							GLText text;
							text.color = EXUvis::defaultColorFloat4;
							text.offsetX = 0.f;
							text.offsetY = 0.f;
							text.size = 0; //indicates to use default size


							if (gDict.contains("color"))
							{
								py::object gColor = gDict["color"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gColor)) //must be a list of graphicsData dictionaries
								{
									py::list colorList = (py::list)(gColor);
									std::vector<float> stdColorList = py::cast<std::vector<float>>(colorList); //! # read out dictionary and cast to C++ type

									if (stdColorList.size() == 4)
									{
										text.color = Float4(stdColorList);
									}
									else { PyError("GraphicsData Text: color must be a float vector with 4 components"); return false; }
								}
								else { PyError("GraphicsData Text: color must be a float vector with 4 components"); return false; }
							}

							if (gDict.contains("position"))
							{
								py::object gData = gDict["position"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gData))  //must be a list of 3 coordinates
								{
									py::list dataList = (py::list)(gData);
									std::vector<float> gd = py::cast<std::vector<float>>(dataList); //! # read out dictionary and cast to C++ type

									if (gd.size() != 3)
									{
										PyError("GraphicsData Text: position must be a float vector with 3 components"); return false;
									}

									text.point = Float3(gd);

								}
								else { PyError("GraphicsData Text: position must be a float vector with 3 components"); return false; }
							}
							else { PyError("GraphicsData Text: must contain 'position'"); return false; }

							if (gDict.contains("text"))
							{
								py::object gData = gDict["text"]; //this is necessary to make isinstance work
								if (py::isinstance<py::str>(gData)) //must be a scalar value
								{
									std::string gText = (py::str)(gData);
									int len = (int)gText.size();
									text.text = new char[len + 1]; //will be deleted in destructor of GraphicsData
									//strcpy_s(text.text, len + 1, gText.c_str()); //not working with gcc
									strcpy(text.text, gText.c_str());

									data.glTexts.Append(text);
								}
								else { PyError("GraphicsData Text: 'text' must be of type string"); return false; }

							}
							else { PyError("GraphicsData Text: must contain 'text' providing a string"); return false; }

						} //end Text ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						else if (pyTypeStr == "TriangleList")
						{
							//dataTriangleList = { 'type':'TriangleList', 
							//'points' : [x0,y0,z0, x1,y1,z1, ...],
							//'colors' : [R0,G0,B0,A0, R1,G2,B1,A1, ...],
							//'normals' : [n0x,n0y,n0z, ...],
							//'triangles' : [T0point0, T0point1, T0point2, ...] }

							ResizableArray<Float3> points; //temporary data, used during evaluation of data
							ResizableArray<Float3> normals; //temporary data, used during evaluation of data (must be size of points)
							ResizableArray<Float4> colors; //temporary data, used during evaluation of data (must be size of points)

							if (gDict.contains("points"))
							{
								py::object gDictList = gDict["points"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gDictList)) //must be a list of graphicsData dictionaries
								{
									py::list gList = (py::list)(gDictList);
									std::vector<float> stdGList = py::cast<std::vector<float>>(gList); //! # read out dictionary and cast to C++ type

									if ((stdGList.size() % 3) == 0)
									{
										Index n = stdGList.size() / 3;
										points.SetNumberOfItems(n);
										for (Index i = 0; i < n; i++)
										{
											points[i] = Float3({ stdGList[i * 3],stdGList[i * 3 + 1],stdGList[i * 3 + 2] });
											//pout << "p" << i << " = " << points[i] << "\n";
										}
									}
									else { PyError("GraphicsData::TriangleList::points must be a float list with 3*n components, n being the number of points"); return false; }
								}
								else { PyError("GraphicsData::TriangleList::points must be a float list"); return false; }
							}
							else
							{
								PyError("GraphicsData::TriangleList must contain 'points' being a float list with n*(x,y,z)-components, n being the number of points");
							}

							if (gDict.contains("colors"))
							{
								py::object gDictList = gDict["colors"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gDictList)) //must be a list of graphicsData dictionaries
								{
									py::list gList = (py::list)(gDictList);
									std::vector<float> stdGList = py::cast<std::vector<float>>(gList); //! # read out dictionary and cast to C++ type

									if ((stdGList.size() % 4) == 0 && stdGList.size() / 4 == points.NumberOfItems())
									{
										Index n = stdGList.size() / 4;
										colors.SetNumberOfItems(n);
										for (Index i = 0; i < n; i++)
										{
											colors[i] = Float4({ stdGList[i * 4], stdGList[i * 4 + 1], stdGList[i * 4 + 2], stdGList[i * 4 + 3] });
										}
									}
									else { PyError("GraphicsData::TriangleList::colors must be a float list with 4*n components (R,G,B,A), n being the identical to the number of points"); return false; }
								}
								else { PyError("GraphicsData::TriangleList::colors must be a float list"); return false; }
							}
							else
							{   //set default color
								colors.SetNumberOfItems(points.NumberOfItems());
								for (auto& color : colors) { color = EXUvis::defaultColorBlue4; }
							}

							bool normalsDefined = true;
							if (gDict.contains("normals"))
							{
								py::object gDictList = gDict["normals"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gDictList)) //must be a list of graphicsData dictionaries
								{
									py::list gList = (py::list)(gDictList);
									std::vector<float> stdGList = py::cast<std::vector<float>>(gList); //! # read out dictionary and cast to C++ type

									if ((stdGList.size() % 3) == 0 && stdGList.size() / 3 == points.NumberOfItems())
									{
										Index n = stdGList.size() / 3;
										normals.SetNumberOfItems(n);
										for (Index i = 0; i < n; i++)
										{
											normals[i] = Float3({ stdGList[i * 3],stdGList[i * 3 + 1],stdGList[i * 3 + 2] });
										}
									}
									else { PyError("GraphicsData::TriangleList::normals must be a float list with 3*n components (nx,ny,nz), n being the identical to the number of points"); return false; }
								}
								else { PyError("GraphicsData::TriangleList::normals must be a float list"); return false; }
							}
							else
							{   //set default normal
								normals.SetNumberOfItems(points.NumberOfItems());
								for (auto& normal : normals) { normal = Float3({ 0,0,0 }); }
								normalsDefined = false;
							}

							if (gDict.contains("triangles"))
							{
								py::object gDictList = gDict["triangles"]; //this is necessary to make isinstance work
								if (py::isinstance<py::list>(gDictList)) //must be a list of graphicsData dictionaries
								{
									py::list gList = (py::list)(gDictList);
									std::vector<Index> stdGList = py::cast<std::vector<Index>>(gList); //! # read out dictionary and cast to C++ type

									if ((stdGList.size() % 3) == 0)
									{
										Index n = stdGList.size() / 3;
										Index np = points.NumberOfItems();
										GLTriangle trig;
										for (Index i = 0; i < n; i++)
										{
											Index3 pointInd = Index3({ stdGList[i * 3], stdGList[i * 3 + 1], stdGList[i * 3 + 2] });
											for (Index j = 0; j < 3; j++)
											{
												Index ind = pointInd[j];
												if (EXUstd::IndexIsInRange(ind, 0, np))
												{
													trig.points[j] = points[ind];
													trig.normals[j] = normals[ind];
													trig.colors[j] = colors[ind];
												}
												else
												{
													PyError(STDstring("GraphicsData::TriangleList::triangles: point indices need to be in range [0, points.size()-1], but got index: ") + EXUstd::ToString(ind)); return false;
												}
											}
											if (!normalsDefined)
											{
												EXUvis::ComputeTriangleNormals(trig.points, trig.normals);
												//Float3 v1 = trig.points[1] - trig.points[0];
												//Float3 v2 = trig.points[2] - trig.points[0];
												//Float3 n = v1.CrossProduct(v2); //@todo: need to check correct outward normal direction in openGL
												//float len = n.GetL2Norm();
												//if (len != 0.f) { n *= 1.f/len; }
												//trig.normals[0] = n;
												//trig.normals[1] = n;
												//trig.normals[2] = n;
											}
											data.glTriangles.Append(trig);
											//pout << "trig" << i << " = " << trig.points[0] << "," << trig.points[0] << "," << trig.points[0] << "\n";
										}
									}
									else { PyError("GraphicsData::TriangleList::triangles must be a float list with 3*n components, n being the number of triangles"); return false; }
								}
								else { PyError("GraphicsData::TriangleList::triangles must be a float list"); return false; }
							}
							else
							{
								PyError("GraphicsData::TriangleList must contain 'triangles' being a float list with n*(point0,point1,point2)-components, n being the number of triangles; point0, point1, point2 ... point indices of one triangle");
							}

						} //end triangles +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						else
						{
							PyError(STDstring("GraphicsData type '") + pyTypeStr + "' not supported");
						}
					} //if (py::isinstance<py::str>(pyType))
				} //gDict.contains("type")
				else
				{
					PyError("GraphicsData must contain a 'type'"); return false;
				}
			}
		}//for-loop graphics items
	}
	else 
	{ 
		PyError("GraphicsData must be of type list: [graphicsDict1, graphicsDict2, ...]"); return false; 
	}
	return true;
}

py::dict PyGetBodyGraphicsDataDictionary(const BodyGraphicsData& data)
{
	auto d = py::dict();
	//d["type"] = std::string("Line");
	d["TODO"] = std::string("Get graphics data to be implemented");
	return d;
}

