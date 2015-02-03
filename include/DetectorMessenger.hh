/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class DetectorMessenger: public G4UImessenger
{
	public:

		DetectorMessenger(DetectorConstruction*);
		virtual ~DetectorMessenger();

		virtual void SetNewValue(G4UIcommand*, G4String);

	private:

		DetectorConstruction*		fDetector;
		G4UIdirectory*              fDetectorDir;
		G4UIcmdWithADoubleAndUnit*  fPmtZDistanceCmd;
		G4UIcmdWithABool*           fDiceCmd;
		G4UIcommand*                fUpdateCmd;
		G4UIcommand*                fDefaultsCmd;

};

#endif
