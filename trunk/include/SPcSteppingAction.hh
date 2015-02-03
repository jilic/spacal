/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcSteppingAction_H
#define SPcSteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class SPcEventAction;
class SPcTrackingAction;
class SPcSteppingMessenger;
class HistoManager;
class SPcRunAction;

class SPcSteppingAction : public G4UserSteppingAction
{
	public:

		SPcSteppingAction(SPcRunAction*, HistoManager* );
		virtual ~SPcSteppingAction();
		virtual void UserSteppingAction(const G4Step*);


	private:
		
		SPcRunAction*    fRunAct;
		HistoManager* fHistoManager;

		SPcSteppingMessenger* fSteppingMessenger;
		G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
