/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcSteppingVerbose_h
#define SPcSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class SPcSteppingVerbose : public G4SteppingVerbose
{
  public:

    SPcSteppingVerbose();
    virtual ~SPcSteppingVerbose();

    virtual void StepInfo();
    virtual void TrackingStarted();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
