/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef RunMessenger_h
#define RunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SPcRunAction;
class G4UIcmdWithAnInteger;

class RunMessenger: public G4UImessenger
{
    public:

	RunMessenger(SPcRunAction*);
	virtual ~RunMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);
    private:

	SPcRunAction*           frun;
	G4UIcmdWithAnInteger*  runIdCommand;
};

#endif
