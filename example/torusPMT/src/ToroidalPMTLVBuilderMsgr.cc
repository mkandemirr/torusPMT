#include "ToroidalPMTLVBuilderMsgr.hh"
#include "ToroidalPMTLVBuilder.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVBuilderMsgr::ToroidalPMTLVBuilderMsgr(ToroidalPMTLVBuilder *builder)
:G4UImessenger(),
mBuilder(builder)
{  
  G4String commandDirectory = "/pmt/";
 
  //-------Command 1----------
  G4String commandName  = "zEdge";
  G4String commandPath  = commandDirectory+commandName;
  G4String usage        = "[usage] "+ commandPath + " zEdge";
  
 
 	mZEdgeCmd = new G4UIcmdWithAString(commandPath, this);
  mZEdgeCmd->SetGuidance(usage);
  mZEdgeCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mZEdgeCmd->SetToBeBroadcasted(false);
  
  //-------Command 2----------
  commandName ="rhoEdge";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " rhoEdge";
  
  mRhoEdgeCmd = new G4UIcmdWithAString(commandPath, this);
  mRhoEdgeCmd->SetGuidance(usage);
  mRhoEdgeCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mRhoEdgeCmd->SetToBeBroadcasted(false);
  
  //-------Command 3----------
  commandName ="zOrigin";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " zOrigin";
  
  mZOriginCmd = new G4UIcmdWithAString(commandPath, this);
  mZOriginCmd->SetGuidance(usage);
  mZOriginCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mZOriginCmd->SetToBeBroadcasted(false);
  
  //-------Command 4----------
  commandName ="faceGap";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " faceGap";
  
  mFaceGapCmd = new G4UIcmdWithADoubleAndUnit(commandPath,this);
  mFaceGapCmd->SetGuidance(usage);
  mFaceGapCmd->SetParameterName("faceGap",false);
  mFaceGapCmd->SetUnitCategory("Length");
  mFaceGapCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
  //-------Command 5----------
  commandName ="dynodeRadius";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " dynodeRadius";
  
  mDynodeRadiusCmd = new G4UIcmdWithADoubleAndUnit(commandPath,this);
  mDynodeRadiusCmd->SetGuidance(usage);
  mDynodeRadiusCmd->SetParameterName("dynodeRadius",false);
  mDynodeRadiusCmd->SetUnitCategory("Length");
  mDynodeRadiusCmd->SetDefaultUnit("mm");
  mDynodeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
  //-------Command 6----------
  commandName ="dynodeTop";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " dynodeTop";
  
  mDynodeTopCmd = new G4UIcmdWithADoubleAndUnit(commandPath,this);
  mDynodeTopCmd->SetGuidance(usage);
  mDynodeTopCmd->SetParameterName("dynodeTop",false);
  mDynodeTopCmd->SetUnitCategory("Length");
  mDynodeTopCmd->SetDefaultUnit("mm");
  mDynodeTopCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
  //-------Command 7----------
  commandName ="wallThickness";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " wallThickness";
  
  mWallThicknessCmd = new G4UIcmdWithADoubleAndUnit(commandPath,this);
  mWallThicknessCmd->SetGuidance(usage);
  mWallThicknessCmd->SetParameterName("wallThickness",false);
  mWallThicknessCmd->SetUnitCategory("Length");
  mWallThicknessCmd->SetDefaultUnit("mm");
  mWallThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  /*
  //-------Command 8----------
  commandName ="photocathodeMinRho";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " photocathodeMinRho";
  
  mPhotocathodeMinRhoCmd = new G4UIcmdWithADouble(commandPath,this);
  mPhotocathodeMinRhoCmd->SetGuidance(usage);
  mPhotocathodeMinRhoCmd->SetDefaultValue(0.);
  mPhotocathodeMinRhoCmd->SetParameterName("photocathodeMinRho",false);
  mPhotocathodeMinRhoCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
   //-------Command 9----------
  commandName ="photocathodeMaxRho";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " photocathodeMaxRho";
  
  mPhotocathodeMaxRhoCmd = new G4UIcmdWithADouble(commandPath,this);
  mPhotocathodeMaxRhoCmd->SetGuidance(usage);
  mPhotocathodeMaxRhoCmd->SetDefaultValue(0.);
  mPhotocathodeMaxRhoCmd->SetParameterName("photocathodeMaxRho",false);
  mPhotocathodeMaxRhoCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
  //-------Command 10----------
  commandName ="prepulseProb";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " prepulseProb";
  
  mPrepulseProbCmd = new G4UIcmdWithADouble(commandPath,this);
  mPrepulseProbCmd->SetGuidance(usage);
  mPrepulseProbCmd->SetDefaultValue(0.);
  mPrepulseProbCmd->SetParameterName("prepulseProb",false);
  mPrepulseProbCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  
  //-------Command 11----------
  commandName ="efficiencyCorrection";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " efficiencyCorrection";
  
  mPrepulseProbCmd = new G4UIcmdWithADouble(commandPath,this);
  mPrepulseProbCmd->SetGuidance(usage);
  mPrepulseProbCmd->SetDefaultValue(1.0);
  mPrepulseProbCmd->SetParameterName("efficiencyCorrection",false);
  mPrepulseProbCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  */
  //-------Command 12----------
  commandName ="bodyMaterial"; 
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " bodyMaterial";
  
  mBodyMatNameCmd = new G4UIcmdWithAString(commandPath, this);
  mBodyMatNameCmd->SetGuidance(usage);
  mBodyMatNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mBodyMatNameCmd->SetToBeBroadcasted(false);
  
   //-------Command 13----------
  commandName ="vacuumMaterial";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " vacuumMaterial";
  
  mPmtVacuumMatNameCmd = new G4UIcmdWithAString(commandPath, this);
  mPmtVacuumMatNameCmd->SetGuidance(usage);
  mPmtVacuumMatNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mPmtVacuumMatNameCmd->SetToBeBroadcasted(false);
  
   //-------Command 14----------
  commandName ="dynodeMaterial";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " dynodeMaterial";
  
  mDynodeMatNameCmd = new G4UIcmdWithAString(commandPath, this);
  mDynodeMatNameCmd->SetGuidance(usage);
  mDynodeMatNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mDynodeMatNameCmd->SetToBeBroadcasted(false);
  
  //-------Command 15----------
  commandName ="photocathodeSurface";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " photocathodeSurface";
  
  mPhotocathodeOSNameCmd = new G4UIcmdWithAString(commandPath, this);
  mPhotocathodeOSNameCmd->SetGuidance(usage);
  mPhotocathodeOSNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mPhotocathodeOSNameCmd->SetToBeBroadcasted(false);
  
  //-------Command 16----------
  commandName ="mirrorSurface";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " mirrorSurface";
  
  mMirrorOSNameCmd = new G4UIcmdWithAString(commandPath, this);
  mMirrorOSNameCmd->SetGuidance(usage);
  mMirrorOSNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mMirrorOSNameCmd->SetToBeBroadcasted(false);
  
  //-------Command 17----------
  commandName ="dynodeSurface";
  commandPath = commandDirectory+commandName;
  usage       = "[usage] "+ commandPath + " dynodeSurface";
  
  mDynodeOSNameCmd = new G4UIcmdWithAString(commandPath, this);
  mDynodeOSNameCmd->SetGuidance(usage);
  mDynodeOSNameCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  mDynodeOSNameCmd->SetToBeBroadcasted(false);
 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVBuilderMsgr::~ToroidalPMTLVBuilderMsgr()
{
  //delete mDir;  
 	delete mZEdgeCmd; 
 	delete mRhoEdgeCmd;
 	delete mZOriginCmd;
 	delete mFaceGapCmd;
 	delete mDynodeRadiusCmd;
 	delete mDynodeTopCmd;
 	delete mWallThicknessCmd;
 	//delete mPhotocathodeMinRhoCmd;
 	//delete mPhotocathodeMaxRhoCmd;
 	//delete mPrepulseProbCmd;
 	//delete mEfficiencyCorrectionCmd;
 	delete mBodyMatNameCmd;
 	delete mPmtVacuumMatNameCmd;
 	delete mDynodeMatNameCmd;
 	delete mPhotocathodeOSNameCmd;
 	delete mMirrorOSNameCmd;
 	delete mDynodeOSNameCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilderMsgr::SetNewValue(G4UIcommand* command,G4String newValue)
{
	if( command == mZEdgeCmd )
	{
		std::vector<double> zEdge;
		GetAsAVector(newValue,zEdge); 
		
		mBuilder->SetZEdge(zEdge);
		
		zEdge.clear();

	}else if ( command == mRhoEdgeCmd)
	{
		std::vector<double> rhoEdge;
		GetAsAVector(newValue,rhoEdge); 
		
		mBuilder->SetRhoEdge(rhoEdge);
		
		rhoEdge.clear();
		
	}else if ( command == mZOriginCmd)
	{
		std::vector<double> zOrigin;
		GetAsAVector(newValue, zOrigin); 
		
		mBuilder->SetZOrigin(zOrigin);
		
		zOrigin.clear();
		
	}else if (command == mFaceGapCmd)
	{
		mBuilder->SetFaceGap(mFaceGapCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mDynodeRadiusCmd)
	{
		mBuilder->SetDynodeRadius(mDynodeRadiusCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mDynodeTopCmd)
	{
		mBuilder->SetDynodeTop(mDynodeTopCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mWallThicknessCmd)
	{
		mBuilder->SetWallThickness(mWallThicknessCmd->GetNewDoubleValue(newValue));
	
	}/*else if (command == mPhotocathodeMinRhoCmd)
	{
		mBuilder->SetPhotocathodeMinRho(
		mPhotocathodeMinRhoCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mPhotocathodeMaxRhoCmd)
	{
		mBuilder->SetPhotocathodeMaxRho(
		mPhotocathodeMaxRhoCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mPrepulseProbCmd)
	{
		mBuilder->SetPrepulseProb(mPrepulseProbCmd->GetNewDoubleValue(newValue));
	
	}else if (command == mEfficiencyCorrectionCmd)
	{
		mBuilder->SetEfficiencyCorrection(
		mEfficiencyCorrectionCmd->GetNewDoubleValue(newValue));
	
	}*/else if (command == mBodyMatNameCmd)
	{
		mBuilder->SetBodyMaterialName(newValue);
	}else if (command == mPmtVacuumMatNameCmd)
	{
		mBuilder->SetPmtVaccumMaterialName(newValue);
	}else if (command == mDynodeMatNameCmd)
	{
		mBuilder->SetDynodeMaterialName(newValue);
	}else if (command == mPhotocathodeOSNameCmd)
	{
		mBuilder->SetPhotocathodeOSName(newValue);
	}else if (command == mMirrorOSNameCmd)
	{
		mBuilder->SetMirrorOSName(newValue);
	}else if (command == mDynodeOSNameCmd)
	{
		mBuilder->SetDynodeOSName(newValue);
	}
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilderMsgr::GetAsAVector(const G4String& inputString,
																		std::vector<G4double>& vec
																		)
{
  std::istringstream instring(inputString);

  while (instring)
  {
    G4String tmp;
    instring >> tmp;
    if (tmp == "") //read last empty 
    {
      break;
    }
    G4double value = G4UIcommand::ConvertToDouble(tmp);
    vec.push_back(value);
  }

  /*
  G4cout<<"PrintInputVector: "<<G4endl;
  for (double element: vec)
	  G4cout<<element<<" ";

  G4cout<<"\n";
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
