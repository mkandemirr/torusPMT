#ifndef ToroidalPMTPVBuilder_h
#define ToroidalPMTPVBuilder_h 1

#include "VComponentBuilder.hh"

class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ToroidalPMTPVBuilder : public VComponentBuilder<G4VPhysicalVolume> {

  public:
    ToroidalPMTPVBuilder(const G4String& modelName); 
    ~ToroidalPMTPVBuilder();
    
  protected:  
    virtual G4VPhysicalVolume* Build() override; 
  
  private:
    G4String mModelName;     
};

#endif

