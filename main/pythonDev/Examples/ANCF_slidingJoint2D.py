#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  ANCF Cable2D element with sliding joint test
#
# Author:   Johannes Gerstmayr
# Date:     2019-09-15
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import exudyn as exu
from exudyn.itemInterface import *

SC = exu.SystemContainer()
mbs = SC.AddSystem()

#background
rect = [-2.5,-2,2.5,1] #xmin,ymin,xmax,ymax
background0 = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[rect[0],rect[1],0, rect[2],rect[1],0, rect[2],rect[3],0, rect[0],rect[3],0, rect[0],rect[1],0]} #background
background1 = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[0,-1,0, 2,-1,0]} #background
oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0], visualization=VObjectGround(graphicsData= [background0])))


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#cable:
mypi = 3.141592653589793

L=2                     # length of ANCF element in m
#L=mypi                 # length of ANCF element in m
E=2.07e11               # Young's modulus of ANCF element in N/m^2
rho=7800                # density of ANCF element in kg/m^3
b=0.001                 # width of rectangular ANCF element in m
h=0.001                 # height of rectangular ANCF element in m
A=b*h                   # cross sectional area of ANCF element in m^2
I=b*h**3/12             # second moment of area of ANCF element in m^4
f=3*E*I/L**2            # tip load applied to ANCF element in N
g=9.81

print("load f="+str(f))
print("EI="+str(E*I))

nGround = mbs.AddNode(NodePointGround(referenceCoordinates=[0,0,0])) #ground node for coordinate constraint
mGround = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nGround, coordinate=0)) #Ground node ==> no action

cableList=[]        #for cable elements
nodeList=[]  #for nodes of cable
markerList=[]       #for nodes
nc0 = mbs.AddNode(Point2DS1(referenceCoordinates=[0,0,1,0]))
nodeList+=[nc0]
nElements = 3
lElem = L / nElements
for i in range(nElements):
    nLast = mbs.AddNode(Point2DS1(referenceCoordinates=[lElem*(i+1),0,1,0]))
    nodeList+=[nLast]
    elem=mbs.AddObject(Cable2D(physicsLength=lElem, physicsMassPerLength=rho*A, 
                               physicsBendingStiffness=E*I, physicsAxialStiffness=E*A, nodeNumbers=[int(nc0)+i,int(nc0)+i+1]))
    cableList+=[elem]
    mBody = mbs.AddMarker(MarkerBodyMass(bodyNumber = elem))
    mbs.AddLoad(Gravity(markerNumber=mBody, loadVector=[0,-g,0]))

addPointMass = False
if addPointMass:
    massTip = 0.01 #tip mass
    nMass = mbs.AddNode(Point2D(referenceCoordinates=[L,0],visualization=VNodePoint2D(drawSize=0.3)))
    mTip0 = mbs.AddMarker(MarkerNodePosition(nodeNumber=nMass))
    mTip1 = mbs.AddMarker(MarkerNodePosition(nodeNumber=nLast))
    mbs.AddObject(MassPoint2D(physicsMass = massTip, nodeNumber=nMass))
    mbs.AddLoad(Force(markerNumber=mTip0, loadVector=[0,-massTip*g,0]))
    mbs.AddObject(RevoluteJoint2D(markerNumbers=[mTip0,mTip1]))


mANCF0 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = int(nc0)+1*0, coordinate=0))
mANCF1 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = int(nc0)+1*0, coordinate=1))
mANCF2 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = int(nc0)+1*0, coordinate=3))
    
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF0]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF1]))
mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF2]))

#mANCF3 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nLast, coordinate=1))
#mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF3]))
#mANCF4 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nLast, coordinate=0))
#mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCF4]))

#add gravity:
markerList=[]
for i in range(len(nodeList)):
    m = mbs.AddMarker(MarkerNodePosition(nodeNumber=nodeList[i])) 
    markerList+=[m]
    #fact = 1 #add (half) weight of two elements to node
    #if (i==0) | (i==len(nodeList)-1): 
    #    fact = 0.5 # first and last node only weighted half
    #mbs.AddLoad(Force(markerNumber = m, loadVector = [0., -rho*A*fact*lElem*g, 0])) #will be changed in load steps

#mANCFend = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber = nodeList[-1], coordinate=1)) #last marker
#mbs.AddObject(CoordinateConstraint(markerNumbers=[mGround,mANCFend]))

#mGroundTip = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[L,0,0])) 
#mbs.AddObject(CartesianSpringDamper(markerNumbers=[mGroundTip,markerList[-1]], stiffness=[10,10,10], damping=[0.1,0.1,0.1]))

#mGroundTip2 = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[L,0.2,0])) 
#mbs.AddObject(SpringDamper(markerNumbers=[mGroundTip2,markerList[-1]], stiffness=0.1, referenceLength=0.2))

#mANCFLast = mbs.AddMarker(MarkerNodePosition(nodeNumber=nLast)) #force
#mbs.AddLoad(Force(markerNumber = mANCFLast, loadVector = [0, -1e8, 0])) #will be changed in load steps

#mANCFrigid = mbs.AddMarker(MarkerBodyRigid(bodyNumber=elem, localPosition=[lElem,0,0])) #local position L = beam tip
#mbs.AddLoad(Torque(markerNumber = mANCFrigid, loadVector = [0, 0, E*I*1*mypi]))

#mANCFnode = mbs.AddMarker(MarkerNodeRigid(nodeNumber=nLast)) #local position L = beam tip
#mbs.AddLoad(Torque(markerNumber = mANCFnode, loadVector = [0, 0, 3*E*I*1*mypi]))

a = 0.1     #y-dim/2 of gondula
b = 0.001    #x-dim/2 of gondula
massRigid = 12*0.01
inertiaRigid = massRigid/12*(2*a)**2
g = 9.81    # gravity

slidingCoordinateInit = lElem*1.5 #0.75*L
initialLocalMarker = 1 #second element
if nElements<2:
    slidingCoordinateInit /= 3.
    initialLocalMarker = 0

addRigidBody = True
if addRigidBody:
    #rigid body which slides:
    graphicsRigid = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[-b,-a,0, b,-a,0, b,a,0, -b,a,0, -b,-a,0]} #drawing of rigid body
    nRigid = mbs.AddNode(Rigid2D(referenceCoordinates=[slidingCoordinateInit,-a,0], initialVelocities=[0,0,0]));
    oRigid = mbs.AddObject(RigidBody2D(physicsMass=massRigid, physicsInertia=inertiaRigid,nodeNumber=nRigid,visualization=VObjectRigidBody2D(graphicsData= [graphicsRigid])))

    markerRigidTop = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oRigid, localPosition=[0.,a,0.])) #support point
    mR2 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oRigid, localPosition=[ 0.,0.,0.])) #center of mass (for load)

    mbs.AddLoad(Force(markerNumber = mR2, loadVector = [massRigid*g*0.1, -massRigid*g, 0]))



#slidingJoint:
addSlidingJoint = True
if addSlidingJoint:
    cableMarkerList = []#list of Cable2DCoordinates markers
    offsetList = []     #list of offsets counted from first cable element; needed in sliding joint
    offset = 0          #first cable element has offset 0
    for item in cableList: #create markers for cable elements
        m = mbs.AddMarker(MarkerBodyCable2DCoordinates(bodyNumber = item))
        cableMarkerList += [m]
        offsetList += [offset]
        offset += lElem

    #mGroundSJ = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[0.*lElem+0.75*L,0.,0.])) 
    nodeDataSJ = mbs.AddNode(NodeGenericData(initialCoordinates=[initialLocalMarker,slidingCoordinateInit],numberOfDataCoordinates=2)) #initial index in cable list
    slidingJoint = mbs.AddObject(ObjectJointSliding2D(name='slider', markerNumbers=[markerRigidTop,cableMarkerList[initialLocalMarker]], 
                                                      slidingMarkerNumbers=cableMarkerList, slidingMarkerOffsets=offsetList, 
                                                      nodeNumber=nodeDataSJ))

    #print(offsetList)



#cStiffness = 1e3
#cDamping = 0.02*cStiffness
#useCircleContact = True
#if useCircleContact:
#    nSegments = 4 #number of contact segments; must be consistent between nodedata and contact element
#    initialGapList = [0.1]*nSegments #initial gap of 0.1

#    mGroundCircle = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[0.65*L,-0.5,0])) 
#    mGroundCircle2 = mbs.AddMarker(MarkerBodyPosition(bodyNumber = oGround, localPosition=[0.25*L,-0.15,0])) 

#    for i in range(len(cableList)):
#        #print("cable="+str(cableList[i]))
#        mCable = mbs.AddMarker(MarkerBodyCable2DShape(bodyNumber=cableList[i], numberOfSegments = nSegments))
#        #print("mCable="+str(mCable))
#        nodeDataContactCable = mbs.AddNode(NodeGenericData(initialCoordinates=initialGapList,numberOfDataCoordinates=nSegments))
#        mbs.AddObject(ObjectContactCircleCable2D(markerNumbers=[mGroundCircle, mCable], nodeNumber = nodeDataContactCable, 
#                                                 numberOfContactSegments=nSegments, contactStiffness = cStiffness, contactDamping=cDamping, 
#                                                 circleRadius = 0.3, offset = 0))
#        nodeDataContactCable = mbs.AddNode(NodeGenericData(initialCoordinates=initialGapList,numberOfDataCoordinates=nSegments))
#        mbs.AddObject(ObjectContactCircleCable2D(markerNumbers=[mGroundCircle2, mCable], nodeNumber = nodeDataContactCable, 
#                                                 numberOfContactSegments=nSegments, contactStiffness = cStiffness, contactDamping=cDamping, 
#                                                 circleRadius = 0.1, offset = 0))


#mbs.systemData.Info()

mbs.Assemble()
print(mbs)

simulationSettings = exu.SimulationSettings() #takes currently set values or default values
#simulationSettings.solutionSettings.coordinatesSolutionFileName = 'ANCFCable2Dbending' + str(nElements) + '.txt'

fact = 1000
simulationSettings.timeIntegration.numberOfSteps = 1*fact
simulationSettings.timeIntegration.endTime = 0.001*fact*0.5
simulationSettings.solutionSettings.writeSolutionToFile = True
simulationSettings.solutionSettings.solutionWritePeriod = simulationSettings.timeIntegration.endTime/fact
#simulationSettings.solutionSettings.outputPrecision = 4
simulationSettings.displayComputationTime = True
simulationSettings.timeIntegration.verboseMode = 1

simulationSettings.timeIntegration.newton.relativeTolerance = 1e-8*100 #10000
simulationSettings.timeIntegration.newton.absoluteTolerance = 1e-10*100

simulationSettings.timeIntegration.newton.useModifiedNewton = False
simulationSettings.timeIntegration.newton.maxModifiedNewtonIterations = 5
simulationSettings.timeIntegration.newton.numericalDifferentiation.addReferenceCoordinatesToEpsilon = False
simulationSettings.timeIntegration.newton.numericalDifferentiation.minimumCoordinateSize = 1.e-3
simulationSettings.timeIntegration.newton.numericalDifferentiation.relativeEpsilon = 1e-8 #6.055454452393343e-06*0.0001 #eps^(1/3)
simulationSettings.timeIntegration.newton.modifiedNewtonContractivity = 1e8
simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = False
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = False
simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.6 #0.6 works well 
simulationSettings.pauseAfterEachStep = False
simulationSettings.displayStatistics = True

#SC.visualizationSettings.nodes.showNumbers = True
SC.visualizationSettings.bodies.showNumbers = False
#SC.visualizationSettings.connectors.showNumbers = True
SC.visualizationSettings.nodes.defaultSize = 0.01
SC.visualizationSettings.markers.defaultSize = 0.01
SC.visualizationSettings.connectors.defaultSize = 0.01
SC.visualizationSettings.contact.contactPointsDefaultSize = 0.005
SC.visualizationSettings.connectors.showContact = 1

simulationSettings.solutionSettings.solutionInformation = "ANCF cable with imposed curvature or applied tip force/torque"

solveDynamic = True
if solveDynamic: 
    exu.StartRenderer()

    exu.SolveDynamic(mbs, simulationSettings)

    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

else:
    simulationSettings.staticSolver.newton.numericalDifferentiation.relativeEpsilon = 1e-10*100 #can be quite small; WHY?
    simulationSettings.staticSolver.verboseMode = 3
    simulationSettings.staticSolver.numberOfLoadSteps  = 20*2
    simulationSettings.staticSolver.loadStepGeometric = False;
    simulationSettings.staticSolver.loadStepGeometricRange = 5e3;

    simulationSettings.staticSolver.newton.relativeTolerance = 1e-5*100 #10000
    simulationSettings.staticSolver.newton.absoluteTolerance = 1e-10
    simulationSettings.staticSolver.newton.maxIterations = 30 #50 for bending into circle

    simulationSettings.staticSolver.discontinuous.iterationTolerance = 0.1
    #simulationSettings.staticSolver.discontinuous.maxIterations = 5
    simulationSettings.staticSolver.pauseAfterEachStep = False
    simulationSettings.staticSolver.stabilizerODE2term = 100

    exu.StartRenderer()

    exu.SolveStatic(mbs, simulationSettings)

    #sol = mbs.systemData.GetODE2Coordinates()
    #n = len(sol)
    #print('tip displacement: x='+str(sol[n-4])+', y='+str(sol[n-3])) 

    SC.WaitForRenderEngineStopFlag()
    exu.StopRenderer() #safely close rendering window!

# exu.InfoStat();




#class MyDialog:
#    def __init__(self, parent):
#        top = self.top = Toplevel(parent)
#        Label(top, text="Value").pack()
#        self.e = Entry(top)
#        self.e.pack(padx=5)
#        b = Button(top, text="OK", command=self.ok)
#        b.pack(pady=5)
#    def ok(self):
#        #print("value is " + self.e.get())
#        exec(self.e.get())
#        self.top.destroy()

#root = Tk()
#Button(root, text="Exudyn").pack()
#root.update()
#d = MyDialog(root)
#root.wait_window(d.top)

