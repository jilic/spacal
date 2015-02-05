/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

#include "G4ScoringManager.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "SPcPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SPcEventAction.hh"
#include "SPcStackingAction.hh"
#include "SPcSteppingAction.hh"
#include "SPcTrackingAction.hh"
#include "SPcRunAction.hh"
#include "SPcSteppingVerbose.hh"


#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include <iostream>
#include <fstream>  // C++ files
#include <iomanip>


namespace {

    void PrintUsage() {
	G4cerr << " Usage: " << G4endl;
	G4cerr << "  [-m macro ]" ;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


int main(int argc, char** argv)
{

	// Evaluate arguments
	if ( argc > 3 ) {
		PrintUsage();
		return 1;
	}

	G4String macro;

	for ( G4int i=1; i<argc; i=i+2 ) {
		if      ( G4String(argv[i]) == "-m" ) macro		= argv[i+1];
		else {
			PrintUsage();
			return 1;
		}
	}

	// Choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
	G4long seed = time(NULL);
	CLHEP::HepRandom::setTheSeed(seed);
	//CLHEP::HepRandom::setTheSeed(111111);

	G4VSteppingVerbose::SetInstance(new SPcSteppingVerbose);

	G4RunManager* runManager = new G4RunManager;

	// Activate UI-command base scorer
	G4ScoringManager* scoringManager = G4ScoringManager::GetScoringManager();
	scoringManager->SetVerboseLevel(1);

	DetectorConstruction* detector = new DetectorConstruction;
	runManager->SetUserInitialization(detector);
	runManager->SetUserInitialization(new SPcPhysicsList);

	// set a HistoManager
	HistoManager*  histo = new HistoManager();

	runManager->SetUserAction(new PrimaryGeneratorAction(detector));
	
	runManager->SetUserAction(new SPcStackingAction);
	
	SPcRunAction* run_action = new SPcRunAction(histo);
	runManager->SetUserAction(run_action);
  
	SPcEventAction* evt_action = new SPcEventAction(run_action,histo, detector);
	runManager->SetUserAction(evt_action);
	
	runManager->SetUserAction(new SPcSteppingAction(run_action,histo));
	
	runManager->SetUserAction(new SPcTrackingAction);

	runManager->Initialize();

#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	// get the pointer to the UI manager and set verbosities
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
//UImanager->ApplyCommand("/tracking/verbose 2");
	if(argc==1){
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute vis.mac");
#endif
		ui->SessionStart();
		delete ui;
#endif

	}
	else if( macro.size() ){
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);
	}

#ifdef G4VIS_USE
	delete visManager;
#endif

	// job termination
	delete histo;
	delete runManager;

	return 0;
}
