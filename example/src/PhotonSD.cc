#include "PhotonSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh" 
#include "G4ParticleTypes.hh" 
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSD::PhotonSD( const G4String& name)
 : G4VSensitiveDetector(name),
   mHitsCollection{nullptr},
   mHCID{-1},
   mOpBoundaryProcess{nullptr}
{
  collectionName.insert("PhotonSDHitCol"); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSD::~PhotonSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  mHitsCollection = new PhotonSDHitsCollection(SensitiveDetectorName, 
                                               collectionName[0]
                                               ); 
  if (mHCID < 0) 
  { 
    mHCID = G4SDManager::GetSDMpointer()->GetCollectionID(mHitsCollection);
  }

  hce->AddHitsCollection(mHCID, mHitsCollection);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  if (step == nullptr) 
    return false;

  G4ParticleDefinition* particleDef = step->GetTrack()->GetDefinition();

  if (particleDef != G4OpticalPhoton::OpticalPhotonDefinition())
    return false;
     
  // find the boundary process only once
  if (mOpBoundaryProcess == nullptr)
  { 
    G4ProcessManager* pm = particleDef->GetProcessManager();
    G4ProcessVector* pv  = pm->GetProcessList();
    
    for(G4int i = 0; i < pv->size(); ++i)
    {
      //G4cout<<"processName: "<<(*pv)[i]->GetProcessName()<<G4endl;
      if ( (*pv)[i] != nullptr && (*pv)[i]->GetProcessName() == "OpBoundary")
      {
        mOpBoundaryProcess = dynamic_cast<G4OpBoundaryProcess*>((*pv)[i]);
        break;
      }
    }
  }

  G4OpBoundaryProcessStatus boundaryStatus = Undefined;

  if (mOpBoundaryProcess != nullptr)
    boundaryStatus = mOpBoundaryProcess->GetStatus();
    
  //G4cout<<"boundaryStatus: "<<boundaryStatus<<G4endl;

  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
  { 
    switch (boundaryStatus)
    {
      case Absorption:
      case Detection: //Trigger hits         
        GenerateHits(step);
        step->GetTrack()->SetTrackStatus(fStopAndKill);
      case StepTooSmall:
      default:
      break;
    }

  }  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::GenerateHits(const G4Step* step)
{   
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  
  //inner1Pv(0)--pmtbody(1)--pmtEnvelope(2)
  G4int copyNo  = postStepPoint->GetTouchableHandle()->GetCopyNumber(2);  
  
  PhotonSDHit *hit  = new PhotonSDHit();
  hit->SetCopyNo(copyNo);
  hit->SetArrivalTime(postStepPoint->GetGlobalTime());
  
  mHitsCollection->insert(hit);
                                  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::EndOfEvent(G4HCofThisEvent*)
{
  PrintAll(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::PrintAll()
{
  G4cout<<"-------------------PhotonSD Output----------------------"<<G4endl;
  G4int characterSpacecNum = 4;
  G4cout.width(6 + characterSpacecNum);
  G4cout<<std::left<<"CopyNo";
  
  characterSpacecNum = 4;
  G4cout.width(24 + characterSpacecNum);
  G4cout<<std::left<<"Photon arrival time (ns)"<<G4endl;
 
  for (std::size_t i = 0; i < mHitsCollection->entries(); ++i )
  {
    auto hit = (*mHitsCollection)[i]; 
    
    G4int characterSpacecNum = 4;
    G4cout.width(6 + characterSpacecNum);
    G4cout<<std::left<<hit->GetCopyNo();
    
    characterSpacecNum = 4;
    G4cout.width(24 + characterSpacecNum);
    G4cout<<std::left<<hit->GetArrivalTime()<<G4endl; 
  }
  
  PrintDetectedPhotonMap();
  
  G4cout<<"-------------------PhotonSD End-------------------------"<<G4endl;    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::PrintDetectedPhotonMap() const
{
   // Before printing, calculate total number of detected photons in 
  // each photosensor.  
  std::map<int,int> map;
  
  for (std::size_t i = 0; i < mHitsCollection->entries(); ++i)
  {
    auto hit   = (*mHitsCollection)[i];             
    int copyNo = hit->GetCopyNo();
        
    auto it    = map.find(copyNo);
    if( it == map.end() ) //not found
      map[copyNo] = 1;
    else
      map.at(it->first) += 1;                  
  }
  
  G4cout<<"--------Detected photons map------------"<<G4endl;  
  
  G4int characterSpacecNum = 4;
  G4cout.width(6 + characterSpacecNum);
  G4cout<<std::left<<"CopyNo";
  
  characterSpacecNum = 4;
  G4cout.width(22 + characterSpacecNum);
  G4cout<<std::left<<"Detected photon number"<<G4endl;
  
  for (auto el: map)
  {
    G4int characterSpacecNum = 4;
    G4cout.width(6 + characterSpacecNum);
    G4cout<<std::left<<el.first;

    characterSpacecNum = 0;
    G4cout.width(22 + characterSpacecNum);
    G4cout<<std::left<<el.second<<G4endl;  
  }
}

