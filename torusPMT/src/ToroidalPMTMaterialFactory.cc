#include "ToroidalPMTMaterialFactory.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTMaterialFactory* ToroidalPMTMaterialFactory::mInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTMaterialFactory::ToroidalPMTMaterialFactory():
VComponentFactory(),
mNumComponents{0},
mDensity{0.}
{
  mMacrosDirectoryPath = "torusPMT/macros/mpt/"; //relative to CMAKE_BINARY_DIR
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTMaterialFactory::~ToroidalPMTMaterialFactory()
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ToroidalPMTMaterialFactory::Create(const G4String& name)
{
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(name);
  if(mat) return mat;
 
  if (name == "pmtVacuum")
    return CreatePmtVacuum(name);  
  else if (name == "glass")
    return CreateGlass(name);
  else if (name == "quartz")
    return CreateQuartz(name);
  else if (name == "stainlessSteel")
    return CreateStainlessSteel(name);                    
  else
    return nullptr;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ToroidalPMTMaterialFactory::CreatePmtVacuum(const G4String& name)
{
  // --- PMT vacuum is very dilute air -------
  mDensity     = 1e-3 * CLHEP::kGasThreshold; //from PhysicalConstants.h
  mNumComponents = 1;
  G4double temperature = CLHEP::STP_Temperature;  //from PhysicalConstants.h
  G4double pressure    = CLHEP::STP_Pressure * mDensity / (1.29e-3*g/cm3);
   
  G4Material* mat = new G4Material(name, 
                                   mDensity, 
                                   mNumComponents, 
                                   kStateGas,
                                   temperature,
                                   pressure
                                   );
  
  mat->AddMaterial(G4NistManager::Instance()->
    FindOrBuildMaterial("G4_AIR"), 1.);                                 
                                   
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
                           
  mat->SetMaterialPropertiesTable(mpt);
  
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ToroidalPMTMaterialFactory::CreateGlass(const G4String& name)
{
  mDensity = 2.2*g/cm3;  // changed 1999/12/03 (was 2.7*g/cm3) -- GAS
  mNumComponents = 2;
  
  G4Material* mat = new G4Material(name, mDensity, mNumComponents);
  mat-> AddElement(GetElement("Si"), 0.4675);
  mat-> AddElement(GetElement("O"),  0.5325);
  
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
  
  mat->SetMaterialPropertiesTable(mpt);
  
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ToroidalPMTMaterialFactory::CreateQuartz(const G4String& name)
{
  mDensity = 2.65*g/cm3; 
  mNumComponents = 2;
  
  G4Material* mat = new G4Material(name, mDensity, mNumComponents);
  mat-> AddElement(GetElement("Si"), 0.4675);
  mat-> AddElement(GetElement("O"),  0.5325);
  
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
  
  mat->SetMaterialPropertiesTable(mpt);
  
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ToroidalPMTMaterialFactory::CreateStainlessSteel
(
  const G4String& name
)
{
  // --- Stainless Steel  71% Fe, 19% Cr, 10% Ni ------
  mDensity       = 7.87*g/cm3;
  mNumComponents = 3;
  
  G4Material* mat = new G4Material(name, mDensity, mNumComponents);
  mat-> AddElement(GetElement("Fe"), 0.71);
  mat-> AddElement(GetElement("Cr"), 0.19);
  mat-> AddElement(GetElement("Ni"), 0.10);
  
  MaterialPropertiesTable* mpt = GetMPT(name);
  mpt->DumpTable();
  
  mat->SetMaterialPropertiesTable(mpt);
  
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element* ToroidalPMTMaterialFactory::GetElement
(
  const G4String &symb, 
  G4bool isotopes
)
{
  return G4NistManager::Instance()->FindOrBuildElement(symb,isotopes); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MaterialPropertiesTable* ToroidalPMTMaterialFactory::GetMPT
(
  const G4String& name)
{
  MaterialPropertiesTable* mpt = new MaterialPropertiesTable(name);
  
  G4String macroFilePath = mMacrosDirectoryPath + name + "_mpt.mac";
  G4UImanager::GetUIpointer()->ExecuteMacroFile(macroFilePath);
  return mpt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTMaterialFactory* ToroidalPMTMaterialFactory::GetInstance()
{
  static ToroidalPMTMaterialFactory instance;
  
  if (mInstance == nullptr)
  {
    mInstance = &instance;
  }
    
  return mInstance; 
}
