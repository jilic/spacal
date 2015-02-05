/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcEventAction_h
#define SPcEventAction_h 1

#include "SPcUserEventInformation.hh"
#include "SPcEventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class SPcRunAction;
class HistoManager;
class DetectorConstruction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class SPcEventAction : public G4UserEventAction
{
	public:

		SPcEventAction(SPcRunAction*, HistoManager*, DetectorConstruction* );
		virtual ~SPcEventAction();

	public:

		virtual void BeginOfEventAction(const G4Event*);
		virtual void EndOfEventAction(const G4Event*);

		void SetSaveThreshold(G4int );
		void SetEventVerbose(G4int v){fVerbose=v;}
		void SetPMTThreshold(G4int t){fPMTThreshold=t;}
		void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
		void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}
		void WriteToFile(bool write, SPcUserEventInformation* evtInfo);

	private:
		SPcRunAction*    fRunAct;
		HistoManager* fHistoManager;
		DetectorConstruction* fDetector;
		SPcEventMessenger* fEventMessenger;

		G4int              fSaveThreshold;
		G4int              fScintCollID;
		G4int              fPMTCollID;
		G4int              fVerbose;
		G4int              fPMTThreshold;

		G4bool fForcedrawphotons;
		G4bool fForcenophotons;

};

#endif
