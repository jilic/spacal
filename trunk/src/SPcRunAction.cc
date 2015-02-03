/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "HistoManager.hh"
#include "SPcRunAction.hh"
#include "RunMessenger.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcRunAction::SPcRunAction(HistoManager* histo): 
	fHistoManager(histo),
    runID(0)
{
frunMessenger = new RunMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcRunAction::~SPcRunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcRunAction::BeginOfRunAction(const G4Run* aRun){

	((G4Run *)(aRun))->SetRunID(runID);

	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	fHistoManager->book();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcRunAction::EndOfRunAction(const G4Run* aRun){

	//save histograms
	fHistoManager->PrintStatistic();
	fHistoManager->save();
}
