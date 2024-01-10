#ifndef ToroidalPMTMaterialFactory_h
#define ToroidalPMTMaterialFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "VComponentFactory.hh"

class MaterialPropertiesTable;
class G4Material;
class G4Element;

//singleton class
class ToroidalPMTMaterialFactory: public VComponentFactory<G4Material>
{
  public:
    static ToroidalPMTMaterialFactory* GetInstance();
    
    //Singletons should not be cloneable.
    ToroidalPMTMaterialFactory(const ToroidalPMTMaterialFactory&) = delete;
    
    //Singletons should not be assignable.
    ToroidalPMTMaterialFactory& operator=(const ToroidalPMTMaterialFactory&) = delete;
     
  protected:  
    ToroidalPMTMaterialFactory();
    virtual ~ToroidalPMTMaterialFactory();
      
    virtual G4Material* Create(const G4String& name) override;
      
  private: 
    G4Material* CreatePmtVacuum(const G4String& name);
    G4Material* CreateGlass(const G4String& name);
    G4Material* CreateQuartz(const G4String& name);
    G4Material* CreateStainlessSteel(const G4String& name);
    
    
    MaterialPropertiesTable* GetMPT(const G4String& name);

    G4Element* GetElement(const G4String &symb, G4bool isotopes=true);
  
  private: 
    static ToroidalPMTMaterialFactory* mInstance;  //singleton class
    
    G4String mMacrosDirectoryPath;

    G4int mNumComponents;
    G4double mDensity;             
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
