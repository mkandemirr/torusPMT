

#ifndef ToroidalPMTLVBuilderMsgr_h
#define ToroidalPMTLVBuilderMsgr_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ToroidalPMTLVBuilder;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ToroidalPMTLVBuilderMsgr: public G4UImessenger
{
  public:
    ToroidalPMTLVBuilderMsgr(ToroidalPMTLVBuilder* );
   ~ToroidalPMTLVBuilderMsgr();

    virtual void SetNewValue(G4UIcommand*, G4String);

	private:
		void GetAsAVector(const G4String& inputString,
											std::vector<G4double>& vec
		                 );
  
    ToroidalPMTLVBuilder *mBuilder;
   
    G4UIcmdWithAString *mZEdgeCmd;
    G4UIcmdWithAString *mRhoEdgeCmd;
    G4UIcmdWithAString *mZOriginCmd;
     
    G4UIcmdWithADoubleAndUnit *mFaceGapCmd;
    G4UIcmdWithADoubleAndUnit	*mDynodeRadiusCmd;
    G4UIcmdWithADoubleAndUnit *mDynodeTopCmd;
    G4UIcmdWithADoubleAndUnit *mWallThicknessCmd;
    /*
    G4UIcmdWithADouble *mPhotocathodeMinRhoCmd;
    G4UIcmdWithADouble *mPhotocathodeMaxRhoCmd;
    
    G4UIcmdWithADouble *mPrepulseProbCmd;
    G4UIcmdWithADouble *mEfficiencyCorrectionCmd;
    */
    G4UIcmdWithAString *mBodyMatNameCmd;
    G4UIcmdWithAString *mPmtVacuumMatNameCmd;
    G4UIcmdWithAString *mDynodeMatNameCmd;
    
    G4UIcmdWithAString *mPhotocathodeOSNameCmd;
    G4UIcmdWithAString *mMirrorOSNameCmd;
    G4UIcmdWithAString *mDynodeOSNameCmd;
   
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

