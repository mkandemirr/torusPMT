#ifndef VComponentBuilder_h
#define VComponentBuilder_h 1

#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
template <typename T>
class VComponentBuilder{
  public:
    VComponentBuilder();
    virtual ~VComponentBuilder(){};

    T* GetProduct();
  
  protected:
    virtual T* Build() = 0;   
    T* product;      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template <typename T>
VComponentBuilder<T>::VComponentBuilder():product{nullptr}
{
}

template <typename T>
T* VComponentBuilder<T>::GetProduct()
{
  if(product)
    return product;
  else
    return Build();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

