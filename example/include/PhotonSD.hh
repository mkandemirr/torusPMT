#ifndef PhotonSD_h
#define PhotonSD_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VSensitiveDetector.hh"
#include "PhotonSDHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4OpBoundaryProcess;

class PhotonSD : public G4VSensitiveDetector
{
  public:
    PhotonSD(const G4String& name);
    virtual ~PhotonSD();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);

    virtual void  EndOfEvent(G4HCofThisEvent* hitCollection);
    virtual void PrintAll() override;
  
  private:
    void PrintDetectedPhotonMap() const;
    void GenerateHits(const G4Step* step);
     
  private: 
    PhotonSDHitsCollection* mHitsCollection;
    G4int mHCID;
    
    G4OpBoundaryProcess* mOpBoundaryProcess;
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

