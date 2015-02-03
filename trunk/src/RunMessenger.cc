/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "RunMessenger.hh"
#include "SPcRunAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunMessenger::RunMessenger(SPcRunAction* run): 
    frun(run)
{
    runIdCommand = new G4UIcmdWithAnInteger("/SPc/setRun",this);
    runIdCommand->SetGuidance("Set run id number");
    //runIdCommand->SetDefaultValue(1);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunMessenger::~RunMessenger(){
    delete runIdCommand;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
    
    if( command == runIdCommand ){
	frun->SetRunID(runIdCommand->GetNewIntValue(newValue));}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

                                           
