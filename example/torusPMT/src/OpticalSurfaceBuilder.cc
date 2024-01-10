#include "OpticalSurfaceBuilder.hh"
#include "OpticalSurface.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalSurfaceBuilder::OpticalSurfaceBuilder(
  const G4String& pSurfaceName, 
  const G4String& pMacroFilePath,
  G4bool pEnableMatProp
  ):
VComponentBuilder(),
mMpt{nullptr},
mSurfaceName(pSurfaceName),
mMacroFilePath(pMacroFilePath),
mEnableMatProp{pEnableMatProp}
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalSurfaceBuilder::~OpticalSurfaceBuilder()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4OpticalSurface* OpticalSurfaceBuilder::Build()
{
  //macro commands are automatically defined when 
  //object of MPT and OS are created. 
  
  OpticalSurface* surface  = new OpticalSurface(mSurfaceName);
   
  if (mEnableMatProp)
  {
    mMpt = new MaterialPropertiesTable(mSurfaceName);
  }
  
  G4UImanager::GetUIpointer()->ExecuteMacroFile(mMacroFilePath);
  
  if (mEnableMatProp)
  {
    mMpt->DumpTable();
    surface->SetMaterialPropertiesTable(mMpt);
  }
    
  return surface;
}
