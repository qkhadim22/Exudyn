#+++++++++++++++++++++++++++++++++++++++++++
# Mini example for class ObjectMassPoint2D
#+++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../../bin/WorkingRelease')
sys.path.append('../TestModels')
from modelUnitTests import ExudynTestStructure, exudynTestGlobals
import exudyn as exu
from exudynUtilities import *
from itemInterface import *
import numpy as np

#create an environment for mini example
SC = exu.SystemContainer()
mbs = SC.AddSystem()

oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0]))
nGround = mbs.AddNode(NodePointGround(referenceCoordinates=[0,0,0]))

testError=1 #set default error, if failed
exu.Print("start mini example for class ObjectMassPoint2D")
try: #puts example in safe environment
    node = mbs.AddNode(NodePoint2D(referenceCoordinates = [1,1], 
                                 initialCoordinates=[0.5,0],
                                 initialVelocities=[0.5,0]))
    mbs.AddObject(MassPoint2D(nodeNumber = node, physicsMass=1))

    #assemble and solve system for default parameters
    mbs.Assemble()
    SC.TimeIntegrationSolve(mbs, 'GeneralizedAlpha', exu.SimulationSettings())

    #check result
    testError = mbs.GetNodeOutput(node, exu.OutputVariableType.Position)[0] - 2 
    #final x-coordinate of position shall be 2

except BaseException as e:
    exu.Print("An error occured in test example for ObjectMassPoint2D:", e)
else:
    exu.Print("example for ObjectMassPoint2D completed, test error =", testError)

