/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcSteppingAction_H
#define SPcSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "SPcSteppingMessenger.hh"

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
		void TrackScintill(G4bool);
		void SetCutOff(G4double);

	private:
		
		SPcSteppingMessenger* fstepMessenger;
		SPcRunAction*    fRunAct;
		HistoManager* fHistoManager;
		G4bool fscintill;
		G4double fcutOff;
		G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
