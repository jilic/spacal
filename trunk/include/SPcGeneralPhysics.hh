/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcGeneralPhysics_h
#define SPcGeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

#include "G4Decay.hh"

class SPcGeneralPhysics : public G4VPhysicsConstructor
{
  public:

    SPcGeneralPhysics(const G4String& name = "general");
    virtual ~SPcGeneralPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

  protected:

    G4Decay* fDecayProcess;
};

#endif
