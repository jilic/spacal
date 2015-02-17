/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcStackingMessenger.hh"
#include "SPcStackingAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcStackingMessenger::SPcStackingMessenger(SPcStackingAction* stack): 
    fstack(stack)
{
    trackScintillCommand = new G4UIcmdWithABool("/SPc/trackScintill",this);
    trackScintillCommand->SetGuidance("Track Scintillation photons");

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcStackingMessenger::~SPcStackingMessenger(){
    delete trackScintillCommand;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcStackingMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
    
    if( command == trackScintillCommand ){
	fstack->KillOrNot(trackScintillCommand->GetNewBoolValue(newValue));}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

                                           
