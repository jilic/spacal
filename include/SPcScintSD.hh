/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcScintSD_h
#define SPcScintSD_h 1

#include "SPcScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SPcScintSD : public G4VSensitiveDetector
{
  public:

    SPcScintSD(G4String name);
    virtual ~SPcScintSD();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
 
  private:

    SPcScintHitsCollection* fScintCollection;
 
};

#endif
