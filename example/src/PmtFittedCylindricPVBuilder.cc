#include "PmtFittedCylindricPVBuilder.hh"
#include "G4Material.hh"
#include "PmtFittedCylindricLVBuilder.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"

#include "ToroidalPMTOSFactory.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtFittedCylindricPVBuilder::PmtFittedCylindricPVBuilder(
  const G4String& pmtModel
  ):
VComponentBuilder(),
mPmtModel{pmtModel}  
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtFittedCylindricPVBuilder::~PmtFittedCylindricPVBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PmtFittedCylindricPVBuilder::Build()
{ 

  PmtFittedCylindricLVBuilder builder(mPmtModel);
  G4LogicalVolume* outerCylinderLv = builder.GetProduct();

  G4Tubs* tubs = static_cast<G4Tubs*>(outerCylinderLv->GetSolid());

  G4double reflectorGap = 0.000001*mm;
  G4double worldSizeX = 2*tubs->GetOuterRadius() + reflectorGap;
  G4double worldSizeY = 2*tubs->GetOuterRadius() + reflectorGap;
  G4double worldSizeZ = 2*tubs->GetZHalfLength() + reflectorGap;

  G4Box* worldBox = new G4Box("OuterCylinderWorldSolid", 
                              worldSizeX*0.5, 
                              worldSizeY*0.5, 
                              worldSizeZ*0.5
                              );

  auto worldLV = new G4LogicalVolume(
    worldBox,
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"),
    "OuterCylinderWorldLv"
    );
    
  /*  
  new G4LogicalSkinSurface(
    "OuterCylinderWorldSkinSurface", 
    worldLV, 
    ToroidalPMTOSFactory::GetInstance()->Get("mirror")
    ); 
  */  
    
  G4VPhysicalVolume* outerCylPv = new G4PVPlacement(
    nullptr,              // no rotation
    G4ThreeVector(0,0,0), // at (0,0,0)
    outerCylinderLv,           // its logical volume
    "OuterCylinderPv",         // its name
    worldLV,              // its mother  volume
    false,                // no boolean operations
    0,                    // copy number
    true                 // checking overlaps
    ); 
      
  return outerCylPv;         
                     
}

