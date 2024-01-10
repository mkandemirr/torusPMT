#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(torusPMT LANGUAGES CXX )

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_subdirectory(G4DCP)
add_subdirectory(OPSim)
add_subdirectory(torusPMT)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
add_executable(main main.cc ${sources} ${headers})
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(main ${Geant4_LIBRARIES} OPSim g4dcp torusPMT)

#----------------------------------------------------------------------------

file(GLOB_RECURSE filePathList_data RELATIVE ${PROJECT_SOURCE_DIR} data/*)
file(GLOB_RECURSE filePathList_macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*)

foreach(_filePath ${filePathList_data} ${filePathList_macros} )

#Copy a file to another location and modify its contents        
configure_file(
  ${PROJECT_SOURCE_DIR}/${_filePath} #input file path
  ${PROJECT_BINARY_DIR}/${_filePath} # output file path
  COPYONLY
  )

endforeach()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build main. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(main_SCRIPTS init_vis.mac
                 vis.mac
                 run.mac
   )

foreach(_script ${main_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS main DESTINATION bin)
