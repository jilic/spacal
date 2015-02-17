/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcSteppingMessenger_h
#define SPcSteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SPcSteppingAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

class SPcSteppingMessenger: public G4UImessenger
{
    public:

	SPcSteppingMessenger(SPcSteppingAction*);
	virtual ~SPcSteppingMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);
    private:

	SPcSteppingAction*     fstep;
	G4UIcmdWithABool*  trackOpticalCommand;
	G4UIcmdWithADoubleAndUnit*  fcutOffCmd;
};

#endif
