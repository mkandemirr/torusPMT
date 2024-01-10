#ifndef ToroidalPMTLVFactory_h
#define ToroidalPMTLVFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "VComponentFactory.hh"
#include <set>
class G4LogicalVolume;

//singleton class
class ToroidalPMTLVFactory: public VComponentFactory<G4LogicalVolume>
{
  public:
    static ToroidalPMTLVFactory* GetInstance();
    
    //Singletons should not be cloneable.
    ToroidalPMTLVFactory(const ToroidalPMTLVFactory&) = delete;
    
    //Singletons should not be assignable.
    ToroidalPMTLVFactory& operator=(const ToroidalPMTLVFactory&) = delete;
       
  protected:  
    ToroidalPMTLVFactory();
    virtual ~ToroidalPMTLVFactory();
      
    virtual G4LogicalVolume* Create(const G4String& name) override;
      
  private: 
    static ToroidalPMTLVFactory* mInstance;  
    
    std::set<G4String> mProductsSet;
    G4String mMacrosDirectoryPath;
    G4String mLVName;
               
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
