#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Heavy top example
#           Refs.:  Terze, Z., Müller, A., Zlatar, D.: Singularity-free time integration of rotational quaternions using non-redundant ordinary differential equations. Multibody System Dynamics 38(3),201–225 (2016)
#
# Author:   Johannes Gerstmayr
# Date:     2020-02-02
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
import sys
sys.path.append('../../bin/WorkingRelease') #for exudyn, itemInterface and exudynUtilities
sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test
from modelUnitTests import ExudynTestStructure, exudynTestGlobals

import exudyn as exu
#from TestModels.modelUnitTests import RunAllModelUnitTests, TestInterface
from exudynUtilities import *
from itemInterface import *

import numpy as np

SC = exu.SystemContainer()
mbs = SC.AddSystem()

print('EXUDYN version='+exu.__version__)

#background
#rect = [-0.1,-0.1,0.1,0.1] #xmin,ymin,xmax,ymax
#background0 = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[rect[0],rect[1],0, rect[2],rect[1],0, rect[2],rect[3],0, rect[0],rect[3],0, rect[0],rect[1],0]} #background
color = [0.1,0.1,0.8,1]
r = 0.5 #radius
L = 1   #length


background0 = GraphicsDataRectangle(-L,-L,L,L,color)
oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0], visualization=VObjectGround(graphicsData= [background0])))

#heavy top is fixed at [0,0,0] (COM of simulated body), but force is applied at [0,1,0] (COM of real top)
m = 15
Jxx=0.234375
Jyy=0.46875
Jzz=0.234375
#yS = 1 #distance from 

#vector to COM, where force is applied
rp = [0.,1.,0.]
rpt = np.array(Vec2Tilde(rp))
#Fg = [0,0,-150*m*9.81]
Fg = [0,0,-m*9.81]
#inertia tensor w.r.t. fixed point
JFP = np.diag([Jxx,Jyy,Jzz]) - m*np.dot(rpt,rpt)
#print(JFP)

omega0 = [0,150,-4.61538] #arbitrary initial angular velocity
omegaP = m*9.81*rp[1]/(Jyy*omega0[1])
#print('omega precession:',omegaP)

ep0 = eulerParameters0 #no rotation
ep_t0 = AngularVelocity2EulerParameters_t(omega0, ep0)
#print('ep0=',ep0)
#print('ep_t0=', ep_t0)

p0 = [0,0,0] #reference position
v0 = [0.,0.,0.] #initial translational velocity

nRB = mbs.AddNode(NodeRigidBodyEP(referenceCoordinates=p0+ep0, initialVelocities=v0+list(ep_t0)))
oGraphics = GraphicsDataOrthoCube(-r/2,-L/2,-r/2, r/2,L/2,r/2, [0.1,0.1,0.8,1])
oRB = mbs.AddObject(ObjectRigidBody(physicsMass=m, physicsInertia=[JFP[0][0], JFP[1][1], JFP[2][2], JFP[1][2], JFP[0][2], JFP[0][1]], 
                                    nodeNumber=nRB, visualization=VObjectRigidBody(graphicsData=[oGraphics])))

mMassRB = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oRB, localPosition=[0,1,0])) #this is the real COM
mbs.AddLoad(Force(markerNumber = mMassRB, loadVector=Fg)) 

nPG=mbs.AddNode(PointGround(referenceCoordinates=[0,0,0])) #for coordinate constraint
mCground = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nPG, coordinate=0)) #coordinate number does not matter

mC0 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nRB, coordinate=0)) #ux
mC1 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nRB, coordinate=1)) #uy
mC2 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nRB, coordinate=2)) #uz
mbs.AddObject(CoordinateConstraint(markerNumbers=[mCground, mC0]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mCground, mC1]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mCground, mC2]))

if exudynTestGlobals.useGraphics:
    #mbs.AddSensor(SensorNode(nodeNumber=nRB, fileName='solution/sensorRotation.txt', outputVariableType=exu.OutputVariableType.Rotation))
    mbs.AddSensor(SensorNode(nodeNumber=nRB, fileName='solution/sensorAngVelLocal.txt', outputVariableType=exu.OutputVariableType.AngularVelocityLocal))
    #mbs.AddSensor(SensorNode(nodeNumber=nRB, fileName='solution/sensorAngVel.txt', outputVariableType=exu.OutputVariableType.AngularVelocity))
    
    mbs.AddSensor(SensorBody(bodyNumber=oRB, fileName='solution/sensorPosition.txt', localPosition=rp, outputVariableType=exu.OutputVariableType.Position))
    mbs.AddSensor(SensorNode(nodeNumber=nRB, fileName='solution/sensorCoordinates.txt', outputVariableType=exu.OutputVariableType.Coordinates))

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
mbs.Assemble()
#print(mbs)

simulationSettings = exu.SimulationSettings() #takes currently set values or default values

fact = 2000
simulationSettings.timeIntegration.numberOfSteps = 1*fact
simulationSettings.timeIntegration.endTime = 0.0001*fact
#simulationSettings.solutionSettings.solutionWritePeriod = simulationSettings.timeIntegration.endTime/fact
simulationSettings.solutionSettings.sensorsWritePeriod = simulationSettings.timeIntegration.endTime/fact

simulationSettings.timeIntegration.verboseMode = 1

simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = True
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = True
#simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.6 #0.6 works well 

if exudynTestGlobals.useGraphics:
    exu.StartRenderer()
    mbs.WaitForUserToContinue()

SC.TimeIntegrationSolve(mbs, 'GeneralizedAlpha', simulationSettings)

if exudynTestGlobals.useGraphics:
    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

sol = mbs.systemData.GetODE2Coordinates(); 
solref = mbs.systemData.GetODE2Coordinates(configuration=exu.ConfigurationType.Reference); 
#print('sol=',sol)
u = 0
for i in range(4):
    u += abs(sol[3+i]+solref[3+i]); #Euler parameters

print('solution of heavy top =',u)
# EP ref solution MATLAB: at t=0.2
#  gen alpha (sigma=0.98, h=1e-4): -0.70813,0.43881,0.54593,0.089251 ==> abs sum=1.782121
#  RK4:                            -0.70828,0.43878,0.54573,0.0894   ==> abs sum=1.78219
#Exudyn: (index2)                  -1.70824157  0.43878143  0.54578152   0.08937154

exudynTestGlobals.testError = u - (1.7821760506326125) #2020-02-03: 



#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#compute exact solution:

if exudynTestGlobals.useGraphics:
    import matplotlib.pyplot as plt
    import matplotlib.ticker as ticker
    plt.close("all")
    
    [fig1, ax1] = plt.subplots()
    [fig2, ax2] = plt.subplots()
    [fig3, ax3] = plt.subplots()
    data1 = np.loadtxt('solution/sensorCoordinates.txt', comments='#', delimiter=',')
    ax1.plot(data1[:,0], data1[:,1+3]+1, 'r-', label='theta 0')  #1, because coordinates to not include ref. values
    ax1.plot(data1[:,0], data1[:,2+3], 'g-', label='theta 1') 
    ax1.plot(data1[:,0], data1[:,3+3], 'b-', label='theta 2') 
    ax1.plot(data1[:,0], data1[:,4+3], 'k-', label='theta 3') 
    
    data1 = np.loadtxt('../../docs/verification/HeavyTopSolution/HeavyTop_TimeEulerParameter_RK4.txt', comments='#', delimiter=',')
    ax1.plot(data1[:,0], data1[:,1], 'r:', label='theta 0 ref')  #1, because coordinates to not include ref. values
    ax1.plot(data1[:,0], data1[:,2], 'g:', label='theta 1 ref') 
    ax1.plot(data1[:,0], data1[:,3], 'b:', label='theta 2 ref') 
    ax1.plot(data1[:,0], data1[:,4], 'k:', label='theta 3 ref') 
    ax1.set_ylabel("Euler parameter")
    
    data2 = np.loadtxt('solution/sensorAngVel.txt', comments='#', delimiter=',')
    ax2.plot(data2[:,0], data2[:,1], 'r-', label='omega X') 
    ax2.plot(data2[:,0], data2[:,2], 'g-', label='omega Y') 
    ax2.plot(data2[:,0], data2[:,3], 'b-', label='omega Z') 
    
    data3 = np.loadtxt('solution/sensorPosition.txt', comments='#', delimiter=',')
    ax3.plot(data3[:,0], data3[:,1], 'r-', label='position X') 
    ax3.plot(data3[:,0], data3[:,2], 'g-', label='position Y') 
    ax3.plot(data3[:,0], data3[:,3], 'b-', label='position Z') 
    
    axList=[ax1,ax2,ax3]
    figList=[fig1, fig2, fig3]
    
    for ax in axList:
        ax.grid(True, 'major', 'both')
        ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
        ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
        ax.set_xlabel("time (s)")
        ax.legend()
        
    ax2.set_ylabel("angular velocity (rad/s)")
    ax3.set_ylabel("coordinate (m)")
    
    for f in figList:
        f.tight_layout()
        f.show() #bring to front
    





