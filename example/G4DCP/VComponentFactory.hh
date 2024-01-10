#ifndef VComponentFactory_h
#define VComponentFactory_h 1

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template <typename T>
class VComponentFactory
{
  public:
    VComponentFactory(){};
    virtual ~VComponentFactory(){};
     
  public:     
    //This method internally invokes the factory method.                          
    T* Get(const G4String& name);                          
  protected: 
    virtual T* Create(const G4String& name) = 0;               
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template <typename T>
T* VComponentFactory<T>::Get(const G4String& name)
{
  T* product = Create(name);
  if (!product) 
  {
    std::ostringstream o;
    o << "Component " << name << " not found!";
    G4Exception("VComponentFactory::Get()","",FatalException,o.str().c_str());
  }
  return product;
}

#endif 
