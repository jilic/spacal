/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcEventMessenger_h
#define SPcEventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SPcEventAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class SPcEventMessenger: public G4UImessenger
{
  public:

    SPcEventMessenger(SPcEventAction*);
    virtual ~SPcEventMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    SPcEventAction*        fSPcEvent;
    G4UIcmdWithAnInteger*  fSaveThresholdCmd;
    G4UIcmdWithAnInteger*  fVerboseCmd;
    G4UIcmdWithAnInteger*  fPmtThresholdCmd;
    G4UIcmdWithABool*      fForceDrawPhotonsCmd;
    G4UIcmdWithABool*      fForceDrawNoPhotonsCmd;
};

#endif
