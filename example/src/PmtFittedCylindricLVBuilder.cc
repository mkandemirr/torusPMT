#include "PmtFittedCylindricLVBuilder.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"

#include "ToroidalPMTLVFactory.hh"
#include "MaterialFactory.hh"

#include "GLG4TorusStack.hh"
#include "G4VisAttributes.hh"

#include "G4LogicalSkinSurface.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtFittedCylindricLVBuilder::PmtFittedCylindricLVBuilder
(
  const G4String& pmtModel
): 
VComponentBuilder(),

mPmtModel{pmtModel},

mDistanceBetweenSideRings{3*cm},
mSideTopRingOffset{3*cm}, //z direction
mSideBottomRingOffset{3*cm}, //z direction
mSidePmtsFrontEndDistance{0.},

mDistanceBetweenTopRings{3*cm},
mTopLastRingOffset{3*cm}, //along r direction
mTopFirstRingRadius{0.},
mTopPmtsFrontEndDistance{0.},

mDistanceBetweenBottomRings{3.*cm},
mBottomLastRingOffset{3.*cm}, //along r direction
mBottomFirstRingRadius{0.},
mBottomPmtsFrontEndDistance{0.},

mPmtEnvelopeRadius{0.},
mPmtEnvelopeZHalfLength{0.},

mInnerCylHight{0.},
mInnerCylRadius{0.},

mPmtEnvelopeLv{nullptr},
mInnerCylLv{nullptr},
mOuterCylLv{nullptr}
{
  Initialize();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtFittedCylindricLVBuilder::~PmtFittedCylindricLVBuilder()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::Initialize()
{
  InitializeSidePmtSettings();
  InitializeTopPmtSettings();
  InitializeBottomPmtSettings();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::InitializeSidePmtSettings()
{
  mSideRingsPmtCounts            = {5,5}; //from bottom to top
  mSideRingsFirstPlacedPmtThetas = {0,0};
  
  mDistanceBetweenSideRings  = 15*cm;
  mSideTopRingOffset         = 5*cm;
  mSideBottomRingOffset      = 5*cm; 
  mSidePmtsFrontEndDistance  = 0.*cm;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::InitializeTopPmtSettings()
{
  mTopRingsPmtCounts            = {1,5}; // from inner to outer
  mTopRingsFirstPlacedPmtThetas = {0,0};

  mTopFirstRingRadius        = 0*mm;
  mDistanceBetweenTopRings   = 10*cm;
  mTopLastRingOffset         = 3*cm;
  mTopPmtsFrontEndDistance   = 0.*cm; 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::InitializeBottomPmtSettings()
{
  mBottomRingsPmtCounts             = {1,5}; // from inner to outer
  mBottomRingsFirstPlacedPmtThetas  = {0,0};
  
  mBottomFirstRingRadius       = 0.*mm;
  mDistanceBetweenBottomRings  = 10*cm;
  mBottomLastRingOffset        = 3*cm;
  mBottomPmtsFrontEndDistance  = 0.*cm;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* PmtFittedCylindricLVBuilder::Build()
{
  ValidateInputParameters();
  
  mPmtEnvelopeLv  = BuildPmt(mPmtModel);
  mInnerCylLv     = BuildInnerCylinder();
  mOuterCylLv     = BuildOuterCylinder();
   
  if (mInnerCylLv && mOuterCylLv)
    PlaceInnerCylinder();
 
  if (mPmtEnvelopeLv && mOuterCylLv)
  {
    PlacePmtsOnInnerCylinderSide();
    PlacePmtsOnInnerCylinderTop();
    PlacePmtsOnInnerCylinderBottom();
  }
  
  product = mOuterCylLv;
  return product; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::ValidateInputParameters()
{
  assert(mSideRingsPmtCounts.size() == 
    mSideRingsFirstPlacedPmtThetas.size());
      
  assert(mTopRingsPmtCounts.size() == 
    mTopRingsFirstPlacedPmtThetas.size());
  
  assert(mBottomRingsPmtCounts.size() == 
      mBottomRingsFirstPlacedPmtThetas.size());
        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* PmtFittedCylindricLVBuilder::BuildPmt
(
  const G4String& modelName  
)
{   
  mPmtEnvelopeLv     = ToroidalPMTLVFactory::GetInstance()->Get(modelName);
  G4Material* envMat = MaterialFactory::GetInstance()->Get("gdDopedWater");
  mPmtEnvelopeLv->SetMaterial(envMat);
  
  { //for getting mPmtEnvelopeRadius, mPmtEnvelopeZHalfLength
    G4Tubs* pmtEnvTubs      = static_cast<G4Tubs*>(mPmtEnvelopeLv->GetSolid());
    mPmtEnvelopeRadius      = pmtEnvTubs->GetOuterRadius();
    mPmtEnvelopeZHalfLength = pmtEnvTubs->GetZHalfLength();
  }                           
  
  return mPmtEnvelopeLv; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* PmtFittedCylindricLVBuilder::BuildInnerCylinder()
{
  mInnerCylHight = mSideBottomRingOffset + mSideTopRingOffset + 
                   mSideRingsPmtCounts.size() * (2*mPmtEnvelopeRadius) +
                   (mSideRingsPmtCounts.size()-1) * 
                   mDistanceBetweenSideRings;
   
  { // for computation of cylinder radius 
  
    G4int higherNumberOfRings = std::max(mTopRingsPmtCounts.size(), 
                                  mBottomRingsPmtCounts.size());
    
    G4double higherDist = std::max(mDistanceBetweenTopRings, 
                            mDistanceBetweenBottomRings);
    
    G4double higherOffset = std::max(mTopLastRingOffset, 
                              mBottomLastRingOffset); 
    
    G4double higherFirstRingRadius = std::max(mTopFirstRingRadius, 
                                       mBottomFirstRingRadius);
                    
    mInnerCylRadius = higherFirstRingRadius + higherOffset +      
                      (higherNumberOfRings-1) * (2*mPmtEnvelopeRadius + 
                      higherDist) + mPmtEnvelopeRadius;                
  }
  
  G4Tubs* tubs = new G4Tubs("InnerCylinderSolid",
                            0, 
                            mInnerCylRadius, 
                            mInnerCylHight * 0.5,
                            0,
                            360*deg
                            );
   
  return new G4LogicalVolume(
    tubs,
    MaterialFactory::GetInstance()->Get("gdDopedWater"),
    "InnerCylinderLv"
    ); 
                                 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* PmtFittedCylindricLVBuilder::BuildOuterCylinder()
{
  G4double envelopeRadius = mInnerCylRadius + mSidePmtsFrontEndDistance +
                            2*mPmtEnvelopeZHalfLength;      
  
  G4double higherFrontEndDistance = std::max(mTopPmtsFrontEndDistance,
                                             mBottomPmtsFrontEndDistance 
                                             );
  
  G4double envelopeLength = mInnerCylHight + 2 * higherFrontEndDistance +
                            4 * mPmtEnvelopeZHalfLength;
  
  G4Tubs* envelopeTubs = new G4Tubs("OuterCylinderSolid", 
                                 0, 
                                 envelopeRadius, 
                                 envelopeLength * 0.5, 
                                 0,
                                 360 * deg
                                 );
                        
  mOuterCylLv = new G4LogicalVolume(
    envelopeTubs,
    MaterialFactory::GetInstance()->Get("gdDopedWater"),
    "OuterCylinderLv"
    );
  
  return mOuterCylLv;                                     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlaceInnerCylinder()
{
  new G4PVPlacement(
    nullptr,  // no rotation
    G4ThreeVector(0,0,0), // at (0,0,0)
    mInnerCylLv, // its logical volume
    "InnerCylinderPv", // its name
    mOuterCylLv, // its mother  volume
    false, // no boolean operations
    0, // copy number
    true // checking overlaps
    );    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlacePmtsOnInnerCylinderSide()
{
  //Before placement, positions and rotations of pmts should be determined.
  const G4double r = mInnerCylRadius + mSidePmtsFrontEndDistance +
                     mPmtEnvelopeZHalfLength;                    
  G4double z{0.};
  
  PmtThetas pmtThetas;
  PmtRotationAnglesList pmtRotAnglesList;
   
  { //for z
    G4double zInitialPosition = -mInnerCylHight*0.5 + 
                                 mSideBottomRingOffset + 
                                 mPmtEnvelopeRadius;
    z = zInitialPosition;
  }
  
  { // for placing side rings                             
    G4double deltaz = mDistanceBetweenSideRings + 2*mPmtEnvelopeRadius;
    G4int numberOfRings = mSideRingsPmtCounts.size();
    for (int i = 0; i < numberOfRings; ++i)
    {
      G4int numberOfPmts = mSideRingsPmtCounts.at(i);
      
      if (ArePmtsOverlappedInTheRing(r, numberOfPmts))
        throw std::runtime_error("Side Pmts overlaps!! ");
        
      FillPmtThetasAndRotationAnglesList
      (
        numberOfPmts, 
        pmtThetas, 
        pmtRotAnglesList,
        mSideRingsFirstPlacedPmtThetas.at(i)
      );
      
      PlaceRing(r, pmtThetas, z, pmtRotAnglesList);
       
      pmtThetas.clear();               
      pmtRotAnglesList.clear();
      
      z+= deltaz;
    
    }
  
  }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlacePmtsOnInnerCylinderTop()
{
  //Before placement, positions and rotations of pmts should be determined.
  const G4double z = mInnerCylHight * 0.5 + mTopPmtsFrontEndDistance + 
                     mPmtEnvelopeZHalfLength;
  G4double r       = mTopFirstRingRadius;
                     
  PmtThetas pmtThetas;
  PmtRotationAnglesList pmtRotAnglesList; // for all PMTs in a ring

  { // for placing top rings 
    G4double deltar = 2*mPmtEnvelopeRadius + mDistanceBetweenTopRings;
    G4int numberOfRings = mTopRingsPmtCounts.size();
    for (int i = 0; i < numberOfRings; ++i )
    {
      G4int numberOfPmts = mTopRingsPmtCounts.at(i);
      
      if (ArePmtsOverlappedInTheRing(r, numberOfPmts))
        throw std::runtime_error("Top PMTs overlapped! ");
        
      FillPmtThetas
      (
        numberOfPmts, 
        pmtThetas, 
        mTopRingsFirstPlacedPmtThetas.at(i)
      );
      
      FillPmtRotationAnglesList(numberOfPmts, pmtRotAnglesList, true);
      
      PlaceRing(r, pmtThetas, z, pmtRotAnglesList);
       
      pmtThetas.clear();               
      pmtRotAnglesList.clear();
     
      r+=deltar;

    }
  
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlacePmtsOnInnerCylinderBottom()
{
  //Before placement, positions and rotations of pmts should be determined.
  const G4double z = -mInnerCylHight*0.5 - mBottomPmtsFrontEndDistance - 
                      mPmtEnvelopeZHalfLength;
  G4double r       = mBottomFirstRingRadius;   
  
  PmtThetas pmtThetas;
  PmtRotationAnglesList pmtRotAnglesList; //for all PMTs in a ring
  
  { // for placing bottom rings
    G4double deltar = 2*mPmtEnvelopeRadius + mDistanceBetweenBottomRings; 
    G4int numberOfRings = mBottomRingsPmtCounts.size();
    for (int i = 0; i < numberOfRings; ++i )
    {
      G4int numberOfPmts = mBottomRingsPmtCounts.at(i);
      
      if (ArePmtsOverlappedInTheRing(r, numberOfPmts))
        throw std::runtime_error("Bottom PMTs overlapped! ");
      
      FillPmtThetas
      (
        numberOfPmts, 
        pmtThetas, 
        mBottomRingsFirstPlacedPmtThetas.at(i)
      );
        
      FillPmtRotationAnglesList(numberOfPmts, pmtRotAnglesList, false);
      
      PlaceRing(r, pmtThetas, z, pmtRotAnglesList);
       
      pmtThetas.clear();               
      pmtRotAnglesList.clear();
     
      r+=deltar;
     
    }
  
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::FillPmtThetas
(
  G4int numberOfPmts, 
  PmtThetas& pmtThetas,
  G4double firstPlacedPmtTheta
)
{

  G4double deltaTheta = (360./numberOfPmts)*deg;
  G4double theta      = firstPlacedPmtTheta*deg;

  //Get theta angles and rotation angles for all Pmts in ring i
  for (int j = 0; j < numberOfPmts; j++ )
  {
    pmtThetas.push_back(theta);
    theta+= deltaTheta;
  }
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::FillPmtRotationAnglesList
(
  G4int numberOfPmts, 
  PmtRotationAnglesList& pmtRotAnglesList,
  G4bool isTopRing
)
{
  PmtRotationAngles pmtRotAngles; 
  for (int j = 0; j < numberOfPmts; j++ )
  {
    if (isTopRing)
      pmtRotAngles.push_back(std::make_pair('y',180*deg));
    else //bottom ring
      pmtRotAngles.push_back(std::make_pair('y',0*deg)); //no ratation
      
    pmtRotAnglesList.push_back(pmtRotAngles);
    pmtRotAngles.clear();
  }  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::FillPmtThetasAndRotationAnglesList
(
  G4int numberOfPmts,
  PmtThetas& pmtThetas,
  PmtRotationAnglesList& pmtRotAnglesList,
  G4double firstPlacedPmtTheta
)
{
  G4double deltaTheta = (360./numberOfPmts)*deg;
  G4double theta      = firstPlacedPmtTheta*deg;
  
  PmtRotationAngles pmtRotAngles;
  for (int j = 0; j < numberOfPmts; j++ )
  {
    pmtThetas.push_back(theta);

    pmtRotAngles.push_back(std::make_pair('z',-theta));
    pmtRotAngles.push_back(std::make_pair('y',90*deg));

    pmtRotAnglesList.push_back(pmtRotAngles);
    
    pmtRotAngles.clear();
    theta+= deltaTheta;
  }
  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlaceRing
(
  G4double r,
  PmtThetas pmtThetas,  
  G4double z, 
  PmtRotationAnglesList pmtRotAnglesList
 )
{
    assert(pmtRotAnglesList.size()==pmtThetas.size());
    
    G4int numPMTs = pmtThetas.size();
             
    for (int i = 0; i < numPMTs; ++i)
    {
      PlacePmt(r, pmtThetas.at(i), z, pmtRotAnglesList.at(i));            
    } 
       
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtFittedCylindricLVBuilder::PlacePmt
(
  G4double r, 
  G4double theta, 
  G4double z,
  PmtRotationAngles rotAngles
)
{

  G4RotationMatrix *pmtRot = new G4RotationMatrix();
  
  for (int i = 0; i < rotAngles.size(); ++i)
  {
    if (rotAngles.at(i).first == 'x')
      pmtRot->rotateX( rotAngles.at(i).second );
    else if (rotAngles.at(i).first == 'y')
      pmtRot->rotateY( rotAngles.at(i).second );
    else if (rotAngles.at(i).first == 'z')
    pmtRot->rotateZ( rotAngles.at(i).second );  
  
  }
  
  G4double x = r*cos(theta);  
  G4double y = r*sin(theta); 
     
  G4PVPlacement* pmtPv = new G4PVPlacement(
    pmtRot, // no rotation
    G4ThreeVector(x,y,z), // at (0,0,0)
    mPmtEnvelopeLv, // its logical volume
    mPmtModel + "_PmtEnvelopePv", // its name
    mOuterCylLv, // its mother  volume
    false, // no boolean operations
    mPmtPvs.size(), // copy number
    false // checking overlaps
    );  
                                  
  mPmtPvs.push_back(pmtPv);
 
}  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PmtFittedCylindricLVBuilder::ArePmtsOverlappedInTheRing
(
  G4double r,
  G4double numberOfPmts
)
{
  // Algorithm for checking Pmt overlaps in a ring: 
  // deltaThetaMin = 2*asin(mPmtEnvelopeRadius/r);
  // singlular point at r < mPmtEnvelopeRadius
  // for r = mPmtEnvelopeRadius, deltaThetaMin = 180 degree, and
  // maxNumbersOfPmt = 360/180 = 2.
  // for r < mPmtEnvelopeRadius, maxNumbersOfPmt should be 1.
  
  if (r < mPmtEnvelopeRadius && numberOfPmts == 1)
  {
    return false;
  }else if (r < mPmtEnvelopeRadius && numberOfPmts > 1) 
  {
    throw std::runtime_error("For r < mPmtEnvelopeRadius,"
      "numberOfPmts should be 1!");
  }
  
  if (numberOfPmts == 0)
    return false;
    
  G4double deltaTheta    = (360./numberOfPmts)*deg;    
  G4double deltaThetaMin = 2*asin(mPmtEnvelopeRadius/r);
  
  if (deltaTheta < deltaThetaMin)
    return true;
  else
    return false; 
}



