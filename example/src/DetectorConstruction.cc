#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "PmtFittedCylindricPVBuilder.hh"
#include "ToroidalPMTPVBuilder.hh"
#include "G4SDManager.hh"
#include "PhotonSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
G4VUserDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  /* Product list
  r5912 
  r3600 
  r14688 
  r1408 
  r11065 
  r11780 
  r7081 
  et9390b 
 */
  
  G4bool isPlacedInWorld = true;
  mPmtModel = "r14688";
  
  //return ConstructSingleTorusPmt(isPlacedInWorld);
  return ConstructDet(isPlacedInWorld);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructSingleTorusPmt
(
  G4bool isPlacedInWorld
)
{
  ToroidalPMTPVBuilder builder(mPmtModel);
  G4VPhysicalVolume* pmtEnvelopePv = builder.GetProduct();
  
  if (!isPlacedInWorld)
    return pmtEnvelopePv;
    
  auto worldPV = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                         // at (0,0,0)
    pmtEnvelopePv->GetMotherLogical(),       // its logical volume
    "WorldPV",                               // its name
    nullptr,                                 // its mother  volume
    false,                                   // no boolean operation
    0,                                       // copy number
    true);                                  // overlaps checking   
  
  return worldPV;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructDet
(
  G4bool isPlacedInWorld
)
{
  PmtFittedCylindricPVBuilder builder(mPmtModel);
  G4VPhysicalVolume* outerCylPv = builder.GetProduct();   

  if (!isPlacedInWorld)
    return outerCylPv;

  auto worldPV = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                         // at (0,0,0)
    outerCylPv->GetMotherLogical(),          // its logical volume
    "WorldPV",                               // its name
    nullptr,                                 // its mother  volume
    false,                                   // no boolean operation
    0,                                       // copy number
    true);                                  // overlaps checking 

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  PhotonSD* pmt = mPhotonSD.Get();
  if (!pmt)
  {
    PhotonSD* pmt_SD = new PhotonSD("/Det/photonSD");
    mPhotonSD.Put(pmt_SD);
  }
  
  G4SDManager::GetSDMpointer()->AddNewDetector(mPhotonSD.Get());
                       
  G4String sdName = mPmtModel + "_inner1Lv";
  SetSensitiveDetector(sdName, mPhotonSD.Get(), true);

}




