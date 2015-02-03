/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "SPcScintHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4Allocator<SPcScintHit> SPcScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintHit::SPcScintHit(): 
	fEdep(0.), 
	fPos(0.), 
	fPhysVol(0) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintHit::SPcScintHit(G4VPhysicalVolume* pVol) : fPhysVol(pVol) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintHit::~SPcScintHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintHit::SPcScintHit(const SPcScintHit &right) : G4VHit()
{
  fEdep		= right.fEdep;
  fPos 		= right.fPos;
  fPhysVol	= right.fPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SPcScintHit& SPcScintHit::operator=(const SPcScintHit &right){
  fEdep		= right.fEdep;
  fPos		= right.fPos;
  fPhysVol	= right.fPhysVol;
  
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SPcScintHit::operator==(const SPcScintHit&) const{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcScintHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void SPcScintHit::Print() {}
