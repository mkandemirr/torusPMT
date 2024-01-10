#include "MaterialFactory.hh"
#include "G4NistManager.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialFactory* MaterialFactory::fInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialFactory::MaterialFactory():
VComponentFactory(),
mNumComponents{0},
mDensity{0.}
{
  mMacrosDirectoryPath = "macros/mpt/";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialFactory::~MaterialFactory()
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* MaterialFactory::Create(const G4String& name)
{
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(name);
  if(mat) return mat;
 
  if (name == "air")
    return CreateAir(name);
  if (name == "gdDopedWater")
    return CreateGdDopedWater(name);
  else{
    return nullptr;
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* MaterialFactory::CreateAir(const G4String& name)
{
	// --- Air  N=70% O=30% ---------
  mDensity       = 1.29e-3*g/cm3;
  mNumComponents = 2;
  
  G4Material* mat = new G4Material(name, mDensity, mNumComponents);
  mat-> AddElement(GetElement("N"), 70*perCent);
  mat-> AddElement(GetElement("O"), 30*perCent);
  
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
  
  mat->SetMaterialPropertiesTable(mpt);
  
	return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* MaterialFactory::CreateGdDopedWater(const G4String& name)
{
	// --- Air  N=70% O=30% ---------
  mDensity       = 1.0*g/cm3;
  mNumComponents = 2;
  
  G4Material* mat = new G4Material(name, mDensity, mNumComponents);
  mat->AddMaterial(GetMaterial("G4_WATER"), 0.999);
  mat->AddMaterial(GetMaterial("G4_Gd"), 0.001);
  
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
  
  mat->SetMaterialPropertiesTable(mpt);
  
	return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element* MaterialFactory::GetElement(const G4String &symb, G4bool isotopes)
{
  return G4NistManager::Instance()->FindOrBuildElement(symb,isotopes); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* MaterialFactory::GetMaterial
(
  const G4String &name, 
  G4bool isotopes, 
  G4bool warning
)
{
  return G4NistManager::Instance()->FindOrBuildMaterial(name,isotopes,warning); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialPropertiesTable* MaterialFactory::GetMPT
(
  const G4String& name)
{
  G4String macroFilePath = mMacrosDirectoryPath + name + "_mpt.mac";
  
  MaterialPropertiesTable* mpt = new MaterialPropertiesTable(name);
  G4UImanager::GetUIpointer()->ExecuteMacroFile(macroFilePath);
  return mpt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialFactory* MaterialFactory::GetInstance()
{
  static MaterialFactory instance;
  
  if(fInstance == nullptr)
  {
    fInstance = &instance;
  }
    
  return fInstance;
   
}
