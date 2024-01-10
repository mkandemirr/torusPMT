#ifndef PmtFittedCylindricPVBuilder_h
#define PmtFittedCylindricPVBuilder_h 1

#include "VComponentBuilder.hh"
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PmtFittedCylindricPVBuilder : public VComponentBuilder<G4VPhysicalVolume> {

  public:
    PmtFittedCylindricPVBuilder(const G4String& pmtModel); 
    ~PmtFittedCylindricPVBuilder();
    
  protected:  
    virtual G4VPhysicalVolume* Build() override; 
  
  private:
    G4String mPmtModel;    
};

#endif

