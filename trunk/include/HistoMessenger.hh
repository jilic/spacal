/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef HistoMessenger_h
#define HistoMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class HistoManager;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class HistoMessenger: public G4UImessenger
{
	public:

		HistoMessenger(HistoManager*);
		virtual ~HistoMessenger();

		virtual void SetNewValue(G4UIcommand*, G4String);

	private:

		HistoManager*			fHistoManager;
		G4UIdirectory*			fHistoDir;
		G4UIcmdWithAString*		fHistoNameCmd;
		G4UIcmdWithABool*		fHistoEventCmd;
		G4UIcmdWithABool*		fHistoPPCmd;
		G4UIcmdWithABool*		fHistoLostCmd;

};

#endif
