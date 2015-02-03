/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#ifndef SPcMuonPhysics_h
#define SPcMuonPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuIonisation.hh"
#include "G4hIonisation.hh"

#include "G4MuonMinusCaptureAtRest.hh"

class SPcMuonPhysics : public G4VPhysicsConstructor
{
  public:

    SPcMuonPhysics(const G4String& name="muon");
    virtual ~SPcMuonPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

  protected:

   // Muon physics
   G4MuIonisation*         fMuPlusIonisation;
   G4MuMultipleScattering* fMuPlusMultipleScattering;
   G4MuBremsstrahlung*     fMuPlusBremsstrahlung;
   G4MuPairProduction*     fMuPlusPairProduction;

   G4MuIonisation*         fMuMinusIonisation;
   G4MuMultipleScattering* fMuMinusMultipleScattering;
   G4MuBremsstrahlung*     fMuMinusBremsstrahlung;
   G4MuPairProduction*     fMuMinusPairProduction;

   G4MuonMinusCaptureAtRest* fMuMinusCaptureAtRest;

};

#endif
