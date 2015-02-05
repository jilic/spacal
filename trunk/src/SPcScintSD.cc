/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/

#include "SPcScintSD.hh"
#include "SPcScintHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintSD::SPcScintSD(G4String name)
  : G4VSensitiveDetector(name)
{
  fScintCollection = NULL;
  collectionName.insert("scintCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcScintSD::~SPcScintSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcScintSD::Initialize(G4HCofThisEvent* hitsCE){

	fScintCollection = new SPcScintHitsCollection(SensitiveDetectorName,collectionName[0]);

	//A way to keep all the hits of this event in one place if needed
	static G4int hitsCID = -1;
	if(hitsCID<0){hitsCID = GetCollectionID(0);}
	
	hitsCE->AddHitsCollection( hitsCID, fScintCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SPcScintSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ){

	G4double edep = aStep->GetTotalEnergyDeposit();
	if(edep==0.) return false; //No edep so dont count as hit

	G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
	G4TouchableHistory* theTouchable =(G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
	G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();

	G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

	//Get the average position of the hit
	G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
	pos/=2.;

	SPcScintHit* scintHit = new SPcScintHit(thePrePV);

	scintHit->SetEdep(edep);
	scintHit->SetPos(pos);

	fScintCollection->insert(scintHit);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcScintSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcScintSD::clear() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcScintSD::DrawAll() {} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void SPcScintSD::PrintAll() {} 
