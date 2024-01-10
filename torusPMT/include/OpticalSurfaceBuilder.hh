
////////////////////////////////////////////////////////////////////////
// OpticalSurfaceBuilder Definition
////////////////////////////////////////////////////////////////////////
//
// File:        OpticalSurfaceBuilder.hh
// Description: This class is used to build optical surface that all 
// properties of the surface including its mpt can be controlled via user 
// interface commands. This class uses OPSim tool.
// Version:     1.0
// Created:     19.12.2023
// Author:      Mustafa Kandemir
// Updated:     
//
////////////////////////////////////////////////////////////////////////

#ifndef OpticalSurfaceBuilder__
#define OpticalSurfaceBuilder__

#include "VComponentBuilder.hh"

class MaterialPropertiesTable;
class G4OpticalSurface;

class OpticalSurfaceBuilder : public VComponentBuilder<G4OpticalSurface> {
  public:
    OpticalSurfaceBuilder(const G4String& pSurfaceName, 
                          const G4String& pMacroFilePath,
                          G4bool pEnableMatProp = true
                         );
      
    ~OpticalSurfaceBuilder();
   
  protected:
    virtual G4OpticalSurface* Build() override;
    
  private:
    MaterialPropertiesTable* mMpt;
    G4String mSurfaceName;
    G4String mMacroFilePath;
    G4bool mEnableMatProp;
            
};

#endif
