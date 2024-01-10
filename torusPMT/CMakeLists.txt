set(headers ${CMAKE_CURRENT_SOURCE_DIR}/include/GLG4TorusStack.hh         
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTLVBuilder.hh      
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTMaterialFactory.hh
            ${CMAKE_CURRENT_SOURCE_DIR}/include/local_g4compat.hh         
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTLVBuilderMsgr.hh  
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTOSFactory.hh
            ${CMAKE_CURRENT_SOURCE_DIR}/include/OpticalSurfaceBuilder.hh  
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTLVFactory.hh      
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ToroidalPMTPVBuilder.hh
            )
  
set(sources ${CMAKE_CURRENT_SOURCE_DIR}/src/GLG4TorusStack.cc         
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTLVBuilderMsgr.cc    
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTOSFactory.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/OpticalSurfaceBuilder.cc  
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTLVFactory.cc        
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTPVBuilder.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTLVBuilder.cc           
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ToroidalPMTMaterialFactory.cc
            )  


set(dirName torusPMT) #do not change!

add_library(${dirName} ${headers} ${sources})
target_include_directories(${dirName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${dirName} PRIVATE g4dcp OPSim ${Geant4_LIBRARIES})

file(GLOB_RECURSE filePathList_data #results variable
                  RELATIVE # keyword to specify relative path
                  ${CMAKE_CURRENT_SOURCE_DIR} # relative to this path
                  data/* # global expression. all files under this directory
                  )
                  
file(GLOB_RECURSE filePathList_macros 
                  RELATIVE 
                  ${CMAKE_CURRENT_SOURCE_DIR} 
                  macros/*
                  )
                  
#message("Relative file path list (relative path is specified with " 
#        "the RELATIVE keyword) : ${filePathList_data}"
#       )

foreach(_filePath ${filePathList_data} ${filePathList_macros})

#message("Copying file from ${CMAKE_CURRENT_SOURCE_DIR}/${_filePath} "
#        "to ${CMAKE_BINARY_DIR}/${dirName}/${_filePath}....")

#Copy a file to another location and modify its contents
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${_filePath} #input file path
  ${CMAKE_BINARY_DIR}/${dirName}/${_filePath} # output file path, do not change!
  # since macro files read from this path!
  COPYONLY
  )

endforeach()
