/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcSteppingAction.hh"
#include "SPcSteppingMessenger.hh"
#include "SPcEventAction.hh"
#include "SPcTrackingAction.hh"
#include "SPcTrajectory.hh"
#include "SPcPMTSD.hh"
#include "SPcUserTrackInformation.hh"
#include "SPcUserEventInformation.hh"

#include "HistoManager.hh"
#include "SPcRunAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include <iostream>
#include <fstream>  // C++ files
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcSteppingAction::SPcSteppingAction(SPcRunAction* run, HistoManager* histo):
	fRunAct(run),
	fHistoManager(histo),
	fscintill(false),
	fcutOff(7.358* GeV)
{
	fstepMessenger = new SPcSteppingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcSteppingAction::~SPcSteppingAction() {}
		HistoManager* fHistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcSteppingAction::TrackScintill(G4bool b){
	fscintill = b;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcSteppingAction::SetCutOff(G4double c){
	if(c<0){
		G4cout << "***********Proposed cutOff is out of range. it should be greather than 0" <<G4endl;	
		G4cout << "*********** and equal to the energy of the incindent electron" <<G4endl;	

	}else {
		fcutOff =c/exp(1) /GeV;
		std::cout<<"fcutOff = "<<fcutOff<<std::endl;
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcSteppingAction::UserSteppingAction(const G4Step * theStep){


	//the track
	G4Track* theTrack = theStep->GetTrack();

	//the particle
	G4ParticleDefinition* particleType = theTrack->GetDefinition();

	//tracking & event info
	SPcUserTrackInformation* trackInformation=(SPcUserTrackInformation*)theTrack->GetUserInformation();
	SPcUserEventInformation* eventInformation=(SPcUserEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();

	//prePoint
	G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

	//postPoint
	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

	G4OpBoundaryProcessStatus boundaryStatus=Undefined;
	static G4OpBoundaryProcess* boundary=NULL;

	//find the boundary process only once
	if(!boundary){
		G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
		G4ProcessVector*  pv = pm->GetProcessList();

		G4int nprocesses = pm->GetProcessListLength();
		//loop over the processes and check any of them is "OpBoundary"
		for( G4int i=0; i<nprocesses; i++){
			if((*pv)[i]->GetProcessName()=="OpBoundary"){
				boundary = (G4OpBoundaryProcess*)(*pv)[i];
				break;
			}
		}
	}
	
	//out of world - do not follow
	if(!thePostPV){
		return;
	}

	// 1) EXTRACT some info about the PRIMARY track
	if( fHistoManager->IsPPAna() && theTrack->GetParentID()==0){


		//how many secondaries the primary track has  produced in this step
		G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
			+ fpSteppingManager->GetfN2ndariesAlongStepDoIt()
			+ fpSteppingManager->GetfN2ndariesPostStepDoIt();

		G4int rn 			= fRunAct->getRid();
		G4int evt 			= eventInformation->getEvtID();
		G4int st    		= theTrack->GetCurrentStepNumber();
		G4double xc 		= theTrack->GetPosition().x() / mm;
		G4double yc 		= theTrack->GetPosition().y() / mm;
		G4double zc 		= theTrack->GetPosition().z() / mm;
		G4double kinen 		= theTrack->GetKineticEnergy()/ GeV;
		G4double dEStep		= theStep->GetTotalEnergyDeposit()/MeV;
		G4double sl 		= theStep->GetStepLength() / mm;
		G4double tl			= theTrack->GetTrackLength() / mm;

		G4bool fnd =eventInformation->isFound();
		if(!fnd && kinen<= fcutOff /GeV){

			eventInformation->SetFound(true);
			eventInformation->X0measurement(zc);
			std::cout<<"X0 = "<< zc <<std::endl;
			std::cout<<"X0 = "<<eventInformation->GetX0()<<std::endl;

		}

		G4String vol("");
		G4String proc("");

		if( theTrack->GetNextVolume() != 0 ) {
			vol = theTrack->GetVolume()->GetName();
		} else {
			vol = "OutOfWorld";
		}

		if(theStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL){
			proc= theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
		} else {
			proc = "UserLimit";
		}

		fHistoManager->FillNtuplePrimP(rn, evt, st, tN2ndariesTot, kinen, dEStep, xc, yc, zc, sl, tl);	

		fHistoManager->FillHisto(1, zc, kinen);
		fHistoManager->FillHisto(2, zc,1);

	}
	//2)  PARTICLES LEAVING the calorimeter
	if(fHistoManager->IsLostAna() && thePostPV->GetName()=="pWorld" && thePrePoint->GetStepStatus()== fGeomBoundary){

		//do not look at optical photons or PrimP
		if(particleType!=G4OpticalPhoton::OpticalPhotonDefinition() && theTrack->GetParentID()!=0){

			//write info about particles leaving the detector
			G4double en = thePostPoint->GetKineticEnergy() /MeV;
			if(en>0){
				G4int pdg 	=	theTrack->GetDefinition()->GetPDGEncoding();
				G4int evt 	= 	eventInformation->getEvtID();
				G4int rn 	  = 	fRunAct->getRid();
				G4double xc = 	thePostPoint->GetPosition().x() /cm;
				G4double yc = 	thePostPoint->GetPosition().y() /cm;
				G4double zc = 	thePostPoint->GetPosition().z() /cm;
				G4int stpid =theTrack->GetCurrentStepNumber(); 
				fHistoManager->FillNtupleLost(rn, evt, pdg, stpid, en, xc, yc, zc);	

			}
		}

	}
/*
//-----------
//optical photons EXITING (z>0) calorimeter
	if(particleType==G4OpticalPhoton::OpticalPhotonDefinition() ){
		G4String name = theTrack->GetCreatorProcess()->GetProcessName();
		if( name == "Scintillation"){
			G4ThreeVector position = theTrack->GetPosition();
			G4ThreeVector vertex   = theTrack->GetVertexPosition();
			G4bool transmit = ((position.z() < -11) );
			if (transmit) { eventInformation->IncExitCount();}
		}
	}
                                                                                                                                                    //---------
*/
	// 3) PMT hits , absorbtion  and other info about OpticalPhotons....
	if(fscintill){

		//3) Scintillation
		if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
			G4String name = theTrack->GetCreatorProcess()->GetProcessName();

			if( name == "Scintillation" ) {

				if (theStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && thePostPV->GetName()=="pWorld"){ 
					eventInformation->IncScintExit();

					if(theStep->GetPostStepPoint()->GetPosition().z() > -1)eventInformation->IncPosScintExit();
				}

				//Kill photons entering world
				if(thePostPV->GetName()=="pWorld"){
					theTrack->SetTrackStatus(fStopAndKill);
				}

				//Was the photon absorbed by the absorption process
				if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpAbsorption"){
					eventInformation->IncAbsorption();
					trackInformation->AddTrackStatusFlag(absorbed);
				}

				boundaryStatus=boundary->GetStatus();
				//std::cout<<boundary->GetStatus()<<std::endl;
				//Check to see if the partcile was actually at a boundary
				//Otherwise the boundary status may not be valid
				//Prior to Geant4.6.0-p1 this would not have been enough to check
				if(thePostPoint->GetStepStatus()==fGeomBoundary){
					if(fExpectedNextStatus==StepTooSmall){
						if(boundaryStatus!=StepTooSmall){
							G4ExceptionDescription ed;
							ed << "SPcSteppingAction::UserSteppingAction(): "<< "No reallocation step after reflection!"<< G4endl;
							G4Exception("SPcSteppingAction::UserSteppingAction()",	"SPcExpl02",FatalException,ed,
									"Something is wrong with the surface normal or geometry");
						}
					}
					fExpectedNextStatus=Undefined;

					switch(boundaryStatus){
						case Absorption:
							{
								eventInformation->IncBoundaryAbsorption();
								trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
								break;
							}
						case Detection:
							{
							//Triger sensitive detector manually since photon is
							//absorbed but status was Detection
							G4SDManager* SDman = G4SDManager::GetSDMpointer();
							G4String sdName="/SPcDet/pmtSD";
							SPcPMTSD* pmtSD = (SPcPMTSD*)SDman->FindSensitiveDetector(sdName);
							if(pmtSD)pmtSD->ProcessHits_constStep(theStep,NULL);
							trackInformation->AddTrackStatusFlag(hitPMT);
							break;
							}
						case FresnelReflection:
						case TotalInternalReflection:
						case LambertianReflection:
						case LobeReflection:
						case SpikeReflection:
						case BackScattering:
						default:
							break;
					}
				}
			}//scint. photon loop
		}//optical photon loop
	}
}
