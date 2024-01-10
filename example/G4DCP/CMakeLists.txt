set(sources VComponentBuilder.hh 
            VComponentFactory.hh  
            )
            
add_library(g4dcp ${sources})
target_include_directories(g4dcp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(g4dcp PRIVATE ${Geant4_LIBRARIES}) 

#this is required, because there is no cpp file 
set_target_properties(g4dcp PROPERTIES LINKER_LANGUAGE CXX) 
                                                    
