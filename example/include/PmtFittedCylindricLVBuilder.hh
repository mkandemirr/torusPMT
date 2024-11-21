#ifndef PmtFittedCylindricLVBuilder__
#define PmtFittedCylindricLVBuilder__

#include "VComponentBuilder.hh"
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;

class PmtFittedCylindricLVBuilder : public VComponentBuilder<G4LogicalVolume>{
      
  public:
    PmtFittedCylindricLVBuilder(const G4String& pmtModel); 
    ~PmtFittedCylindricLVBuilder();
  
  protected:
    virtual G4LogicalVolume* Build() override;
           
  private:
    //for a single Pmt
    using PmtRotationAngles = std::vector<std::pair<char, G4double>>;
    
    // for all Pmts in a ring
    using PmtRotationAnglesList = std::vector<PmtRotationAngles>; 
    using PmtThetas             = std::vector<G4double>; 
    
    void Initialize();
    void InitializeSidePmtSettings();
    void InitializeTopPmtSettings();
    void InitializeBottomPmtSettings();
    void ValidateInputParameters();
    
    G4LogicalVolume* BuildPmt(const G4String& modelName);
    G4LogicalVolume* BuildInnerCylinder();
    G4LogicalVolume* BuildOuterCylinder();
    
    void PlaceInnerCylinder();
    void PlacePmtsOnInnerCylinderSide();                                    
    void PlacePmtsOnInnerCylinderTop();                           
    void PlacePmtsOnInnerCylinderBottom();                                    
         
    //No restriction         
    void PlaceRing(
           G4double r,
           PmtThetas pmtThetas, 
           G4double z,   
           PmtRotationAnglesList pmtRotAnglesList 
           );                
    
    //No restriction                           
    void PlacePmt(
           G4double r, 
           G4double theta, 
           G4double z,
           PmtRotationAngles rotAngles
           );  
            
    G4bool ArePmtsOverlappedInTheRing(
             G4double r,
             G4double numberOfPmts
             );         
   
    //For a top and bottom ring. There is a restriction: 
    // it fill Pmts at equal theta intervals
    void FillPmtThetas(
           G4int numberOfPmts, 
           PmtThetas& pmtThetas,
           G4double firstPlacedPmtTheta
           );
   
    //For a top and bottom ring.
    void FillPmtRotationAnglesList(
           G4int numberOfPmts, 
           PmtRotationAnglesList& pmtRotAnglesList,
           G4bool isTopRing
           );
    
    //For a side ring. Since pmt rotation angles depends their theta angles,
    //pmtThetas and pmtRotAnglesList are filled in a single function.         
    void FillPmtThetasAndRotationAnglesList(
           G4int numberOfPmts,
           PmtThetas& pmtThetas,
           PmtRotationAnglesList& pmtRotAnglesList,
           G4double firstPlacedPmtTheta
           );            
  private:
    //User interface parameters, independent parameters
    G4String mPmtModel;
    //Side 
    std::vector<G4int> mSideRingsPmtCounts;
    std::vector<G4double> mSideRingsFirstPlacedPmtThetas; 
    G4double mDistanceBetweenSideRings; //z direction
    G4double mSideTopRingOffset; //z direction
    G4double mSideBottomRingOffset; //z direction
    G4double mSidePmtsFrontEndDistance;
    //Top 
    std::vector<G4int> mTopRingsPmtCounts; 
    std::vector<G4double> mTopRingsFirstPlacedPmtThetas;
    G4double mDistanceBetweenTopRings; //in r direction
    G4double mTopLastRingOffset; //in r direction
    G4double mTopFirstRingRadius;  
    G4double mTopPmtsFrontEndDistance;
    //Bottom 
    std::vector<G4int> mBottomRingsPmtCounts;
    std::vector<G4double> mBottomRingsFirstPlacedPmtThetas;
    G4double mDistanceBetweenBottomRings;
    G4double mBottomLastRingOffset; //along r direction
    G4double mBottomFirstRingRadius;
    G4double mBottomPmtsFrontEndDistance;
    
    //The parameters below are not obtained from users
    G4double mPmtEnvelopeRadius;
    G4double mPmtEnvelopeZHalfLength;
    
    G4double mInnerCylHight;
    G4double mInnerCylRadius;
    
    G4LogicalVolume* mPmtEnvelopeLv;
    G4LogicalVolume* mInnerCylLv;
    G4LogicalVolume* mOuterCylLv;
    
    std::vector<G4VPhysicalVolume*> mPmtPvs;                                             
};

#endif
