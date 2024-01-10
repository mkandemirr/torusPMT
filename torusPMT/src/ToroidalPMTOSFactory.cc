#include "ToroidalPMTOSFactory.hh"
#include "G4OpticalSurface.hh"
#include "OpticalSurfaceBuilder.hh"

ToroidalPMTOSFactory* ToroidalPMTOSFactory::mInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTOSFactory::ToroidalPMTOSFactory():
  VComponentFactory()
{
  mMacrosDirectoryPath = "torusPMT/macros/os/"; //relative to CMAKE_BINARY_DIR
  
  mProductsMap.insert(std::make_pair("r5912", nullptr)); 
  mProductsMap.insert(std::make_pair("r14688", nullptr)); 
  mProductsMap.insert(std::make_pair("r1408", nullptr)); 
  mProductsMap.insert(std::make_pair("r11065", nullptr));
  mProductsMap.insert(std::make_pair("r11780", nullptr));
  mProductsMap.insert(std::make_pair("r7081", nullptr));
  mProductsMap.insert(std::make_pair("r3600", nullptr));
  mProductsMap.insert(std::make_pair("et9390b", nullptr));
   
  mProductsMap.insert(std::make_pair("mirror", nullptr)); 
  mProductsMap.insert(std::make_pair("stainlessSteel", nullptr));  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTOSFactory::~ToroidalPMTOSFactory()
{   
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4OpticalSurface* ToroidalPMTOSFactory::Create(const G4String& name)
{
  //Because there is no a container class for G4OpticalSurface objects(
  //like a G4LogicalVolumeStore for G4LogicalVolume objects),
  //we can not check whether an optical surface created before. So,
  //we create a map. Now, a product will only be created once.
  
  if (mProductsMap.find(name) != mProductsMap.end() ) 
  {
    if (mProductsMap.at(name) != nullptr)
      return mProductsMap.at(name); 
    
    mProductsMap.at(name) = CreateSurface(name, true);
    return mProductsMap.at(name);
  }else //not found
  {
    return nullptr;
  }
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTOSFactory* ToroidalPMTOSFactory::GetInstance()
{
  static ToroidalPMTOSFactory instance;
  
  if (mInstance == nullptr)
  {
    mInstance = &instance;
  }
    
  return mInstance;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4OpticalSurface* ToroidalPMTOSFactory::CreateSurface
(
  const G4String& name, 
  G4bool enableMatProp
)
{
  G4String macroFileName = name + "_os.mac";
  G4String macroFilePath = mMacrosDirectoryPath + macroFileName; 
  
  OpticalSurfaceBuilder builder(name, macroFilePath, enableMatProp);
  return builder.GetProduct();  
}

