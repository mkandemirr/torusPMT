#include "ToroidalPMTPVBuilder.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "ToroidalPMTLVFactory.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTPVBuilder::ToroidalPMTPVBuilder(const G4String& modelName):
VComponentBuilder(),
mModelName{modelName}
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTPVBuilder::~ToroidalPMTPVBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ToroidalPMTPVBuilder::Build()
{     
  G4LogicalVolume* envelopeLV = ToroidalPMTLVFactory::GetInstance()->
                                Get(mModelName);
                                
  G4Tubs* tubs = static_cast<G4Tubs*>(envelopeLV->GetSolid());
  
  //Envelope world
  G4double worldSizeX = 2*tubs->GetOuterRadius();
  G4double worldSizeY = worldSizeX;
  G4double worldSizeZ = 2*tubs->GetZHalfLength();
  
  G4Box* worldBox = new G4Box(mModelName + "_envelopeWorldSolid", 
                              worldSizeX*0.5, 
                              worldSizeY*0.5, 
                              worldSizeZ*0.5
                              );
  
  auto worldLV = new G4LogicalVolume(
    worldBox,
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"),
    mModelName + "_envelopeWorldLv"
    ); 
    
  
  G4VisAttributes* visAtt = new G4VisAttributes(G4Color(0.1,0.1,1.0,0.1));
  worldLV->SetVisAttributes(visAtt);
  //worldLV->SetVisAttributes(G4VisAttributes::Invisible)
           
  G4VPhysicalVolume* pmtEnvelopePv = new G4PVPlacement(
    nullptr,              // no rotation
    G4ThreeVector(0,0,0), // at (0,0,0)
    envelopeLV,           // its logical volume
    mModelName + "_envelopePv",         // its name
    worldLV,              // its mother  volume
    false,                // no boolean operations
    0,                    // copy number
    true                 // checking overlaps
    );   
    
  return pmtEnvelopePv;        
                                           
}

