/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcStackingMessenger_h
#define SPcStackingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SPcStackingAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class SPcStackingMessenger: public G4UImessenger
{
    public:

	SPcStackingMessenger(SPcStackingAction*);
	virtual ~SPcStackingMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);
    private:

	SPcStackingAction*  fstack;
	G4UIcmdWithABool*   trackScintillCommand;
};

#endif
