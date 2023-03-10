#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  3D rigid body tutorial with 2 bodies and revolute joints
#
# Author:   Johannes Gerstmayr
# Date:     2021-03-22
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *
#from exudyn.graphicsDataUtilities import *

import numpy as np

SC = exu.SystemContainer()
mbs = SC.AddSystem()


#%%++++++++++++++++++++++++++++++++++++++++++++++++++++
#physical parameters
g =     [0,-9.81,0] #gravity
bodyDim=[1,0.1,0.1] #body dimensions
p0 =    [0,0,0]     #origin of pendulum
pMid0 = np.array([bodyDim[0]*0.5,0,0]) #center of mass, body0

#first link:
#inertia with helper function
iCube0 = InertiaCuboid(density=5000, sideLengths=[1,0.1,0.1])
#print(iCube)

#graphics for body
graphicsBody0 = GraphicsDataRigidLink(p0=[-0.5*bodyDim[0],0,0],p1=[0.5*bodyDim[0],0,0], 
                                     axis0=[0,0,1], axis1=[0,0,0*1], radius=[0.05,0.05], 
                                     thickness = 0.1, width = [0.12,0.12], color=color4red)

[n0,b0]=AddRigidBody(mainSys = mbs,
                     inertia = iCube0,
                     nodeType = str(exu.NodeType.RotationEulerParameters),
                     position = pMid0,
                     rotationMatrix = np.diag([1,1,1]),
                     angularVelocity = [0,0,0],
                     gravity = g,
                     graphicsDataList = [graphicsBody0])

#ground body and marker
oGround = mbs.AddObject(ObjectGround())
markerGround = mbs.AddMarker(MarkerBodyRigid(bodyNumber=oGround, localPosition=[0,0,0]))

#markers for rigid body:
markerBody0J0 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=b0, localPosition=[-0.5*bodyDim[0],0,0]))

#revolute joint (free z-axis)
mbs.AddObject(GenericJoint(markerNumbers=[markerGround, markerBody0J0], 
                           constrainedAxes=[1,1,1,1,1,0],
                           visualization=VObjectJointGeneric(axesRadius=0.01, axesLength=0.1)))

#%%++++++++++++++++++++++++++
#second link:
pMid1 = np.array([bodyDim[0],0,0]) + np.array([0,0,0.5*bodyDim[0]]) #center of mass, body1

graphicsBody1 = GraphicsDataRigidLink(p0=[0,0,-0.5*bodyDim[0]],p1=[0,0,0.5*bodyDim[0]], 
                                     axis0=[1,0,0], axis1=[0,0,0], radius=[0.06,0.05], 
                                     thickness = 0.1, width = [0.12,0.12], color=color4steelblue)

iCube1 = InertiaCuboid(density=5000, sideLengths=[0.1,0.1,1])

[n1,b1]=AddRigidBody(mainSys = mbs,
                     inertia = iCube1,
                     nodeType = str(exu.NodeType.RotationEulerParameters),
                     position = pMid1,
                     rotationMatrix = np.diag([1,1,1]),
                     angularVelocity = [0,0,0],
                     gravity = g,
                     graphicsDataList = [graphicsBody1])

sens1=mbs.AddSensor(SensorBody(bodyNumber=b1, localPosition=[0,0,0.5*bodyDim[0]],
                               fileName='solution/sensorPos.txt',
                               outputVariableType = exu.OutputVariableType.Position))

#markers for rigid body:
markerBody0J1 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=b0, localPosition=[ 0.5*bodyDim[0],0,0]))
markerBody1J0 = mbs.AddMarker(MarkerBodyRigid(bodyNumber=b1, localPosition=[0,0,-0.5*bodyDim[0]]))

#revolute joint (free z-axis)
mbs.AddObject(GenericJoint(markerNumbers=[markerBody0J1, markerBody1J0], 
                            constrainedAxes=[1,1,1,0,1,1],
                            visualization=VObjectJointGeneric(axesRadius=0.01, axesLength=0.1)))

#%%++++++++++++++++++++++++++++++++++++++++++++++++++++++
#assemble system and solve
mbs.Assemble()
if False:
    mbs.systemData.Info() #show detailed information
if False:
    from exudyn.utilities import DrawSystemGraph
    DrawSystemGraph(mbs, useItemTypes=True) #draw nice graph of system

simulationSettings = exu.SimulationSettings() #takes currently set values or default values

tEnd = 4 #simulation time
h = 1e-3 #step size
simulationSettings.timeIntegration.numberOfSteps = int(tEnd/h)
simulationSettings.timeIntegration.endTime = tEnd
simulationSettings.timeIntegration.verboseMode = 1
simulationSettings.timeIntegration.simulateInRealtime = True

SC.visualizationSettings.window.renderWindowSize=[1600,1200]
SC.visualizationSettings.openGL.multiSampling = 4
SC.visualizationSettings.general.autoFitScene = False

exu.StartRenderer()
if 'renderState' in exu.sys: #reload old view
    SC.SetRenderState(exu.sys['renderState'])

mbs.WaitForUserToContinue() #stop before simulating

exu.SolveDynamic(mbs, simulationSettings = simulationSettings,
                 solverType=exu.DynamicSolverType.TrapezoidalIndex2)

SC.WaitForRenderEngineStopFlag() #stop before closing
exu.StopRenderer() #safely close rendering window!

if True:
    from exudyn.plot import PlotSensor
    PlotSensor(mbs, [sens1],[1])


