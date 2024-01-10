#ifndef MaterialFactory_h
#define MaterialFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "VComponentFactory.hh"

class MaterialPropertiesTable;

//singleton class
class MaterialFactory: public VComponentFactory<G4Material>
{
  public:
    static MaterialFactory* GetInstance();
    
    //Singletons should not be cloneable.
    MaterialFactory(const MaterialFactory&) = delete;
    
    //Singletons should not be assignable.
    MaterialFactory& operator=(const MaterialFactory&) = delete;
     
  protected:  
    MaterialFactory();
    virtual ~MaterialFactory();
      
    virtual G4Material* Create(const G4String& name) override;
      
  private:
    G4Material* CreateAir(const G4String& name); 
    G4Material* CreateGdDopedWater(const G4String& name);
    
    MaterialPropertiesTable* GetMPT(const G4String& name);

    G4Element* GetElement(const G4String &symb, G4bool isotopes=true);
    G4Material* GetMaterial(
      const G4String &name, 
      G4bool isotopes = true, 
      G4bool warning = false
      );
      
  private: //members
    static MaterialFactory* fInstance;  //singleton class
    
    G4String mMacrosDirectoryPath;

    G4int mNumComponents;
    G4double mDensity;
                   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
