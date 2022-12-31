#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Test for AddRevoluteJoint utility function
#
# Author:   Johannes Gerstmayr 
# Date:     2021-07-01
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *

# from modelUnitTests import ExudynTestStructure, exudynTestGlobals

from math import sin, cos, pi
import numpy as np

SC = exu.SystemContainer()
mbs = SC.AddSystem()


#background
color = [0.1,0.1,0.8,1]
L = 0.4 #length of bodies
d = 0.1 #diameter of bodies

oGround=mbs.AddObject(ObjectGround(referencePosition= [-0.5*L,0,0])) 
mPosLast = mbs.AddMarker(MarkerBodyRigid(bodyNumber = oGround, localPosition=[0,0,0]))
A0 = np.eye(3)
Alast = A0 #previous marker
bodyLast = oGround

Alist=[]
axisList=[]
A=RotationMatrixX(0)

nBodies = 100
for i in range(nBodies):
    delta = 0.01*pi
    #A = RotationMatrixZ(delta)
    v0= A@[0,0,1]
    Alist+=[A]
    axisList+=[v0]
    A = A @ RotationMatrixX(delta)@RotationMatrixZ(2*delta)


p0 = [0.,0.,0] #reference position
vLoc = np.array([L,0,0]) #last to next joint
g = [0,0,9.81]
#g = [0,9.81,0]

#create a chain of bodies:
for i in range(nBodies):
    #print("Build Object", i)
    inertia = InertiaCuboid(density=1000, sideLengths=[L,d,d])
    p0 += Alist[i] @ (0.5*vLoc)
    #p0 += (0.5*vLoc)

    ep0 = eulerParameters0 #no rotation
    graphicsBody = GraphicsDataOrthoCubePoint([0,0,0], [0.96*L,d,d], color4steelblue)
    [nRB, oRB] = AddRigidBody(mbs, inertia, nodeType=exu.NodeType.RotationEulerParameters,
                              position=p0,
                              rotationMatrix=Alist[i],
                              gravity=g,
                              graphicsDataList=[graphicsBody])

    body0 = bodyLast
    body1 = oRB
    # point = mbs.GetObjectOutputBody(oRB,exu.OutputVariableType.Position,
    #                                 localPosition=[-0.5*L,0,0],
    #                                 configuration=exu.ConfigurationType.Reference)
    #axis = [0,0,1]
    axis = axisList[i]
    AddRevoluteJoint(mbs, body0, body1, [0.5*L,0,0], Alast.T@axis, useGlobalFrame=False, 
                          axisRadius=0.6*d, axisLength=1.2*d)
    # AddRevoluteJoint(mbs, body0, body1, point, axis, useGlobalFrame=True, 
    #                   axisRadius=0.6*d, axisLength=1.2*d)

    bodyLast = oRB
    
    p0 += Alist[i] @ (0.5*vLoc)
    #p0 += (0.5*vLoc)
    Alast = Alist[i]

#mbs.AddLoad(LoadForceVector(markerNumber=mPosLast, loadVector=[0,0,20]))

mbs.Assemble()

simulationSettings = exu.SimulationSettings() #takes currently set values or default values

tEnd = 5
h=0.0005  #use small step size to detext contact switching

simulationSettings.timeIntegration.numberOfSteps = int(tEnd/h)
simulationSettings.timeIntegration.endTime = tEnd
simulationSettings.solutionSettings.solutionWritePeriod = 0.005
simulationSettings.solutionSettings.sensorsWritePeriod = 0.01
#simulationSettings.timeIntegration.simulateInRealtime = True
simulationSettings.timeIntegration.realtimeFactor = 0.5
simulationSettings.timeIntegration.verboseMode = 1

simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.8
simulationSettings.timeIntegration.generalizedAlpha.computeInitialAccelerations=True
simulationSettings.timeIntegration.newton.useModifiedNewton = True
#simulationSettings.timeIntegration.newton.modifiedNewtonJacUpdatePerStep = True
simulationSettings.linearSolverType = exu.LinearSolverType.EigenSparse

SC.visualizationSettings.nodes.show = True
SC.visualizationSettings.nodes.drawNodesAsPoint  = False
SC.visualizationSettings.nodes.showBasis = True
SC.visualizationSettings.nodes.basisSize = 0.015
SC.visualizationSettings.connectors.showJointAxes = True

#for snapshot:
SC.visualizationSettings.openGL.multiSampling=4
SC.visualizationSettings.openGL.lineWidth=2
SC.visualizationSettings.window.renderWindowSize = [800,600]
SC.visualizationSettings.general.drawCoordinateSystem=False
SC.visualizationSettings.general.drawWorldBasis=True
# SC.visualizationSettings.general.useMultiThreadedRendering = False
SC.visualizationSettings.general.autoFitScene = False #use loaded render state
useGraphics = True
if useGraphics:
    simulationSettings.displayComputationTime = True
    simulationSettings.displayStatistics = True
    exu.StartRenderer()
    if 'renderState' in exu.sys:
        SC.SetRenderState(exu.sys[ 'renderState' ])
    #mbs.WaitForUserToContinue()
else:
    simulationSettings.solutionSettings.writeSolutionToFile = False

#exu.SolveDynamic(mbs, simulationSettings, solverType=exu.DynamicSolverType.TrapezoidalIndex2)
exu.SolveDynamic(mbs, simulationSettings, showHints=True)

if True: #use this to reload the solution and use SolutionViewer
    #sol = LoadSolutionFile('coordinatesSolution.txt')
    from exudyn.interactive import SolutionViewer
    SolutionViewer(mbs) #can also be entered in IPython ...


u0 = mbs.GetNodeOutput(nRB, exu.OutputVariableType.Displacement)
rot0 = mbs.GetNodeOutput(nRB, exu.OutputVariableType.Rotation)
exu.Print('u0=',u0,', rot0=', rot0)

result = (abs(u0)+abs(rot0)).sum()
exu.Print('solution of addRevoluteJoint=',result)

# exudynTestGlobals.testError = result - (1.2538806799246283) #2020-07-01: 1.2538806799246283
# exudynTestGlobals.testResult = result



#%%+++++++++++++++++++++++++++++
if useGraphics:
    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!


