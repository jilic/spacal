/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcTrajectory.hh"
#include "SPcTrackingAction.hh"
#include "SPcUserTrackInformation.hh"
#include "SPcUserEventInformation.hh"
#include "DetectorConstruction.hh"
#include "SPcUserEventInformation.hh"
#include "G4EventManager.hh"
#include "SPcEventAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcTrackingAction::SPcTrackingAction() 
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	//Let this be up to the user via vis.mac
	  
	fpTrackingManager->SetStoreTrajectory(true);

	//Use custom trajectory class
	fpTrackingManager->SetTrajectory(new SPcTrajectory(aTrack));

	//This user track information is only relevant to the photons
	fpTrackingManager->SetUserTrackInformation(new SPcUserTrackInformation);

	/*  const G4VProcess* creator = aTrack->GetCreatorProcess();
		if(creator)
		G4cout<<creator->GetProcessName()<<G4endl;
	 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcTrackingAction::PostUserTrackingAction(const G4Track* aTrack ){

	SPcTrajectory* trajectory=(SPcTrajectory*)fpTrackingManager->GimmeTrajectory();
	SPcUserTrackInformation*trackInformation=(SPcUserTrackInformation*)aTrack->GetUserInformation();

	//Lets choose to draw only the photons that hit a pmt
	if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

		if(trackInformation->GetTrackStatus()&hitPMT) trajectory->SetDrawTrajectory(true);
	}
	else if(aTrack->GetParentID()==0){
		trajectory->SetDrawTrajectory(true);
	} else {
		trajectory->SetDrawTrajectory(false);
	}
	
	if(trackInformation->GetForceDrawTrajectory())
		trajectory->SetDrawTrajectory(true);


}
