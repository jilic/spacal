/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcTrackingAction_h
#define SPcTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"


class SPcEventAction;

class SPcTrackingAction : public G4UserTrackingAction {

  public:

    SPcTrackingAction();
    virtual ~SPcTrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
  private:

};

#endif
