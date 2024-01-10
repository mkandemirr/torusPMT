#ifndef ToroidalPMTLVBuilder__
#define ToroidalPMTLVBuilder__

#include "VComponentBuilder.hh"
#include "G4NistManager.hh"

class G4PVPlacement;
class ToroidalPMTLVBuilderMsgr;
class G4LogicalBorderSurface;
class G4LogicalVolume;
class G4OpticalSurface;
class G4Material;
class GLG4TorusStack;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ToroidalPMTLVBuilder : public VComponentBuilder<G4LogicalVolume> {
  public:
    ToroidalPMTLVBuilder(const G4String& pModelName, 
                 const G4String& pMacroFilePath = ""
                 );
                 
  ~ToroidalPMTLVBuilder();
  
  G4LogicalVolume* GetProductInAnEnvelope(G4Material* pEnvMat = 
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"));
    
  public:
    //UI commands, input parameters
    void SetZEdge(const std::vector<G4double>& pZEdge);
    void SetRhoEdge(const std::vector<G4double>& pRhoEdge);
    void SetZOrigin(const std::vector<G4double>& pZOrigin);
    void SetFaceGap(G4double pFaceGap);
    void SetDynodeRadius(G4double pDynodeRadius);
    void SetDynodeTop(G4double pDynodeTop);
    void SetWallThickness(G4double pWallThickness);
    
    //void SetPhotocathodeMinRho(G4double pPhotocathodeMinRho); 
    //void SetPhotocathodeMaxRho(G4double pPhotocathodeMaxRho); 
    
    void SetPrepulseProb(G4double pPrepulseProb);
    void SetEfficiencyCorrection(G4double pEfficiencyCorrection);
    
    void SetBodyMaterialName(const G4String& name); 
    void SetPmtVaccumMaterialName(const G4String& name); 
    void SetDynodeMaterialName(const G4String& name);   
    
    void SetPhotocathodeOSName(const G4String& name); 
    void SetMirrorOSName(const G4String& name);
    void SetDynodeOSName(const G4String& name);
  
  protected:
    virtual G4LogicalVolume* Build() override;
          
  private:
    void SetInputParametersWithAMacroFile();
    void ValidateInputParameters();
    
    G4LogicalVolume* BuildEnvelope();
    G4LogicalVolume* BuildBody();
    
    void CalculateBodyDaughterVolumesParameters(
      std::vector<G4double> &innerZEdge, 
      std::vector<G4double> &innerRhoEdge,
      int &equatorIndex, 
      G4double &zLowestDynode
      );
    
    G4LogicalVolume* BuildInner1(
      const std::vector<double> &innerZEdge,
      const std::vector<double> &innerRhoEdge,
      int equatorIndex
      );
    
    G4LogicalVolume* BuildInner2(
      const std::vector<double> &innerZEdge,
      const std::vector<double> &innerRhoEdge,
      int equatorIndex
      );
    
    /*
    void EvaluatePhotocathodeMinAndMaxRho(
           const std::vector<G4double>& innerRhoEdge,
           int equatorIndex  
           );                 
    */
    
    G4LogicalVolume* BuildDynode(G4double zLowestDynode);
    void BuildSkinSurfaceForDynode();
    /*
    G4LogicalVolume* BuildCentralGap( 
      const std::vector<double> &innerRhoEdge,
      int equatorIndex
      );
   */
    void SetVisAttributes();
    
    G4PVPlacement* PlaceBodyInEnvelope();
    G4PVPlacement* PlaceInner1InBody();
    G4PVPlacement* PlaceInner2InBody();
    //G4PVPlacement* PlaceCentralGapInBody();
    void PlaceDynodeInInner2();
    
    void BuildPmtMirrorSurfaces(
      G4PVPlacement *bodyPV,
      G4PVPlacement *inner2PV
      );
    
    void BuildPhotoCathodeSurface(
      G4PVPlacement* inner1Pv,
      G4PVPlacement* bodyPv
      );
    /*        
    void CreatePmtOpticalPhotonSimulationModel(
      G4LogicalBorderSurface* photocathodeSurface
      );
    */
    G4Material* GetMaterial(const G4String& name);
    G4OpticalSurface* GetOS(const G4String& name);
    
    private: 
      ToroidalPMTLVBuilderMsgr *mMessenger;
      
      //G4VSensitiveDetector *mSensitiveDet;
      
      G4String mModelName;
      G4String mMacroFilePath;
      G4String mPrefix;
      bool mSimpleVis;
            
      //Envelope control
      bool mUseEnvelope;
      G4double mFaceGap;
      G4double mMinEnvelopeRadius;
      G4Material *mEnvelopeMat;

      G4double mDynodeRadius; // mm
      G4double mDynodeTop; // mm
      
      //G4double mPhotocathodeMinRho;//mm
      //G4double mPhotocathodeMaxRho;//mm

      //G4double mEfficiencyCorrection; // default to 1.0 for no correction
      //G4double mPrepulseProb;
      
      G4String mBodyMatName;
      G4String mPmtVacuumMatName;
      G4String mDynodeMatName;
       
      G4String mPhotocathodeOSName;
      G4String mMirrorOSName;
      G4String mDynodeOSName;
      
      // Body control
      std::vector<G4double> mZEdge; // n+1
      std::vector<G4double> mRhoEdge; // n+1
      std::vector<G4double> mZOrigin; // n
      G4double mWallThickness; // mm
      
      G4LogicalVolume* mEnvelopeLv;
      G4LogicalVolume* mBodyLv;
      G4LogicalVolume* mInner1Lv;
      G4LogicalVolume* mInner2Lv;
      G4LogicalVolume* mDynodeLv;
      //G4LogicalVolume* mCentralGapLv;
       
};

#endif
