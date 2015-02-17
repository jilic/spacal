/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcStackingAction_H
#define SPcStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "SPcStackingMessenger.hh"

class SPcStackingAction : public G4UserStackingAction
{
  public:

    SPcStackingAction();
    virtual ~SPcStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
	void KillOrNot(G4bool b); 
  private:
		SPcStackingMessenger* fstackMessenger;
		G4bool follow;

};

#endif
