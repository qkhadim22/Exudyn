#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Test with postNewtonUserFunction and recommendedStepSize modeling a simplistic 1-mass- penalty contact problem;
#           Uses step reduction to resolve contact switching point
#
# Author:   Johannes Gerstmayr
# Date:     2021-03-20
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *

from modelUnitTests import ExudynTestStructure, exudynTestGlobals #for testing
#import numpy as np

SC = exu.SystemContainer()
mbs = SC.AddSystem()
exu.Print('EXUDYN version='+exu.GetVersionString())

#define parameters of mass point
L=0.5
r = 0.05
g=9.81
mass = 0.25         #mass in kg
spring = 20000        #stiffness of spring-damper in N/m
damper = 0*0.01*spring          #damping constant in N/(m/s)
load0 = -mass*g     #in negative y-direction

doRefSol = False
tEnd = 0.5     #end time of simulation
h = 5e-3
if doRefSol:
    h=1e-5

#data coordinate: 0=no contact, 1=contact
nData=mbs.AddNode(NodeGenericData(initialCoordinates=[0], numberOfDataCoordinates=1))

#node for 3D mass point:
n1=mbs.AddNode(Point(referenceCoordinates = [0,L,0],
                     initialCoordinates = [0,r-L+0.01,0],
                     initialVelocities = [0,-1,0]))

#user function for spring force
def springForce(mbs, t, u, v, k, d, offset, mu, muPropZone):
    p = L+u-r
    #print(p)
    data = mbs.systemData.GetDataCoordinates()
    if data[0] == 1:
        return k*p
    else:
        return 0

def PostNewtonUserFunction(mbs, t):
    p0 = mbs.GetNodeOutput(n1, exu.OutputVariableType.Position, configuration=exu.ConfigurationType.StartOfStep)[1] - r
    p = mbs.GetNodeOutput(n1, exu.OutputVariableType.Position)[1] - r
    #v0 = mbs.GetNodeOutput(n1, exu.OutputVariableType.Velocity, configuration=exu.ConfigurationType.StartOfStep)[1]
    #a0 = mbs.GetNodeOutput(n1, exu.OutputVariableType.Acceleration, configuration=exu.ConfigurationType.StartOfStep)[1]
    h = mbs.sys['dynamicSolver'].it.currentStepSize #grab current step size from dynamic solver
    data = mbs.systemData.GetDataCoordinates()
    data0 = mbs.systemData.GetDataCoordinates(configuration=exu.ConfigurationType.StartOfStep)

    #data[0] = 0 #no contact; 0 corresponds to the only one data coordinate in the system, attributed to contact
    recommendedStepSize = -1
    error = 0
    #check if previous assumption was wrong ==> set error, reduce step size and set new contact state
    if p < 0:
        if data0[0] == 0:
            error = abs(p)
            #recommendedStepSize = 1e-6 #simple alternative
            #x = abs(v0)*h #this is the estimated distance (acc=0) per step
            #x = 0.5*abs(a0)*h**2
            #recommendedStepSize = min(h,abs(h*(x-error)/x)) #assuming almost constant velocity during step
            if (p0 > 0):
                recommendedStepSize = h*(abs(p0))/(abs(p0)+abs(p))
            else:
                recommendedStepSize = 0.25*h #simple alternative


            data[0] = 1 #contact
        #else:
        #    recommendedStepSize = 1e-4
        #    error = abs(h-1e-4)
    else:
        if data0[0] == 1:
            error = abs(p)
            #recommendedStepSize = 1e-6 #simple alternative
            if (p0 > 0):
                recommendedStepSize = h*(abs(p0))/(abs(p0)+abs(p))
            else:
                recommendedStepSize = 0.25*h #simple alternative
            data[0] = 0 #contact off

    #print("t=", round(t,6), ", p=", round(p,6), ", p0=", round(p0,6), #", a0=", round(a0,6), 
    #      ", h=", round(h,6), ", hRec=", 
    #      round(recommendedStepSize,6), ", tRec=", round(t-h+recommendedStepSize,6), 
    #      ", c0=", data0[0], ", c=", data[0], ", e=", error)

    mbs.systemData.SetDataCoordinates(data)
    return [error,recommendedStepSize]

mbs.SetPostNewtonUserFunction(PostNewtonUserFunction)

#ground node
d=0.01
gGround = GraphicsDataOrthoCubePoint([0,-d*0.5,0],[2*L,d,d],color=color4grey)
oGround=mbs.AddObject(ObjectGround(visualization=VObjectGround(graphicsData=[gGround])))

nGround=mbs.AddNode(NodePointGround(referenceCoordinates = [0,0,0]))

#add mass point (this is a 3D object with 3 coordinates):
gSphere = GraphicsDataSphere([0,0,0], r, color=color4red, nTiles=20)
massPoint = mbs.AddObject(MassPoint(physicsMass = mass, nodeNumber = n1,
                                    visualization=VMassPoint(graphicsData=[gSphere])))

#marker for ground (=fixed):
groundMarker=mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= nGround, coordinate = 0))
#marker for springDamper for first (x-)coordinate:
nodeMarker  =mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= n1, coordinate = 1)) #y-coordinate

#Spring-Damper between two marker coordinates
mbs.AddObject(CoordinateSpringDamper(markerNumbers = [groundMarker, nodeMarker], 
                                     stiffness = spring, damping = damper, 
                                     springForceUserFunction = springForce,
                                     visualization=VCoordinateSpringDamper(show=False))) 

#add load:
loadC = mbs.AddLoad(LoadCoordinate(markerNumber = nodeMarker, 
                           load = load0))


if exudynTestGlobals.useGraphics:
    sPos = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Position, 
                             fileName="solution/sensorPos.txt"))
    sVel = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Velocity, 
                             fileName="solution/sensorVel.txt"))
    sAcc = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Acceleration, 
                             fileName="solution/sensorAcc.txt"))
    #dummy, for PlotSensor
    sPosRef = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Position, 
                                       writeToFile=doRefSol, #set True to compute reference solution
                                        fileName="solution/sensorPosRef.txt"))
    sVelRef = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Velocity, 
                                       writeToFile=doRefSol, #set True to compute reference solution
                                        fileName="solution/sensorVelRef.txt"))
    sAccRef = mbs.AddSensor(SensorNode(nodeNumber=n1, outputVariableType=exu.OutputVariableType.Acceleration, 
                                       writeToFile=doRefSol, #set True to compute reference solution
                                        fileName="solution/sensorAccRef.txt"))

#exu.Print(mbs)
mbs.Assemble()

simulationSettings = exu.SimulationSettings()
simulationSettings.solutionSettings.writeSolutionToFile = False
simulationSettings.solutionSettings.sensorsWritePeriod = 1e-5
simulationSettings.timeIntegration.numberOfSteps = int(tEnd/h)
simulationSettings.timeIntegration.endTime = tEnd
simulationSettings.timeIntegration.minimumStepSize = 1e-10

#important settings for contact:
simulationSettings.timeIntegration.discontinuous.iterationTolerance = 1e-8 #this is the accepted penetration before reducing step size
simulationSettings.timeIntegration.discontinuous.maxIterations = 1 #immediately perform step reduction
simulationSettings.timeIntegration.discontinuous.ignoreMaxIterations = False #repeat step in case of failure
simulationSettings.timeIntegration.adaptiveStepRecoverySteps = 0 #number of steps to wait until step size is increased again
simulationSettings.timeIntegration.adaptiveStepIncrease = 10    #after successful step, increase again rapidly


simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 1

simulationSettings.displayStatistics = True
simulationSettings.timeIntegration.verboseMode = 1
#simulationSettings.timeIntegration.simulateInRealtime = True

if exudynTestGlobals.useGraphics:
    exu.StartRenderer()              #start graphics visualization
    #mbs.WaitForUserToContinue()    #wait for pressing SPACE bar to continue

#start solver:
exu.SolveDynamic(mbs, solverType=exu.DynamicSolverType.TrapezoidalIndex2, simulationSettings=simulationSettings)
#exu.SolveDynamic(mbs, solverType=exu.DynamicSolverType.RK67, simulationSettings=simulationSettings)

if exudynTestGlobals.useGraphics:
    #SC.WaitForRenderEngineStopFlag()#wait for pressing 'Q' to quit
    exu.StopRenderer()               #safely close rendering window!

u = mbs.GetNodeOutput(n1, exu.OutputVariableType.Position)
exu.Print('postNewtonStepContactTest=',u[1])

exudynTestGlobals.testError = u[1] - (0.057286638346409235) 
exudynTestGlobals.testResult = u[1]


if exudynTestGlobals.useGraphics:
    from exudyn.plot import PlotSensor
    import matplotlib.pyplot as plt
    plt.close('all')

    plt.figure('Pos')
    PlotSensor(mbs, sensorNumbers=[sPos, sPosRef], components=[1,1])
    plt.figure('Vel')
    PlotSensor(mbs, sensorNumbers=[sVel, sVelRef], components=[1,1])
    plt.figure('Acc')
    PlotSensor(mbs, sensorNumbers=[sAcc, sAccRef], components=[1,1])




