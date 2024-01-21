#include "PhotonSDHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PhotonSDHit>* PhotonSDHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSDHit::PhotonSDHit()
 : G4VHit(),
   fCopyNo(-1),
   fArrivalTime(0.)
   
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSDHit::~PhotonSDHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSDHit::PhotonSDHit(const PhotonSDHit& right)
  : G4VHit()
{
  fCopyNo               = right.fCopyNo;
  fArrivalTime          = right.fArrivalTime;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PhotonSDHit& PhotonSDHit::operator=(const PhotonSDHit& right)
{
  fCopyNo             = right.fCopyNo;
  fArrivalTime        = right.fArrivalTime;
  
  
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonSDHit::operator==(const PhotonSDHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
