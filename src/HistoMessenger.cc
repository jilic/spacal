/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "HistoMessenger.hh"
#include "HistoManager.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoMessenger::HistoMessenger(HistoManager* histo):
	fHistoManager(histo)
{
	//Setup a command directory for detector controls with guidance
	fHistoDir = new G4UIdirectory("/SPc/histo/");
	fHistoDir->SetGuidance("histogram control");

	//Various
	fHistoNameCmd = new G4UIcmdWithAString("/SPc/histo/name",this);
	fHistoNameCmd->SetGuidance("Set the output root name");

	fHistoEventCmd = new G4UIcmdWithABool("/SPc/histo/event",this);
	fHistoEventCmd->SetGuidance("write or not event tree");
	
	fHistoPPCmd = new G4UIcmdWithABool("/SPc/histo/pp",this);
	fHistoPPCmd->SetGuidance("write or not prim particle tree");
	
	fHistoLostCmd = new G4UIcmdWithABool("/SPc/histo/lost",this);
	fHistoLostCmd->SetGuidance("write or not lost particles tree");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoMessenger::~HistoMessenger()
{
	delete fHistoNameCmd;
	delete fHistoEventCmd;
	delete fHistoPPCmd;
	delete fHistoLostCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 

	if( command == fHistoNameCmd ){
		fHistoManager->SetRootName(newValue);
	}
	else if (command == fHistoEventCmd){
		fHistoManager->SetEventTree(fHistoEventCmd->GetNewBoolValue(newValue));
	}
	else if (command == fHistoPPCmd){
		fHistoManager->SetPPTree(fHistoPPCmd->GetNewBoolValue(newValue));
	}
	else if (command == fHistoLostCmd){
		fHistoManager->SetLostTree(fHistoLostCmd->GetNewBoolValue(newValue));
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

