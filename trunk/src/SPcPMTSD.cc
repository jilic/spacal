/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "SPcPMTSD.hh"
#include "SPcPMTHit.hh"
#include "DetectorConstruction.hh"
#include "SPcUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcPMTSD::SPcPMTSD(G4String name)
  : G4VSensitiveDetector(name),fPMTHitCollection(0),fPMTPositionsX(0)
  ,fPMTPositionsY(0),fPMTPositionsZ(0)
{
	collectionName.insert("pmtHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcPMTSD::~SPcPMTSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//  G4HCofThisEvent  is a class which stores hits collections generated at one event.
// This class is exclusively constructed by G4SDManager when the first
// hits collection of an event is passed to the manager, and this class
// object is deleted by G4RunManager when a G4Event class object is deleted.
//  Almost all public methods must be used by Geant4 kernel classes and
// the user should not invoke them. The user can use two const methods,
// GetHC() and GetNumberOfCollections() for accessing to the stored hits
// collection(s).


void SPcPMTSD::Initialize(G4HCofThisEvent* hitsCE){
	
	fPMTHitCollection = new SPcPMTHitsCollection(SensitiveDetectorName,collectionName[0]);
	
	//Store collection with event and keep ID
	static G4int hitCID = -1;
	if(hitCID<0){hitCID = GetCollectionID(0);}
	hitsCE->AddHitsCollection( hitCID, fPMTHitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SPcPMTSD::ProcessHits(G4Step* ,G4TouchableHistory* ){
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool SPcPMTSD::ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* ){

	//need to know if this is an optical photon
	if(aStep->GetTrack()->GetDefinition()!= G4OpticalPhoton::OpticalPhotonDefinition()) return false;

	//User replica number 1 since photocathode is a daughter volume
	//to the pmt which was replicated
	G4int pmtNumber=aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
	G4VPhysicalVolume* physVol=aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

	//Find the correct hit collection
	G4int n=fPMTHitCollection->entries();
	SPcPMTHit* hit=NULL;
	for(G4int i=0;i<n;i++){
		if((*fPMTHitCollection)[i]->GetPMTNumber()==pmtNumber){
			hit=(*fPMTHitCollection)[i];
			break;
		}
	}

	if(hit==NULL){//this pmt wasnt previously hit in this event
		hit = new SPcPMTHit(); //so create new hit
		hit->SetPMTNumber(pmtNumber);
		hit->SetPMTPhysVol(physVol);
		fPMTHitCollection->insert(hit);
		hit->SetPMTPos((*fPMTPositionsX)[pmtNumber],(*fPMTPositionsY)[pmtNumber],
				(*fPMTPositionsZ)[pmtNumber]);
	}

	hit->IncPhotonCount(); //increment hit for the selected pmt

	hit->SetDrawit(true);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMTSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMTSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMTSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMTSD::PrintAll() {}
