//***********************************************************************
//* Neutrino Segmented Detector () Copyright 2022 Mustafa Kandemir  *
//*                                                                     *
//* This file is part of .                                          *
//*                                                                     *
//*  is free software: you can redistribute it and/or modify        * 
//* it under the terms of the GNU General Public License as published   *
//* by the Free Software Foundation, either version 3 of the License,   *
//* or any later version.                                               *
//*                                                                     *
//*  is distributed in the hope that it will be useful, but         *
//* WITHOUT ANY WARRANTY; without even the implied warranty of          *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        * 
//* GNU General Public License for more details.                        *
//*                                                                     *
//* You should have received a copy of the GNU General Public License   *
//* along with . If not, see <https://www.gnu.org/licenses/>.       *
//*                                                                     *
//***********************************************************************

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
