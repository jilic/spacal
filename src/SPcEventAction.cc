/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "SPcEventAction.hh"
#include "SPcScintHit.hh"
#include "SPcPMTHit.hh"
#include "SPcUserEventInformation.hh"
#include "SPcTrajectory.hh"

#include "HistoManager.hh"
#include "SPcRunAction.hh"
#include "DetectorConstruction.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>  // C++ files
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcEventAction::SPcEventAction(SPcRunAction* run, HistoManager* histo, DetectorConstruction* myDetector):
	fRunAct(run),
	fHistoManager(histo),
	fDetector(myDetector),
	fSaveThreshold(0),
	fScintCollID(-1),
	fPMTCollID(-1),
	fVerbose(1),
	fPMTThreshold(0),
	fForcedrawphotons(false),
	fForcenophotons(false)
{
  fEventMessenger = new SPcEventMessenger(this);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcEventAction::~SPcEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcEventAction::BeginOfEventAction(const G4Event* anEvent){

	G4cout << "\n---> Begin of event: " << anEvent->GetEventID() << G4endl;
	
	//New event, add the user information object
	
	SPcUserEventInformation * eventInformation = new SPcUserEventInformation;
	G4EventManager::GetEventManager()->SetUserInformation(eventInformation);
	eventInformation->SetEvtID(anEvent->GetEventID());

	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	if(fScintCollID<0)fScintCollID=SDman->GetCollectionID("scintCollection");
	if(fPMTCollID<0)fPMTCollID=SDman->GetCollectionID("pmtHitCollection");

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcEventAction::EndOfEventAction(const G4Event* anEvent){

	SPcUserEventInformation* eventInformation=(SPcUserEventInformation*)anEvent->GetUserInformation();

	G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

	// extract the trajectories and draw them
	if (G4VVisManager::GetConcreteInstance()){
		for (G4int i=0; i<n_trajectories; i++){
			SPcTrajectory* trj = (SPcTrajectory*)
				((*(anEvent->GetTrajectoryContainer()))[i]);
			if(trj->GetParticleName()=="opticalphoton"){
				trj->SetForceDrawTrajectory(fForcedrawphotons);
				trj->SetForceNoDrawTrajectory(fForcenophotons);
			}
			trj->DrawTrajectory(50);
		}
	}







	SPcScintHitsCollection* scintHC = 0;
	SPcPMTHitsCollection* pmtHC = 0;
	G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();

	//Get the hit collections
	if(hitsCE){
		if(fScintCollID>=0)	scintHC = (SPcScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
		if(fPMTCollID>=0)	pmtHC 	= (SPcPMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
	}

	//Hits in scintillator
	if(scintHC){
		int n_hit = scintHC->entries();
		G4ThreeVector  eWeightPos(0.);
		G4double edep;
		G4double edepMax=0;

		//gather info on hits in scintillator
		for(int i=0;i<n_hit;i++){
			edep=(*scintHC)[i]->GetEdep();
			eventInformation->IncEDep(edep); //sum up the edep
			eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
			if(edep>edepMax){
				edepMax=edep;//store max energy deposit
				G4ThreeVector posMax=(*scintHC)[i]->GetPos();
				eventInformation->SetPosMax(posMax,edep);
			}
		}

		if(eventInformation->GetEDep()==0.){
			if(fVerbose>0)G4cout<<"No hits in the scintillator this event."<<G4endl;
		}
		else{
			//Finish calculation of energy weighted position
			eWeightPos/=eventInformation->GetEDep();
			eventInformation->SetEWeightPos(eWeightPos);
			if(fVerbose>0){
				G4cout << "\tEnergy weighted position of hits in SPc : "<< eWeightPos/mm << G4endl;
			}
		}

		if(fVerbose>0){
			G4cout << "\tTotal energy deposition in scintillator : "<< eventInformation->GetEDep() / keV << " (keV)" << G4endl;
		}
	}

	//Hits in PMTs
	if(pmtHC){
		G4int pmts=pmtHC->entries();

		//Gather info from all PMTs
		for(G4int i=0;i<pmts;i++){
			eventInformation->IncHitCount((*pmtHC)[i]->GetPhotonCount());

			if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
				eventInformation->IncPMTSAboveThreshold();
			}else{
				//wasnt above the threshold, turn it back off
				(*pmtHC)[i]->SetDrawit(false);
			}
		}

		pmtHC->DrawAllHits();
	}

	if(fVerbose>0){
		//End of event output. later to be controlled by a verbose level
		G4cout << "\tNumber of photons produced by scintillation in this event : "
			<< eventInformation->GetPhotonCount_Scint() << G4endl;
		G4cout << "\tNumber of photons produced by cerenkov in this event : "
			<< eventInformation->GetPhotonCount_Ceren() << G4endl;
		G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
			<< eventInformation->GetAbsorptionCount() << G4endl;
		G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
			<< "this event : " << eventInformation->GetBoundaryAbsorptionCount()
			<< G4endl;
		G4cout << "Unacounted for photons in this event : "
			<< (eventInformation->GetPhotonCount_Scint() -
					eventInformation->GetAbsorptionCount() -
					eventInformation->GetHitCount() -
					eventInformation->GetBoundaryAbsorptionCount())
			<< G4endl;
	}

	//Fill in the event ntuple
	G4int rn = fRunAct->getRid();
	G4int evt= anEvent->GetEventID();
	G4int a = eventInformation->GetPhotonCount(); 
	G4int b = eventInformation->GetPhotonCount_Scint();
	G4int c = eventInformation->GetPhotonCount_Ceren();
	G4int d = eventInformation->GetHitCount();
	G4int e = eventInformation->GetAbsorptionCount();
	G4double energy = eventInformation->GetEDep() / (keV);
	
	G4double X0 = eventInformation->GetX0() / mm;
	std::cout<< "X0 = "<<X0 <<std::endl;

	int scintCnt =eventInformation->GetScintExit();
	int scintCntP =eventInformation->GetPosScintExit();
	std::cout<<"eventInformation->GetScintExit()"<<eventInformation->GetScintExit()<<std::endl;
	std::cout<<"eventInformation->GetPosScintExit()"<<eventInformation->GetPosScintExit()<<std::endl;

	if(fHistoManager->IsEventAna()) fHistoManager->FillNtupleEvt(rn, evt, a,b,c,d,e,energy, scintCnt,scintCntP, X0);	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void SPcEventAction::SetSaveThreshold(G4int save){
}
