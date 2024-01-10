# torusPMT

torusPMT is a tool that allows users to realistically model the following Hamamatsu PMTs (except et9390b) in the Geant4 simulation package:

r5912 \
r14688 \
r1408 \
r11065 \
r11780 \
r7081 \
r3600 \
et9390b (ET Enterprises electron tubes)

The tool takes its name from the shape of the PMTs. The shape has a rather complex geometry consisting of toroidal segments and cannot be derived from any of the solid classes provided by Geant4 for constructing the shape of a volume. The tool utilizes the GLG4TorusStack class, which is derived from G4VSolid, in the [GLG4Sim](https://www.phys.ksu.edu/personal/gahs/GLG4sim/) package. The tool also takes the data required for the definition of PMTs from the database of [RAT-PAC](https://github.com/rat-pac/rat-pac.git) package. The PMTs defined in this tool have been used for many years in neutrino experiments, including the Kamiokande, Super-Kamiokande, SNO, Double Chooz, and Eos experiments.

This tool is not a complete experiment application. It is developed only to help those who want to use the above PMT models in their experiments when constructing their detectors.

The figure below shows the geometry details of a toroidal PMT.

![githubPic](https://github.com/mkandemirr/torusPMT/assets/114905224/7b5fac8c-dd54-485f-8d32-925b3bdc55da)

## Dependency

This tool utilizes [OPSim](https://github.com/mkandemirr/OpSim.git) for easy simulation of optical photons, and [G4DCP](https://github.com/mkandemirr/G4DCP.git) for flexible geometry (detector) construction.


## How to use torusPMT in a Geant4 optical application?

The tool can be easily integrated into a typical Geant4 application/project as a library by copying the entire directory (directory name is torusPMT) into the project source directory where your CMakeList.txt file is located, and adding the following lines of code to the CMakeList.txt file (for help, you can examine the CMakeList.txt files in the provided example application):

before the executable target is created:

```cmake	
   add_subdirectory(torusPMT)
   add_subdirectory(OPSim)
   add_subdirectory(g4dcp)
  ```  
after the executable target is created:

```cmake	
   target_link_libraries(yourExecutableTarget OPSim g4dcp torusPMT)
  ```  

Now you can get the above PMTs as both G4LogicalVolume and G4VPhysicalVolume objects with just two lines of code! Add the following code to the Construct() method of your detector construction class to visualise the geometry:

```c++	
   #include "ToroidalPMTPVBuilder.hh"
   
   G4String pmtModel = "r5912"; 
   ToroidalPMTPVBuilder builder(pmtModel);
   G4VPhysicalVolume* pmtEnvelopePv = builder.GetProduct();
   return pmtEnvelopePv;
  ```  

or to obtain a PMT as a G4LogicalVolume object:

```c++	
   #include "ToroidalPMTLVFactory.hh"
   G4LogicalVolume* pmtLv = ToroidalPMTLVFactory::GetInstance()->Get(modelName)
  ```
              
## The package structure:   

_torusPMT/include_: This directory includes all header files (.h) of torusPmt.

_torusPMT/src_: This directory includes all source files (.cc) of torusPmt;

_torusPMT/data_: This directory contains data on the optical properties of the materials and surfaces defined to construct a toroidal PMT. The data is categorized according to material optical properties and surface optical properties.  

_torusPMT/data/material_mpt_: This directory contains data files for the optical properties of materials defined to construct a toroidal PMT. The materials defined to construct a torusPmt are glass, quartz, pmtVacuum, and stainless steel.

_torusPMT/data/surface_mpt_: This directory contains data files for the optical properties of surfaces defined to construct toroidal PMTs. The defined optical surfaces are reflector surfaces and photocathode surfaces.

_torusPMT/macros_: This directory contains macro files (file with .mac extension) to control parameters defined to construct PMTs. All the parameters can be changed at run time without recompilation of the code.

_torusPMT/macros/mpt_: This directory contains macro files to control material optical properties such as refractive index, absorption length, etc..

_torusPMT/macros/os_: This directory contains macro files to control surface optical properties such as reflector reflectivity and PMT photocathode quantum efficiency. 
 
_torusPMT/macros/pmts_: This directory contains macro files to control the geometry construction parameters of PMTs.

_torusPMT/macros/vis_: This directory contains macro files to control visual parameters such as visibility and colors.

_example_: This directory contains a sample Geant4 application using toroidal PMTs in the detector geometry. To test the PMTs, a toy Cherenkov detector surrounded by PMTs was constructed, and a sensitive detector was defined to count the photons detected. The program prints out the photon detection time and the number of photons detected by each PMT. Other classes needed to complete the application (except the UserDetectorConstruction class) have been kept in their simplest form.

## Tested on 

* Os: Linux (Ubuntu 22.04.2 LTS)
* Architecture: x86_64
* Compiler: GCC 11.3.0
* Geant4.11.1.1

## How to run the provided example applications?   
  
1. Go to the directory where you installed the example directory in your system.
  ```bash	
   cd path/To/example
  ```
  
2. Create a directory named example-build in the same directory as example. 
  ```bash
   mkdir example-build
   ```

3. Now go inside this build directory and run CMake to generate the Makefiles needed to build the application. Two arguments are 
passed to CMake. The first argument to CMake is the path to the source directory that includes the CMakeList.txt file. The second argument is the path to the build directory where the generated make files will be located. CMake will now run to configure the build and generate Makefiles.
```bash
  cd example-build
  cmake -S ../example -B .
 ```
5. With the Makefile available, you can now build by simply running make: 
  ```bash
   make  
  ```
5. The application is now ready to run. If you now list the contents of your build directory, you will notice that the executable main has been created. The program can be executed in two modes: interactive and batch. To run in an interactive mode:
  ```bash
   ./main
```

The figure below shows an example event output from the program.

![ToyCherenkovDet](https://github.com/mkandemirr/torusPMT/assets/114905224/f4206c1a-7585-41df-8e95-643f41f26d54)

## Contacts 

If you have any questions or wish to notify of updates and/or modifications please contact: \
Mustafa Kandemir (mustafa.kandemir@erdogan.edu.tr)

