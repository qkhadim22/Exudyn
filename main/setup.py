# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++# setup.py file for EXUDYN; generated by Johannes Gerstmayr and based on# template provided provided in pybind11# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import sys
import platform
import setuptools

from src.pythonGenerator.exudynVersion import exudynVersionString


#os.environ["CC"] = "gcc-8" #use gcc-8.4 on linux; does not work on windows
#os.environ["CXX"] = "gcc-8"


isWindows = False
isLinux = False

#detect the platform used during running the setup tool:
if platform.system() == 'Windows':
    isWindows = True
    print("platform == Windows")
if platform.system() == 'Linux':
    isLinux = True
    print("platform == Linux")

is64bits = False
is32bits = False
if platform.architecture()[0] == '64bit':
    is64bits = True
else:
    is32bits = True

addLibrary_dirs = []
#find whether 32 or 64 bits are used
if isWindows:
    if is32bits:
        addLibrary_dirs=['libs/libs32' ]
        print("architecture==32bits")
    else:
        addLibrary_dirs=['libs/libs64' ]
        print("architecture==64bits")

print("python version =",platform.python_version())

#detect python version:
pyVersionString = str(sys.version_info.major) + '.' + str(sys.version_info.minor)

__version__ = exudynVersionString
print('build Exudyn version',exudynVersionString)

class get_pybind_include(object):
    def __str__(self):
        import pybind11
        return pybind11.get_include()


ext_modules = [
    Extension(
        'exudyn.exudynCPP', #this is the C++ library inside the exudyn package
        # Alternative, but not needed, as .cpp file list is generated automatically: sort input source files to ensure bit-for-bit reproducible builds: sorted([...])
        [                 'src/Autogenerated/versionCpp.cpp',
                 'src/Graphics/GlfwClient.cpp',
                 'src/Graphics/VisualizationPrimitives.cpp',
                 'src/Graphics/VisualizationSystem.cpp',
                 'src/Graphics/VisualizationSystemContainer.cpp',
                 'src/Graphics/VisualizationSystemData.cpp',
                 'src/Graphics/VisualizationUserFunctions.cpp',
                 'src/Linalg/LinearSolver.cpp',
                 'src/Linalg/Matrix.cpp',
                 'src/Linalg/Vector.cpp',
                 'src/Main/CSystem.cpp',
                 'src/Main/MainObjectFactory.cpp',
                 'src/Main/MainSystem.cpp',
                 'src/Main/Stdoutput.cpp',
                 'src/Objects/CMarkerBodyCable2DCoordinates.cpp',
                 'src/Objects/CMarkerBodyCable2DShape.cpp',
                 'src/Objects/CMarkerBodyMass.cpp',
                 'src/Objects/CMarkerBodyPosition.cpp',
                 'src/Objects/CMarkerBodyRigid.cpp',
                 'src/Objects/CMarkerNodeCoordinate.cpp',
                 'src/Objects/CMarkerNodePosition.cpp',
                 'src/Objects/CMarkerNodeRigid.cpp',
                 'src/Objects/CMarkerNodeRotationCoordinate.cpp',
                 'src/Objects/CMarkerObjectODE2Coordinates.cpp',
                 'src/Objects/CMarkerSuperElementPosition.cpp',
                 'src/Objects/CMarkerSuperElementRigid.cpp',
                 'src/Objects/CNode1D.cpp',
                 'src/Objects/CNodeGenericData.cpp',
                 'src/Objects/CNodeGenericODE2.cpp',
                 'src/Objects/CNodePoint.cpp',
                 'src/Objects/CNodePoint2D.cpp',
                 'src/Objects/CNodePoint2DSlope1.cpp',
                 'src/Objects/CNodePointGround.cpp',
                 'src/Objects/CNodeRigidBody2D.cpp',
                 'src/Objects/CNodeRigidBodyEP.cpp',
                 'src/Objects/CNodeRigidBodyRotVecLG.cpp',
                 'src/Objects/CNodeRigidBodyRxyz.cpp',
                 'src/Objects/CObjectALEANCFCable2D.cpp',
                 'src/Objects/CObjectANCFCable2D.cpp',
                 'src/Objects/CObjectConnectorCartesianSpringDamper.cpp',
                 'src/Objects/CObjectConnectorCoordinate.cpp',
                 'src/Objects/CObjectConnectorCoordinateSpringDamper.cpp',
                 'src/Objects/CObjectConnectorCoordinateVector.cpp',
                 'src/Objects/CObjectConnectorDistance.cpp',
                 'src/Objects/CObjectConnectorRigidBodySpringDamper.cpp',
                 'src/Objects/CObjectConnectorRollingDiscPenalty.cpp',
                 'src/Objects/CObjectConnectorSpringDamper.cpp',
                 'src/Objects/CObjectContactCircleCable2D.cpp',
                 'src/Objects/CObjectContactCoordinate.cpp',
                 'src/Objects/CObjectContactFrictionCircleCable2D.cpp',
                 'src/Objects/CObjectFFRF.cpp',
                 'src/Objects/CObjectFFRFreducedOrder.cpp',
                 'src/Objects/CObjectGenericODE2.cpp',
                 'src/Objects/CObjectGround.cpp',
                 'src/Objects/CObjectJointALEMoving2D.cpp',
                 'src/Objects/CObjectJointGeneric.cpp',
                 'src/Objects/CObjectJointPrismatic2D.cpp',
                 'src/Objects/CObjectJointRevolute2D.cpp',
                 'src/Objects/CObjectJointRollingDisc.cpp',
                 'src/Objects/CObjectJointSliding2D.cpp',
                 'src/Objects/CObjectJointSpherical.cpp',
                 'src/Objects/CObjectMass1D.cpp',
                 'src/Objects/CObjectMassPoint.cpp',
                 'src/Objects/CObjectMassPoint2D.cpp',
                 'src/Objects/CObjectRigidBody.cpp',
                 'src/Objects/CObjectRigidBody2D.cpp',
                 'src/Objects/CObjectRotationalMass1D.cpp',
                 'src/Objects/VisuNodePoint.cpp',
                 'src/Pymodules/PybindModule.cpp',
                 'src/Pymodules/PyMatrixContainer.cpp',
                 'src/Solver/CSolver.cpp',
                 'src/Solver/CSolverBase.cpp',
                 'src/Solver/CSolverImplicitSecondOrder.cpp',
                 'src/Solver/CSolverStatic.cpp',
                 'src/Solver/MainSolver.cpp',
                 'src/Solver/MainSolverBase.cpp',
                 'src/Solver/StaticSolver.cpp',
                 'src/Solver/TimeIntegrationSolver.cpp',
                 'src/System/CLoad.cpp',
                 'src/System/CNode.cpp',
                 'src/System/CObjectBody.cpp',
                 'src/System/CSensor.cpp',
                 'src/System/MainNode.cpp',
                 'src/System/MainObject.cpp',
                 'src/Tests/UnitTestBase.cpp',
                 'src/Utilities/BasicFunctions.cpp',

		],
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
    			"src",
    			"include",
    			"include/pybind11",
    			"include/glfw/deps",
    			"include/glfw",
    			"include/lest",
      ],
      library_dirs = addLibrary_dirs,
		define_macros=[('NDEBUG', '1'),
						#('_MBCS', ''),
						#('_WINDLL', ''),
						('_CRT_SECURE_NO_WARNINGS', ''),
		],
        language='c++'
    ),
]

# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
#    Return a boolean indicating whether a flag name is supported on
#    the specified compiler.
    import tempfile
    import os
    with tempfile.NamedTemporaryFile('w', suffix='.cpp', delete=False) as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        fname = f.name
    try:
        compiler.compile([fname], extra_postargs=[flagname])
    except setuptools.distutils.errors.CompileError:
        return False
    finally:
        try:
            os.remove(fname)
        except OSError:
            pass
    return True


def cpp_flag(compiler):
#    Return the -std=c++[11/14/17] compiler flag.
#    The newer version is prefered over c++11 (when it is available).
    flags = ['-std=c++17', '-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag):
            return flag

    raise RuntimeError('Unsupported compiler -- at least C++11 support '
                       'is needed!')


class BuildExt(build_ext):
#    A custom build extension for adding compiler-specific options.
    c_opts = {
        #'msvc': ['/EHsc'],
        'msvc': ['/EHsc',
				'/permissive-',
				'/MP', '/GS', '/Qpar',
				'/GL', '/W3', '/Gy', 
				'/Zc:wchar_t',
				'/Zi', 
				'/Gm-', 
				'/O2', 
				'/sdl',
				'/Zc:inline',
				'/fp:precise',
			   '/D', '_MBCS', 
				'/D', '_WINDLL',
				'/D','_CRT_SECURE_NO_WARNINGS', #/D and _CRT_SECURE_NO_WARNINGS must be consecutive==>WORKS!
				'/errorReport:prompt', 
				'/WX-', 
				'/Zc:forScope',
				'/arch:AVX2',
				'/Gd', '/Oy', '/Oi', 
				#'/MD', #/MT=multithreaded, /MD=multithreaded DLL, overwirtes /MT
				'/openmp',
				'/std:c++17',
				'/FC',
            '/Ot', #favor faster code
				'/Zc:twoPhase-',
            ],
        'unix': [
         #'-std=c++17', #==>chosen automatic
         '-Wno-non-template-friend', #deactivate warning for several vector/array templates
         '-Wno-comment', #deactivate multiline comment warning /* ... * * ...*/
         #'-fpermissive', #because of exceptions ==> allows compilation
         #'-fopenmp',
    		 '-Wall',
    		 '-Wno-class-memaccess', #avoid warnings on gcc-8 regarding memory access in class
    		 #'-O3', #takes long ...
    		 #'-shared',
    		 #'-fPIC',

#		#working:
#		'-Wno-non-template-friend', #deactivate warning for several vector/array templates
#		'-Wno-comment', #deactivate multiline comment warning /* ... * * ...*/
#		'-Wall',
#		'-Wno-class-memaccess', #avoid warnings on gcc-8 regarding memory access in class
        ],
    }
    l_opts = {
        'msvc': [
        'opengl32.lib', #not needed since VS2015?
        'glfw3.lib',
        'kernel32.lib',
        'user32.lib',
        'gdi32.lib',
        'shell32.lib',
        #additional libs that may be needed in future:
        #"winspool.lib" "comdlg32.lib" "advapi32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"        
        ],
        'unix': [
			'-lgomp', #for openmp ==> needed for omp_get_num_threads()
    		#for graphics (if GLFW enabled); needs to be installed -> see setupToolsHowTo.txt:
         '-lglfw', #GLFW
    		'-lGL', #OpenGL
         '-lstdc++fs', #for autocreate directories, using std::filesystem from c++17 std
			],
    }

    if sys.platform == 'darwin':
        darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=10.7']
        c_opts['unix'] += darwin_opts
        l_opts['unix'] += darwin_opts

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')

        for ext in self.extensions:
            ext.define_macros = [('VERSION_INFO', '"{}"'.format(self.distribution.get_version()))]
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)


setup(
    name='exudyn',
    version=__version__,
    author='Johannes Gerstmayr',
    author_email='reply.exudyn@gmail.com',
    url='https://github.com/jgerstmayr/EXUDYN',
    description='EXUDYN flexible multibody dynamics simulation in C++ and Python',
    long_description="""EXUDYN
A flexible multibody dynamics systems simulation code with Python and C++
For license information see LICENSE.txt
For more information, installation and tutorials see docs/theDoc/theDoc.pdf""",
    package_dir={'':'pythonDev'},   #only add packages from that dir; must include a __init__.py file
    packages=['exudyn'],            #adds all python files (=modules) in directories with __init__.py file; this is a subdirectory to the directory provided in package_dir
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
    license = 'BSD',
    classifiers=[
        "Development Status :: 4 - Beta",
        "Programming Language :: Python :: 3",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: BSD License",
        "Operating System :: Microsoft :: Windows :: Windows 10",
        "Operating System :: POSIX :: Linux",
        "Topic :: Scientific/Engineering",
    ],
    python_requires='=='+pyVersionString+'.*', #'.*' required on UBUNTU
)