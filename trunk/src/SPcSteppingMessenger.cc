/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcSteppingMessenger.hh"
#include "SPcSteppingAction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcSteppingMessenger::SPcSteppingMessenger(SPcSteppingAction* step): 
    fstep(step)
{
    trackOpticalCommand = new G4UIcmdWithABool("/SPc/boundaryProc",this);
    trackOpticalCommand->SetGuidance("Trak Scintillation photons");

	fcutOffCmd = new G4UIcmdWithADoubleAndUnit("/SPc/cutOff",this);
	fcutOffCmd->SetGuidance("Set the value of 1/e * energyOfIncidentParticle ");
	fcutOffCmd->SetParameterName("cutOff",true);
	fcutOffCmd->SetDefaultUnit("GeV");
	fcutOffCmd->SetRange("cutOff >0 ");
	fcutOffCmd->AvailableForStates(G4State_PreInit,G4State_Idle);;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcSteppingMessenger::~SPcSteppingMessenger(){
    delete trackOpticalCommand;
    delete fcutOffCmd;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcSteppingMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
    
    if( command == trackOpticalCommand ){
	fstep->TrackScintill(trackOpticalCommand->GetNewBoolValue(newValue));}

    if( command == fcutOffCmd ){
	fstep->SetCutOff(fcutOffCmd->GetNewDoubleValue(newValue));}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

                                           
