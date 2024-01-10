#include "ToroidalPMTLVFactory.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

#include "ToroidalPMTLVBuilder.hh"
#include "G4NistManager.hh"
#include <set>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVFactory* ToroidalPMTLVFactory::mInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVFactory::ToroidalPMTLVFactory():
VComponentFactory()
{
  mMacrosDirectoryPath = "torusPMT/macros/pmts/";//relative to CMAKE_BINARY_DIR
  
  //available products in the factory.
  mProductsSet.insert("r5912");
  mProductsSet.insert("r14688");
  mProductsSet.insert("r1408");
  mProductsSet.insert("r11065");
  mProductsSet.insert("r11780");
  mProductsSet.insert("r7081");
  mProductsSet.insert("r3600");
  mProductsSet.insert("et9390b");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVFactory::~ToroidalPMTLVFactory()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* ToroidalPMTLVFactory::Create(const G4String& modelName)
{
  mLVName = modelName + "_envelopeLv";
  
  G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(
    mLVName,
    false
    );
    
  if (lv) return lv;
  
  G4String macroFilePath = mMacrosDirectoryPath + modelName + ".mac";
  
  if (mProductsSet.find(modelName) != mProductsSet.end()) 
  {
    ToroidalPMTLVBuilder builder(modelName, macroFilePath);
    return builder.GetProductInAnEnvelope();   
  }
  else // product is not available in the factory!
  {
    return nullptr;  
  }
        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVFactory* ToroidalPMTLVFactory::GetInstance()
{
  static ToroidalPMTLVFactory instance;
  
  if(mInstance == nullptr)
  {
    mInstance = &instance;
  }
    
  return mInstance;
   
}

