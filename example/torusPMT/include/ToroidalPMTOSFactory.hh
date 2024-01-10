#ifndef ToroidalPMTOSFactory_h
#define ToroidalPMTOSFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "VComponentFactory.hh"

class G4OpticalSurface;
class OpticalSurfaceBuilder;

//singleton class
class ToroidalPMTOSFactory: public VComponentFactory<G4OpticalSurface>
{
  public:
    static ToroidalPMTOSFactory* GetInstance();
    
    //Singletons should not be cloneable.
    ToroidalPMTOSFactory(const ToroidalPMTOSFactory&) = delete;
    
    //Singletons should not be assignable.
    ToroidalPMTOSFactory& operator=(const ToroidalPMTOSFactory&) = delete;
     
  protected:  
    ToroidalPMTOSFactory();
    virtual ~ToroidalPMTOSFactory();
      
    virtual G4OpticalSurface* Create(const G4String& name) override;
      
  private: //functions
    G4OpticalSurface* CreateSurface(const G4String& name, G4bool enableMatProp);
  
  private: //data members
    static ToroidalPMTOSFactory* mInstance; //singleton class
    
    std::map<G4String, G4OpticalSurface*> mProductsMap;
    G4String mMacrosDirectoryPath;
                   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
