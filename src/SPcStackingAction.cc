/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcStackingAction.hh"
#include "SPcUserEventInformation.hh"
#include "SPcSteppingAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcStackingAction::SPcStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcStackingAction::~SPcStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack SPcStackingAction::ClassifyNewTrack(const G4Track * aTrack){

	SPcUserEventInformation* eventInformation=(SPcUserEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();

	//Count what process generated the optical photons
	if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition() && aTrack->GetParentID()>0){
		
			if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation"){
			eventInformation->IncPhotonCount_Scint();
			return fKill;	
			//return fUrgent;
		}else if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov"){
			eventInformation->IncPhotonCount_Ceren();
			return fKill;	
		}
	}else{
		return fUrgent;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcStackingAction::NewStage() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcStackingAction::PrepareNewEvent() {}
