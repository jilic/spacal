/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Scintillation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
 : fDetector(detector)
{
	//Setup a command directory for detector controls with guidance
	fDetectorDir = new G4UIdirectory("/SPc/detector/");
	fDetectorDir->SetGuidance("Detector geometry control");


	//Various commands for modifying detector geometry
	fDiceCmd = new G4UIcmdWithABool("/SPc/detector/dice5",this);
	fDiceCmd->SetGuidance("Set the dice4 or dice5. dice4 is default");

	fPmtZDistanceCmd = new G4UIcmdWithADoubleAndUnit("/SPc/detector/pmtZDist",this);
	fPmtZDistanceCmd->SetGuidance("Set the Z distance of the PMTs from the Calo block");
	fPmtZDistanceCmd->SetParameterName("zDistance",false);
	fPmtZDistanceCmd->SetDefaultUnit("cm");

	fUpdateCmd = new G4UIcommand("/SPc/detector/update",this);
	fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
	fUpdateCmd->SetGuidance("Must be run before beamOn if detector has been changed.");

	fDefaultsCmd = new G4UIcommand("/SPc/detector/defaults",this);
	fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
	fDefaultsCmd->SetGuidance("(Update still required)");


	//fMainScintYield=new G4UIcmdWithADouble("/SPc/detector/MainScintYield",this);
	//fMainScintYield->SetGuidance("Set scinitillation yield of main volume.");
	//fMainScintYield->SetGuidance("Specified in photons/MeV");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDiceCmd;
  delete fPmtZDistanceCmd;
  delete fUpdateCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if( command == fDiceCmd ){
		fDetector->SetDiceType(fDiceCmd->GetNewBoolValue(newValue));
	}
	else if (command == fPmtZDistanceCmd){
		fDetector->SetPMTzDistance(fPmtZDistanceCmd->GetNewDoubleValue(newValue));
	}
	else if (command == fUpdateCmd){
		fDetector->UpdateGeometry();
	}
	else if (command == fDefaultsCmd){
		fDetector->SetDefaults();
	}
}
