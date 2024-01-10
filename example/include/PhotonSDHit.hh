#ifndef PhotonSDHit_h
#define PhotonSDHit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class PhotonSDHit : public G4VHit
{
  public:
    PhotonSDHit();
    PhotonSDHit(const PhotonSDHit&);
    virtual ~PhotonSDHit();

    // operators
    const PhotonSDHit& operator=(const PhotonSDHit&);
    G4bool operator==(const PhotonSDHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print() {};
 
    G4int GetCopyNo() const { return fCopyNo; }
    G4double GetArrivalTime() const { return fArrivalTime; }

    void SetArrivalTime(G4double time) { fArrivalTime = time; }
    void SetCopyNo(G4int cpyNo) { fCopyNo = cpyNo; }
  
  private:
    G4int fCopyNo;
    G4double  fArrivalTime; 
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using PhotonSDHitsCollection = G4THitsCollection<PhotonSDHit>;
//typedef G4THitsCollection<PhotonSDHit> PhotonSDHitsCollection;

extern G4ThreadLocal G4Allocator<PhotonSDHit>* PhotonSDHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PhotonSDHit::operator new(size_t)
{
  if (!PhotonSDHitAllocator) 
  {
    PhotonSDHitAllocator = new G4Allocator<PhotonSDHit>;
  }
  void *hit;
  hit = (void *) PhotonSDHitAllocator->MallocSingle();
  return hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PhotonSDHit::operator delete(void *hit)
{
  if (!PhotonSDHitAllocator) 
  {
    PhotonSDHitAllocator = new G4Allocator<PhotonSDHit>;
  }
  PhotonSDHitAllocator->FreeSingle((PhotonSDHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
