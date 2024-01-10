#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class PhotonSD;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();
    
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

  private:
    G4VPhysicalVolume* ConstructSingleTorusPmt(G4bool isPlacedInWorld = true);
    G4VPhysicalVolume* ConstructDet(G4bool isPlacedInWorld = false);
  private:
    G4Cache<PhotonSD*> mPhotonSD;
    
    G4String mPmtModel;
   
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
