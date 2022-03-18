#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Example of double pendulum with Mass points and CoordinateVectorConstraint;
#
# Author:   Johannes Gerstmayr
# Date:     2022-03-17
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *
import numpy as np

from modelUnitTests import ExudynTestStructure, exudynTestGlobals

SC = exu.SystemContainer()
mbs = SC.AddSystem()

doublePendulum = True
withUserFunction = True

L = 0.8 #length of arm
mass = 2.5
g = 9.81

r = 0.05 #just for graphics
d = r/2

#add ground object and mass point:
sizeRect = 1.2*L*(1+int(doublePendulum))
#graphicsBackground = GraphicsDataRectangle(-sizeRect,-sizeRect, sizeRect, 0.2*L, [1,1,1,1]) #for appropriate zoom
graphicsBackground = GraphicsDataCheckerBoard(point=[0,-0.5*sizeRect,-2*r],size=sizeRect*1.8)

oGround = mbs.AddObject(ObjectGround(referencePosition = [0,0,0], 
                           visualization = VObjectGround(graphicsData = [graphicsBackground])))


graphicsSphere = GraphicsDataSphere(point=[0,0,0], radius=r, color=color4steelblue, nTiles = 16)

nR0 = mbs.AddNode(Point2D(referenceCoordinates=[L,0]))
oR0 = mbs.AddObject(MassPoint2D(nodeNumber=nR0, physicsMass=mass, visualization=VMassPoint2D(graphicsData=[graphicsSphere])))

mGround0 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oGround, localPosition = [0,0,0]))
mTip0 = mbs.AddMarker(MarkerNodePosition(nodeNumber=nR0))

if not withUserFunction: #with internal terms:
    oCD0 = mbs.AddObject(DistanceConstraint(markerNumbers=[mGround0, mTip0], distance=L))
else:
    #just for drawing, with inactive connector:
    mbs.AddObject(DistanceConstraint(markerNumbers=[mGround0, mTip0], distance=L, activeConnector=False))

    nGround = mbs.AddNode(NodePointGround())
    mCoordsGround = mbs.AddMarker(MarkerNodeCoordinates(nodeNumber=nGround))
    mCoords0 = mbs.AddMarker(MarkerNodeCoordinates(nodeNumber=nR0))
    
    # mbs.AddObject(CoordinateVectorConstraint(markerNumbers=[mCoordsGround, mCoords0],
    #                                           scalingMarker0=[], scalingMarker1=[],
    #                                           quadraticTermMarker0=[], quadraticTermMarker1=np.array([[1,1]]),
    #                                           offset=[L**2],
    #                                           visualization=VCoordinateVectorConstraint(show=False)))
    def UFconstraint(mbs, t, itemNumber, q, q_t,velocityLevel):
        #print("q=", q, ", q_t=", q_t)
        return [np.sqrt(q[0]**2 + q[1]**2) - L]

    def UFjacobian(mbs, t, itemNumber, q, q_t,velocityLevel):
        #print("q=", q, ", q_t=", q_t)
        jac  = np.zeros((1,2))

        f = np.sqrt(q[0]**2 + q[1]**2)
        jac[0,0] = q[0]/f
        jac[0,1] = q[1]/f
        return jac
        
    mbs.AddObject(CoordinateVectorConstraint(markerNumbers=[mCoordsGround, mCoords0],
                                             scalingMarker0=np.zeros((1,2)), #needed to define number of algebraic equations; rows=nAE, cols=len(q) of mCoordsGround + mCoords0
                                             constraintUserFunction=UFconstraint,
                                             jacobianUserFunction=UFjacobian,
                                             visualization=VCoordinateVectorConstraint(show=False)))
        


#
mbs.AddLoad(Force(markerNumber = mTip0, loadVector = [0, -mass*g, 0])) 

fileNameDouble = 'solution/coordVecConstraintRefDouble.txt'
fileNameSingle = 'solution/coordVecConstraintRefSingle.txt'
    
sPos0 = mbs.AddSensor(SensorNode(nodeNumber = nR0, storeInternal = True,
                                 #fileName=fileNameSingle, #single pendulum
                                 outputVariableType=exu.OutputVariableType.Position))



if doublePendulum:
    graphicsSphere = GraphicsDataSphere(point=[0,0,0], radius=r, color=color4red, nTiles = 16)
    nR1 = mbs.AddNode(Point2D(referenceCoordinates=[L*2,0]))
    oR1 = mbs.AddObject(MassPoint2D(nodeNumber=nR1, physicsMass=mass, visualization=VMassPoint2D(graphicsData=[graphicsSphere])))
    
    mTip1 = mbs.AddMarker(MarkerNodePosition(nodeNumber=nR1))

    if not withUserFunction: #with internal terms:
        oCD1 = mbs.AddObject(DistanceConstraint(markerNumbers=[mTip0, mTip1], distance=L))
    else:
        #just for drawing, with inactive connector:
        mbs.AddObject(DistanceConstraint(markerNumbers=[mTip0, mTip1], distance=L, activeConnector=False))

        mCoords0 = mbs.AddMarker(MarkerNodeCoordinates(nodeNumber=nR0))
        mCoords1 = mbs.AddMarker(MarkerNodeCoordinates(nodeNumber=nR1))
 
        def UFconstraint2(mbs, t, itemNumber, q, q_t,velocityLevel):
            #print("q=", q, ", q_t=", q_t)
            return [np.sqrt((q[2]-q[0])**2 + (q[3]-q[1])**2) - L]

        def UFjacobian2(mbs, t, itemNumber, q, q_t,velocityLevel):
            #print("q=", q, ", q_t=", q_t)
            jac  = np.zeros((1,4))
            f = np.sqrt((q[2]-q[0])**2 + (q[3]-q[1])**2)
            jac[0,0] =-(q[2]-q[0])/f
            jac[0,1] =-(q[3]-q[1])/f
            jac[0,2] = (q[2]-q[0])/f
            jac[0,3] = (q[3]-q[1])/f
            return jac
            
        mbs.AddObject(CoordinateVectorConstraint(markerNumbers=[mCoords0, mCoords1],
                                                 scalingMarker0=np.zeros((1,2+2)), #needed to define number of algebraic equations; rows=nAE, cols=len(q) of mCoordsGround + mCoords0
                                                 constraintUserFunction=UFconstraint2,
                                                 jacobianUserFunction=UFjacobian2,
                                                 visualization=VCoordinateVectorConstraint(show=False)))

 
    #
    mbs.AddLoad(Force(markerNumber = mTip1, loadVector = [0, -mass*g, 0])) 
    
    sPos1 = mbs.AddSensor(SensorNode(nodeNumber = nR1, storeInternal = True,
                                     #fileName=fileNameDouble,
                                     outputVariableType=exu.OutputVariableType.Position))
    


mbs.Assemble()

simulationSettings = exu.SimulationSettings()

# exudynTestGlobals.useGraphics=False
tEnd = 1
h = 1e-3
if exudynTestGlobals.useGraphics:
    tEnd = 1
    simulationSettings.timeIntegration.simulateInRealtime = True
    simulationSettings.timeIntegration.realtimeFactor = 3
    
simulationSettings.timeIntegration.numberOfSteps = int(tEnd/h)
simulationSettings.timeIntegration.endTime = tEnd

#simulationSettings.solutionSettings.solutionWritePeriod = h
simulationSettings.timeIntegration.verboseMode = 1
#simulationSettings.solutionSettings.solutionWritePeriod = tEnd/steps

simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.8 #SHOULD work with 0.9 as well

SC.visualizationSettings.nodes.showBasis=True

if exudynTestGlobals.useGraphics:
    exu.StartRenderer()
    mbs.WaitForUserToContinue()

exu.SolveDynamic(mbs, simulationSettings)

p0=mbs.GetObjectOutputBody(oR0, exu.OutputVariableType.Position, localPosition=[0,0,0])
exu.Print("p0=", list(p0))
u=sum(p0)

exu.Print('solution of coordinateVectorConstraint=',u)

exudynTestGlobals.testError = u - (-1.0825265797698322)
exudynTestGlobals.testResult = u


if exudynTestGlobals.useGraphics:
    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

    from exudyn.plot import PlotSensor
    
    if doublePendulum:
        PlotSensor(mbs, [sPos0,sPos0,sPos1,sPos1], components=[0,1,0,1], closeAll=True)
    else:
        PlotSensor(mbs, [sPos0,sPos0], components=[0,1], closeAll=True)

    #if reference solution computed:
    # if doublePendulum:
    #     PlotSensor(mbs, [sPos0,sPos0,sPos1,sPos1,fileNameDouble], components=[0,1,0,1,1], closeAll=True, 
    #                markerStyles=['','','','','x'], lineStyles=['-','-','-','-',''])
    # else:
    #     PlotSensor(mbs, [sPos0,sPos0,fileNameSingle], components=[0,1,1], closeAll=True, 
    #                markerStyles=['','','x'], lineStyles=['-','-',''])



