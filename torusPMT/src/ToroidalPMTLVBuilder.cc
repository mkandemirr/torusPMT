#include "ToroidalPMTLVBuilder.hh"
#include "ToroidalPMTLVBuilderMsgr.hh"
#include <algorithm>
//#include <GLG4PMTOpticalModel.hh>
#include "GLG4TorusStack.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UImanager.hh"
#include "ToroidalPMTMaterialFactory.hh"
#include "ToroidalPMTOSFactory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ToroidalPMTLVBuilder::ToroidalPMTLVBuilder(const G4String& pModelName,
                           const G4String& pMacroFilePath
                          ) :
VComponentBuilder(),
mMessenger{nullptr},
//mSensitiveDet{nullptr},
mModelName{pModelName},
mMacroFilePath{pMacroFilePath},
mPrefix{pModelName}, //model name will be used as 
                     //a prefix when creating solids, Lv and Pv
mUseEnvelope{false},
mFaceGap{0.},
mEnvelopeMat{nullptr},
mDynodeRadius{0.},
mDynodeTop{0.},
//mPhotocathodeMinRho{0.}, // Need to change to min Rho
//mPhotocathodeMaxRho{0.}, // Need to change to max Rho
//mEfficiencyCorrection{1.0},
//mPrepulseProb{0.},
mWallThickness{0.}
{  
  mMessenger = new ToroidalPMTLVBuilderMsgr(this);
  
  SetInputParametersWithAMacroFile();  
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

ToroidalPMTLVBuilder::~ToroidalPMTLVBuilder()
{
  //delete mMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

void ToroidalPMTLVBuilder::SetInputParametersWithAMacroFile()
{
  if (mMacroFilePath == "") 
  {
    G4String macrosDirectoryPath = "torusPMT/macros/pmts/";
    mMacroFilePath = macrosDirectoryPath + mModelName + ".mac";  
  }

  G4UImanager::GetUIpointer()->ExecuteMacroFile(mMacroFilePath);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* ToroidalPMTLVBuilder::GetProductInAnEnvelope(G4Material* pEnvMat)
{
  assert(pEnvMat);
  
  mEnvelopeMat = pEnvMat;
  mUseEnvelope = true;
  return GetProduct();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume *ToroidalPMTLVBuilder::Build()
{
  ValidateInputParameters();
  
  // ----------Build Logical Volumes----------
  if (mUseEnvelope)
    mEnvelopeLv = BuildEnvelope();

  mBodyLv = BuildBody();
 // if (mSensitiveDet)
  //  mBodyLv->SetSensitiveDetector(mSensitiveDet);
  
  { // Build inner volumes of pmt body. Before building, some parameters should
    //be calculated.
    std::vector<double> innerZEdge; 
    std::vector<double> innerRhoEdge;
    G4double zLowestDynode{0.};
    int equatorIndex{0};
    
    CalculateBodyDaughterVolumesParameters(
      innerZEdge, 
      innerRhoEdge, 
      equatorIndex, 
      zLowestDynode
      );
    
    mInner1Lv = BuildInner1(innerZEdge, innerRhoEdge, equatorIndex);
   // mInner1Lv->SetSensitiveDetector(mSensitiveDet);
    
    mInner2Lv = BuildInner2(innerZEdge, innerRhoEdge, equatorIndex);
                                   
    //EvaluatePhotocathodeMinAndMaxRho(innerRhoEdge, equatorIndex);                               
    
    mDynodeLv = BuildDynode(zLowestDynode);
    BuildSkinSurfaceForDynode();
      
    //mCentralGapLv = BuildCentralGap(innerRhoEdge, equatorIndex);        
  }
  
  SetVisAttributes();
  
  // ----------Build Physical Volumes -------------
  G4PVPlacement* bodyPv{nullptr};
  if (mUseEnvelope)
    bodyPv = PlaceBodyInEnvelope();
  
  G4PVPlacement* inner1Pv     = PlaceInner1InBody();
  G4PVPlacement* inner2Pv     = PlaceInner2InBody();
  //G4PVPlacement* centralGapPv = PlaceCentralGapInBody();
  PlaceDynodeInInner2(); 
  
  
  //--------------Exterior Optical Surface-----------------
  // If we're using an envelope, bodyPv has been created and we can therefore
  // set the optical surfaces, otherwise this must be done later once the
  // physical volume has been placed. If you obtain Pmt object with GetProduct 
  //method, these surfaces will not be defined. If you obtain it with the 
  //GetProductInAnEnvelope method, they will be defined.
  if (mUseEnvelope) 
  {
    BuildPmtMirrorSurfaces(bodyPv, inner2Pv);
 
    BuildPhotoCathodeSurface(inner1Pv, bodyPv);  
    
    //CreatePmtOpticalPhotonSimulationModel(photocathodeSurface);
    return mEnvelopeLv;
  }
  
  return mBodyLv;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

void ToroidalPMTLVBuilder::ValidateInputParameters()
{
  assert(mZEdge.size() == mRhoEdge.size());
  assert(mZEdge.size() == mZOrigin.size()+1);
  
  assert(!mBodyMatName.empty());
  assert(!mPmtVacuumMatName.empty());
  assert(!mDynodeMatName.empty());
  
  assert(!mPhotocathodeOSName.empty());
  assert(!mMirrorOSName.empty());
  assert(!mDynodeOSName.empty());
  
  // assert(mPhotocathodeMinRho);
  // Do not assert mSensitiveDet. If mSensitiveDet==0, 
  // then do not associate PMT with sensitive volume
  //assert(mSensitiveDet);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume *ToroidalPMTLVBuilder::BuildEnvelope()
{
  G4double zEdgeTop    = mZEdge.at(0);
  G4double zEdgeBottom = mZEdge.at(mZEdge.size() - 1);

  G4double envZHalfLength = (zEdgeTop + mFaceGap + abs(zEdgeBottom))*0.5;
  
  G4double envRadius = *std::max_element(mRhoEdge.begin(), mRhoEdge.end());
  
  G4Tubs* envSolid = new G4Tubs(mPrefix + "_envelopeSolid", 
                                0.0, 
                                envRadius, 
                                envZHalfLength,
                                0.0, 
                                CLHEP::twopi
                                );

  return new G4LogicalVolume(envSolid, 
                             mEnvelopeMat, 
                             mPrefix + "_envelopeLv"
                             );
                                                        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

G4LogicalVolume *ToroidalPMTLVBuilder::BuildBody()
{ 
  GLG4TorusStack *bodySolid = new GLG4TorusStack(mPrefix + "_bodySolid");
  bodySolid->SetAllParameters(mZOrigin.size(),
                              &mZEdge[0], 
                              &mRhoEdge[0], 
                              &mZOrigin[0]
                             );
                         
 
  return new G4LogicalVolume(bodySolid, 
                             GetMaterial(mBodyMatName), 
                             mPrefix + "_bodyLv"
                             );
                                                                        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

G4LogicalVolume* ToroidalPMTLVBuilder::BuildInner1
(
  const std::vector<double> &innerZEdge,
  const std::vector<double> &innerRhoEdge,
  int equatorIndex
)
{
  GLG4TorusStack *inner1Solid = new GLG4TorusStack(mPrefix + "_inner1Solid");
  
  inner1Solid->SetAllParameters(equatorIndex, 
                                &innerZEdge[0], 
                                &innerRhoEdge[0],
                                &mZOrigin[0]
                                );
  
  return new G4LogicalVolume(inner1Solid, 
                             GetMaterial(mPmtVacuumMatName), 
                             mPrefix + "_inner1Lv"
                             );
                              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* ToroidalPMTLVBuilder::BuildInner2
(
  const std::vector<double> &innerZEdge,
  const std::vector<double> &innerRhoEdge,
  int equatorIndex
)
{
  GLG4TorusStack *inner2Solid = new GLG4TorusStack(mPrefix + "_inner2Solid");
  
  inner2Solid->SetAllParameters(mZOrigin.size()-equatorIndex,
                                &innerZEdge[equatorIndex],
                                &innerRhoEdge[equatorIndex],
                                &mZOrigin[equatorIndex]
                                );
   
  return new G4LogicalVolume(inner2Solid, 
                             GetMaterial(mPmtVacuumMatName), 
                             mPrefix + "_inner2Lv"
                             );                                  
                                
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* ToroidalPMTLVBuilder::BuildDynode(G4double zLowestDynode)
{
  G4double zHalfLength = (mDynodeTop - zLowestDynode)/2.0;
  G4Tubs* dynodeSolid = new G4Tubs(mPrefix + "_dynodeSolid",
                                   0.0, 
                                   mDynodeRadius,// solid cylinder (FIXME?)
                                   zHalfLength, // half height of cylinder
                                   0., 
                                   CLHEP::twopi // cylinder complete in phi
                                   );
                          
  return new G4LogicalVolume(dynodeSolid, 
                             GetMaterial(mDynodeMatName), 
                             mPrefix + "_dynodeLv"
                             );                             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilder::BuildSkinSurfaceForDynode()
{
  // build the optical surface for the dynode 
  //straight away since we already have the logical volume
  new G4LogicalSkinSurface(mPrefix + "_dynodeLS", 
                           mDynodeLv, 
                           GetOS(mDynodeOSName)
                           );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilder::SetVisAttributes()
{
  //path relative to CMAKE_BINARY_DIR
  G4String macrosDirectoryPath = "torusPMT/macros/vis/";
  G4String path = macrosDirectoryPath + mModelName + "_vis.mac";  
  
  G4UImanager::GetUIpointer()->ExecuteMacroFile(path);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
G4LogicalVolume* ToroidalPMTLVBuilder::BuildCentralGap
(
  const std::vector<double> &innerRhoEdge,
  int equatorIndex
)
{ 
  // the outer radius of the gap needs to be equal to the
  // inner radius of the PMT where inner1 and inner2 join
  G4double toleranceGapRadius = innerRhoEdge[equatorIndex]; 

  G4Tubs* centralGapSolid = new G4Tubs(
    mPrefix + "_centralGapSolid",
    0.0 , 
    toleranceGapRadius,   // solid cylinder with same radius as PMT
    mCentralHalfHightGap, //tolerance gap between inner1 and inner2, 
                          //needed to prevent overlap due to floating point 
                          //roundoff.half the needed gap between the front and 
                          //back of the PMT  
    0., 
    CLHEP::twopi // cylinder complete in phi
    );  
 
  return new G4LogicalVolume(centralGapSolid, 
                             GetMaterial(mPmtVacuumMatName), 
                             mPrefix + "_centralGapLv"
                             );                              

}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PVPlacement* ToroidalPMTLVBuilder::PlaceBodyInEnvelope()
{
  G4Tubs* envTubs = static_cast<G4Tubs*>(mEnvelopeLv->GetSolid());
  G4double envZHalfLength = envTubs->GetZHalfLength();
  
  G4double zEdgeTop    = mZEdge.at(0); //zEquator
  G4double zEdgeBottom = mZEdge.at(mZEdge.size() - 1);
  
  G4ThreeVector translation(0, 0, abs(zEdgeBottom) - envZHalfLength);

  return new G4PVPlacement(
    0,                 // no rotation
    translation, 
    mBodyLv,           // the logical volume
    mPrefix + "_bodyPv", // a name for this physical volume
    mEnvelopeLv,       // the mother volume
    false,             // no boolean ops
    0,                // copy number
    false
    );                  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PVPlacement* ToroidalPMTLVBuilder::PlaceInner1InBody()
{  
  G4ThreeVector noTranslation(0,0,0);
   
  return new G4PVPlacement( 
    0,                    // no rotation
    noTranslation,                                    
    mInner1Lv,            // the logical volume
    mPrefix + "_inner1Pv",  // a name for this physical volume
    mBodyLv,              // the mother volume
    false,                // no boolean ops
    0,                    // copy number
    false
    );                     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4PVPlacement* ToroidalPMTLVBuilder::PlaceInner2InBody()
{
  G4ThreeVector noTranslation(0,0,0);
   
  return new G4PVPlacement( 
    0,                   // no rotation
    noTranslation,       
    mInner2Lv,           // the logical volume
    mPrefix + "_inner2Pv", // a name for this physical volume
    mBodyLv,             // the mother volume
    false,               // no boolean ops
    0,                   // copy number
    false
    );                    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
G4PVPlacement* ToroidalPMTLVBuilder::PlaceCentralGapInBody()
{
  return new G4PVPlacement( 
    0,                              // no rotation
    G4ThreeVector(0.0, 0.0, mCentralHalfHightGap), 
    mCentralGapLv,                  // the logical volume
    mPrefix + "_centralGapPv",      // a name for this physical volume
    mBodyLv,                        // the mother volume
    false,                          // no boolean ops
    0,                              // copy number
    false
    );                                 
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilder::PlaceDynodeInInner2()
{
  // place dynode in stem/back,inside inner2
  G4Tubs* dynodeSolid = static_cast<G4Tubs*>(mDynodeLv->GetSolid());
  G4double dynodeZHalfLength = dynodeSolid->GetZHalfLength();
  
  new G4PVPlacement( 
    0,
    G4ThreeVector(0.0, 0.0, mDynodeTop - dynodeZHalfLength),
    mDynodeLv,
    mPrefix + "_dynodePv",
    mInner2Lv,
    false,
    0,
    true 
    );
                              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
/*
void ToroidalPMTLVBuilder::EvaluatePhotocathodeMinAndMaxRho
(
  const std::vector<G4double>& innerRhoEdge,
  int equatorIndex
)
{
// Evalute MIN photocathode
  if (mPhotocathodeMinRho != 0.)
  {
    G4cout << "Value of photocathode MIN. " << 
    mPhotocathodeMinRho << ", max:" <<
    innerRhoEdge[equatorIndex] << 
    G4endl;
  }else
  {
    G4cout << "Setting value of photocathode MIN. " << 
    mPhotocathodeMinRho << " to " <<
    innerRhoEdge[equatorIndex] << 
    G4endl;

    mPhotocathodeMinRho = innerRhoEdge[equatorIndex];
  }
   
  // Evalute MAX photocathode, if inputed
  if (mPhotocathodeMaxRho != 0.)
  {
    G4cout << "Value of photocathode MAX. " << 
    mPhotocathodeMaxRho << ", max:" <<
    innerRhoEdge[equatorIndex] << 
    G4endl;
  }else
  {
    G4cout << "Setting value of photocathode MAX. " << 
    mPhotocathodeMaxRho << " to " <<
    innerRhoEdge[equatorIndex] << 
    G4endl;

    mPhotocathodeMaxRho = innerRhoEdge[equatorIndex];
  }
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  
void ToroidalPMTLVBuilder::BuildPmtMirrorSurfaces
(
  G4PVPlacement *bodyPv,
  G4PVPlacement *inner2Pv
)
{
  /* Set the optical surfaces for a PMT. This must be called 
  *after* the physical PMT has been placed
     If this is not done, the mirror surface is not created.
  */
  //build the mirrored surface
  new G4LogicalBorderSurface(mPrefix + "_mirrorLS1",
                             inner2Pv,
                             bodyPv,
                             GetOS(mMirrorOSName)
                            );
                              
  new G4LogicalBorderSurface(mPrefix + "_mirrorLS2",
                             bodyPv, 
                             inner2Pv,
                             GetOS(mMirrorOSName)
                             );
 /*
  // also include the tolerance gap
  new G4LogicalBorderSurface(mPrefix + "_centralGapLS1",
                             centralGapPv,
                             bodyPv,
                             GetOS(mMirrorOSName)
                             );
                             
  new G4LogicalBorderSurface(mPrefix + "_centralGapLS2",
                             bodyPv,
                             centralGapPv,
                             GetOS(mMirrorOSName)
                             );
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilder::BuildPhotoCathodeSurface
(
  G4PVPlacement* inner1Pv,
  G4PVPlacement* bodyPv
)
{
  new G4LogicalBorderSurface(mPrefix + "_photocathodeSurface1",
                                    inner1Pv, 
                                    bodyPv,
                                    GetOS(mPhotocathodeOSName)
                                    );
                                    
  new G4LogicalBorderSurface(mPrefix + "_photocathodeSurface2",
                                    bodyPv, 
                                    inner1Pv,
                                    GetOS(mPhotocathodeOSName)
                                    );                                    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
void ToroidalPMTLVBuilder::CreatePmtOpticalPhotonSimulationModel
(
  G4LogicalBorderSurface* photocathodeSurfaceace
)
{
  // ------------ FastSimulationModel -------------
  // 28-Jul-2006 WGS: Must define a G4Region for Fast Simulations
  // (change from Geant 4.7 to Geant 4.8).
 //  G4Region* bodyRegion = new G4Region(mPrefix+"_GLG4_PMTOpticalRegion");
 //  bodyRegion->AddRootLogicalVolume(mBodyLv);
  
   //GLG4PMTOpticalModel * pmtOpticalModel 
  
   new GLG4PMTOpticalModel(mPrefix+"_optical_model", bodyRegion, mBodyLv,
         photocathodeSurfaceace, mEfficiencyCorrection,
         mDynodeTop, mDynodeRadius,
         mPrepulseProb,
         mPhotocathodeMinRho,mPhotocathodeMaxRho);
   
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ToroidalPMTLVBuilder::CalculateBodyDaughterVolumesParameters
(
  std::vector<double> &innerZEdge,
  std::vector<double> &innerRhoEdge,
  int &equatorIndex,
  double &zLowestDynode
)
{
  // Local references
  const G4double wall = mWallThickness;
  const std::vector<G4double> &outerZEdge = mZEdge;
  const std::vector<G4double> &outerRhoEdge = mRhoEdge;
  const int nEdge = mZEdge.size() - 1;

  // set shapes of inner volumes, scan for lowest allowed point of dynode
  zLowestDynode = mDynodeTop;
  innerZEdge.resize(mZEdge.size());
  innerRhoEdge.resize(mRhoEdge.size());

  // We will have to calculate the inner dimensions of the PMT.
  G4ThreeVector norm;
  equatorIndex = -1;

  // calculate inner surface edges, check dynode position, and find equator
  innerZEdge[0]   = outerZEdge[0] - wall;
  innerRhoEdge[0] = 0.0;
  
  for (int i=1; i < nEdge; i++) 
  {
    norm = mBodyLv->GetSolid()->SurfaceNormal(G4ThreeVector(0.0, 
                                              outerRhoEdge[i], 
                                              outerZEdge[i]) 
                                              );
    innerZEdge[i]  = outerZEdge[i]   - wall * norm.z();
    innerRhoEdge[i] = outerRhoEdge[i] - wall * norm.y();
    if (innerRhoEdge[i] > mDynodeRadius && innerZEdge[i] < zLowestDynode)
      zLowestDynode= innerZEdge[i];
    if (outerZEdge[i] == 0.0 || innerZEdge[i] == 0.0)
      equatorIndex= i;
  }

  innerZEdge[nEdge]  = outerZEdge[nEdge]   + wall;
  innerRhoEdge[nEdge]= outerRhoEdge[nEdge] - wall;

  // one final check on dynode allowed position
  if (innerRhoEdge[nEdge] > mDynodeRadius && innerZEdge[nEdge] < zLowestDynode)
    zLowestDynode= innerZEdge[nEdge];

  // sanity check equator index
  if (equatorIndex < 0)
  {
    G4ExceptionDescription ed;
    ed << "Pathological PMT shape with no equator edge";
    G4Exception("ToroidalPMTLVBuilder::CalculateBodyDaughterVolumesParameters()", 
                "error0", 
                FatalException, 
                ed
                );
  }
  
  // sanity check on dynode height  
  if (mDynodeTop > innerZEdge[equatorIndex])
  {
    G4ExceptionDescription ed;
    ed << "Top of PMT dynode cannot be higher than equator.";
    G4Exception("ToroidalPMTLVBuilder::CalculateBodyDaughterVolumesParameters()", 
                "error1", 
                FatalException, 
                ed
                ); 
  }
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

G4Material* ToroidalPMTLVBuilder::GetMaterial(const G4String& name)
{
  return ToroidalPMTMaterialFactory::GetInstance()->Get(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

G4OpticalSurface* ToroidalPMTLVBuilder::GetOS(const G4String& name)
{
  return ToroidalPMTOSFactory::GetInstance()->Get(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
// UI Commands

void ToroidalPMTLVBuilder::SetZEdge(const std::vector<double>& pZEdge) 
{
  mZEdge = pZEdge;
}

void ToroidalPMTLVBuilder::SetRhoEdge(const std::vector<double>& pRhoEdge) 
{
  mRhoEdge = pRhoEdge; 
}

void ToroidalPMTLVBuilder::SetZOrigin(const std::vector<double>& pZOrigin) 
{
  mZOrigin = pZOrigin; 
}

void ToroidalPMTLVBuilder::SetFaceGap(G4double pFaceGap)
{
  mFaceGap = pFaceGap;
}
      
void ToroidalPMTLVBuilder::SetDynodeRadius(G4double pDynodeRadius)
{
  mDynodeRadius = pDynodeRadius;
}

void ToroidalPMTLVBuilder::SetDynodeTop(G4double pDynodeTop)
{
  mDynodeTop = pDynodeTop;
}

void ToroidalPMTLVBuilder::SetWallThickness(G4double pWallThickness)
{
  mWallThickness = pWallThickness;
}

/*
void ToroidalPMTLVBuilder::SetPhotocathodeMinRho(G4double pPhotocathodeMinRho)
{
  mPhotocathodeMinRho = pPhotocathodeMinRho;
}

void ToroidalPMTLVBuilder::SetPhotocathodeMaxRho(G4double pPhotocathodeMaxRho)
{
  mPhotocathodeMaxRho = pPhotocathodeMaxRho;
}

void ToroidalPMTLVBuilder::SetPrepulseProb(G4double pPrepulseProb)
{
  mPrepulseProb = pPrepulseProb;
}

void ToroidalPMTLVBuilder::SetEfficiencyCorrection(G4double pEfficiencyCorrection)
{
  mEfficiencyCorrection = pEfficiencyCorrection;
}
*/

void ToroidalPMTLVBuilder::SetBodyMaterialName(const G4String& name)
{
  mBodyMatName = name;
}   

void ToroidalPMTLVBuilder::SetPmtVaccumMaterialName(const G4String& name)
{
  mPmtVacuumMatName = name;
} 

void ToroidalPMTLVBuilder::SetDynodeMaterialName(const G4String& name)
{
  mDynodeMatName = name;
}

void ToroidalPMTLVBuilder::SetPhotocathodeOSName(const G4String& name)
{
  mPhotocathodeOSName = name;
} 

void ToroidalPMTLVBuilder:: SetMirrorOSName(const G4String& name)
{
  mMirrorOSName = name;
}

void ToroidalPMTLVBuilder::SetDynodeOSName(const G4String& name)
{
  mDynodeOSName = name;
} 

