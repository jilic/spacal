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
	fUpdateCmd = new G4UIcommand("/SPc/detector/update",this);
	fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
	fUpdateCmd->SetGuidance("Must be run before beamOn if detector has been changed.");

	fDefaultsCmd = new G4UIcommand("/SPc/detector/defaults",this);
	fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
	fDefaultsCmd->SetGuidance("(Update still required)");

	fDiceCmd = new G4UIcmdWithABool("/SPc/detector/dice5",this);
	fDiceCmd->SetGuidance("Set the dice4 or dice5. dice4 is default");

	fpitchCmd = new G4UIcmdWithADoubleAndUnit("/SPc/detector/pitch",this);
	fpitchCmd->SetGuidance("Set the pitch (in mm)");
	fpitchCmd->SetParameterName("pitch",true);
	fpitchCmd->SetDefaultUnit("mm");
	fpitchCmd->SetRange("pitch >0 ");
	fpitchCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;

	fholeCmd = new G4UIcmdWithADoubleAndUnit("/SPc/detector/holeR",this);
	fholeCmd->SetGuidance("Set the radius of holes (in mm)");
	fholeCmd->SetParameterName("holeR",true);
	fholeCmd->SetDefaultUnit("mm");
	fholeCmd->SetRange("holeR >0");
	fholeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;

	fcapillCmd = new G4UIcmdWithADoubleAndUnit("/SPc/detector/capillR",this);
	fcapillCmd->SetGuidance("Set the radius of capillaries (in mm)");
	fcapillCmd->SetParameterName("capillR",true);
	fcapillCmd->SetDefaultUnit("mm");
	fcapillCmd->SetRange("capillR> 0");
	fcapillCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;

	fcoreCmd = new G4UIcmdWithADoubleAndUnit("/SPc/detector/coreR",this);
	fcoreCmd->SetGuidance("Set the radius of the scintill core (in mm)");
	fcoreCmd->SetParameterName("coreR",true);
	fcoreCmd->SetDefaultUnit("mm");
	fcoreCmd->SetRange("coreR >0");
	fcoreCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;

	fcaloCmd = new G4UIcmdWithABool("/SPc/detector/calo",this);
	fcaloCmd->SetParameterName("calo",true);
	fcaloCmd->SetGuidance("construct the calo or not");

	fpswCmd = new G4UIcmdWithABool("/SPc/detector/psw",this);
	fpswCmd->SetParameterName("psw",true);
	fpswCmd->SetGuidance("construct the preshower or not");

	fmirrorCmd = new G4UIcmdWithABool("/SPc/detector/mirror",this);
	fmirrorCmd->SetParameterName("mirror",true);
	fmirrorCmd->SetGuidance("construct the mirror or not");

	ffieldCmd = new G4UIcmdWithABool("/SPc/detector/magfield",this);
	ffieldCmd->SetParameterName("magfield",true);
	ffieldCmd->SetGuidance("switch on the magnetic field");

	fpmtsCmd = new G4UIcmdWithABool("/SPc/detector/pmts",this);
	fpmtsCmd->SetParameterName("pmts",true);
	fpmtsCmd->SetGuidance("construct PMTs");

	fNoPmtsCmd = new G4UIcmdWithAnInteger("/SPc/detector/nbPmts",this);
	fNoPmtsCmd->SetGuidance("Set the number of PMTs along the x and y  axis (default 5)");
	fNoPmtsCmd->SetParameterName("nbPmts",false);
	fNoPmtsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;

	fpswNoRadLenghtsCmd = new G4UIcmdWithADouble("/SPc/detector/pswNbX0",this);
	fpswNoRadLenghtsCmd	->SetGuidance("Set the thickness of the preshower in the units of RadLenght");
	fpswNoRadLenghtsCmd	->SetParameterName("pswNbX0",true);
	fpswNoRadLenghtsCmd	->AvailableForStates(G4State_PreInit,G4State_Idle);;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
	delete fDiceCmd;
	delete fpitchCmd;
	delete fDefaultsCmd;
	delete fUpdateCmd;
    delete fholeCmd;
    delete fcapillCmd;
    delete fcoreCmd;
    delete fcaloCmd;
    delete fpswCmd;
    delete fmirrorCmd;
    delete ffieldCmd;
    delete fpmtsCmd;
    delete fNoPmtsCmd;
    delete fpswNoRadLenghtsCmd ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if( command == fDiceCmd ){
		fDetector->SetDiceType(fDiceCmd->GetNewBoolValue(newValue));
	}

	if (command == fUpdateCmd){
		fDetector->UpdateGeometry();
	}
	if (command == fDefaultsCmd){
		fDetector->SetDefaults();
	}
	if(command == fpitchCmd){
		fDetector->SetPitch(fpitchCmd->GetNewDoubleValue(newValue));
	}
	if(command == fholeCmd){
		fDetector->SetHoleR(fholeCmd->GetNewDoubleValue(newValue));
	}
	if(command == fcapillCmd){
		fDetector->SetCapillaryR(fcapillCmd->GetNewDoubleValue(newValue));
	}
	if(command == fcoreCmd){
		fDetector->SetCoreR(fcoreCmd->GetNewDoubleValue(newValue));
	}
	if(command == fcaloCmd){
		fDetector->SetCalo(fcaloCmd->GetNewBoolValue(newValue));
	}
	if(command == fpswCmd){
		fDetector->SetPsw(fpswCmd->GetNewBoolValue(newValue));
	}
	if(command == fmirrorCmd){
		fDetector->SetMirror(fmirrorCmd->GetNewBoolValue(newValue));
	}
	if(command == ffieldCmd){
		fDetector->SetMagField(ffieldCmd->GetNewBoolValue(newValue));
	}
	if(command == fpmtsCmd){
		fDetector->SetPmts(fpmtsCmd->GetNewBoolValue(newValue));
	}
	if(command == fNoPmtsCmd){
		fDetector->SetNbPmts(fNoPmtsCmd->GetNewIntValue(newValue) );
	}
	if( command == fpswNoRadLenghtsCmd ){ 
		fDetector->SetNbRadLenghtsPsw(fpswNoRadLenghtsCmd->GetNewDoubleValue(newValue));
	}
}
