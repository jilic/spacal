/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcStackingAction_H
#define SPcStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class SPcStackingAction : public G4UserStackingAction
{
  public:

    SPcStackingAction();
    virtual ~SPcStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
