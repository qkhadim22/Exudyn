#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Test with ConvexContact
#
# Author:   Peter Manzl
# Date:     2021-12-21
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test
import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *
# from exudyn.graphicsDataUtilities import *
from modelUnitTests import ExudynTestStructure, exudynTestGlobals
SC = exu.SystemContainer()
mbs = SC.AddSystem()


# create Ground and graphics: 
scb, g1, g2 = 0.5, 0.92, 0.72
graphGround = GraphicsDataCheckerBoard (point= [0,0,0], normal= [0,0,1], size= scb, color= [g1, g1, g1, 1.0],
                                        alternatingColor= [g2, g2, g2, 1.0], nTiles= 12)
oGround = mbs.AddObject(ObjectGround(referencePosition= [0,0,0], 
                                   visualization=VObjectGround(graphicsData=[graphGround])))
mGround = mbs.AddMarker(MarkerBodyRigid(bodyNumber=oGround,localPosition = [0,0,0], name='Ground'))
nGround0=mbs.AddNode(NodePointGround(referenceCoordinates = [0,0,0]))


poly = [-3.6, 0,  1.65e-02] # coefficients of the polynomial creating the rolling body
length = 0.1                # length of the roller
contour =  [[-length/2, 0]]  
x = np.linspace(start = - length/2, stop =length/2, num=51) 
for i in range(np.size(x)):
    contour+= [[x[i], np.polyval(poly, x[i])]]
contour += [ [length/2, 0]] # to create a closed contour
graphRoll = [GraphicsDataSolidOfRevolution([0,0,0], [1,0,0], contour, color4lightred[0:3]+[1],
                                          alternatingColor=color4blue, nTiles = 32)]

InertiaRoll = InertiaCylinder(density=7800, length=length, outerRadius=3e-3, axis=0) 
[nRoll, bRoll]=AddRigidBody(mainSys = mbs,
                              inertia = InertiaRoll, 
                              nodeType = str(exu.NodeType.RotationEulerParameters), 
                              position = [0,0,poly[-1]*1.2],  
                              rotationMatrix =RotationMatrixY(np.pi/16),
                              angularVelocity = RotationMatrixY(np.pi/16) @ np.array([[-1000],[0],[0]]),  # in Global coordinates
                              velocity= [0,0,0],
                              gravity = [0,0,-9.81], 
                              graphicsDataList = graphRoll) 
        
nData = mbs.AddNode(NodeGenericData(initialCoordinates=[0,0,0], numberOfDataCoordinates=3))
mRoll = mbs.AddMarker(MarkerNodeRigid(nodeNumber=nRoll))
CConvexRoll = mbs.AddObject(ObjectContactConvexRoll(markerNumbers=[mGround, mRoll], 
                        nodeNumber=nData, contactStiffness=1e3, contactDamping=1, dynamicFriction = 0.9,
                       staticFrictionOffset = 0, viscousFriction=0, exponentialDecayStatic=1e-3, 
                       frictionProportionalZone=1e-4, rollLength=length, coefficientsHull=poly, 
                       visualization={'show': True, 'color': color4lightgreen}))

sBody = mbs.AddSensor(SensorBody(bodyNumber=bRoll, #fileName='PosRoller.txt',
                                 storeInternal=True,
                                 outputVariableType=exu.OutputVariableType.Position, visualization={'show': False})) 

mbs.Assemble()

h = 5e-4   #test
tEnd = 0.1 #test
#tEnd = 0.1*20 #for simulation
sims=exu.SimulationSettings()
sims.timeIntegration.generalizedAlpha.spectralRadius=0.7
sims.timeIntegration.endTime = tEnd
sims.timeIntegration.numberOfSteps = int(tEnd/h) #original: 1e-3, fails now in Newton
sims.timeIntegration.verboseMode = 0
sims.timeIntegration.stepInformation = 3 #do not show step reduction
sims.solutionSettings.coordinatesSolutionFileName = 'solution/coordinatesSolution.txt'
# sims.timeIntegration.newton.absoluteTolerance = 1e-8
# sims.timeIntegration.newton.relativeTolerance = 1e-6

if exudynTestGlobals.useGraphics: 
    sims.timeIntegration.verboseMode = 1
    sims.timeIntegration.stepInformation = 3+128+256
    exu.StartRenderer()
    mbs.WaitForUserToContinue()
exu.SolveDynamic(mbs, sims)
if exudynTestGlobals.useGraphics: 
    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

sol = mbs.systemData.GetODE2Coordinates()
exudynTestGlobals.testResult = np.sum(sol[:2])
exu.Print('result of ConvexContactTest=',exudynTestGlobals.testResult)
# %% 
if exudynTestGlobals.useGraphics: 
    #pos = np.loadtxt('PosRoller.txt', delimiter=',', comments='#')
    pos = mbs.GetSensorStoredData(sBody)
    exu.Print('End Pos: {}'.format(pos[-1,:]))
    
    from exudyn.plot import PlotSensor
    PlotSensor(mbs,sBody,[0,1,2])
    
    
if exudynTestGlobals.useGraphics and False:
    SC.visualizationSettings.general.autoFitScene = False
    SC.visualizationSettings.general.graphicsUpdateInterval=0.02
    from exudyn.interactive import SolutionViewer
    sol = LoadSolutionFile('solution/coordinatesSolution.txt', safeMode=True)#, maxRows=100)
    print('start SolutionViewer')
    SolutionViewer(mbs, sol)

