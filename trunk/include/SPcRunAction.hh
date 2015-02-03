/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "G4UserRunAction.hh"
#include "RunMessenger.hh"
#include "globals.hh"

#ifndef SPcRunAction_h
#define SPcRunAction_h 1

class HistoManager;
class SPcRunAction : public G4UserRunAction
{
    public:

	SPcRunAction(HistoManager*);
	virtual ~SPcRunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
	void SetRunID (G4int i) {runID = i;}
	G4int getRid() {return runID;}

    private:
	HistoManager* fHistoManager;
	RunMessenger* frunMessenger;
	G4int runID;
};

#endif
