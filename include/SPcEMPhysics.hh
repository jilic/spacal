/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcEMPhysics_h
#define SPcEMPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

class SPcEMPhysics : public G4VPhysicsConstructor
{
  public:

    SPcEMPhysics(const G4String& name ="EM");
    virtual ~SPcEMPhysics();

  public:

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

  protected:

   // Gamma physics
    G4PhotoElectricEffect* fPhotoEffect;
    G4ComptonScattering* fComptonEffect;
    G4GammaConversion* fPairProduction;
 
    // Electron physics
    G4eMultipleScattering* fElectronMultipleScattering;
    G4eIonisation* fElectronIonisation;
    G4eBremsstrahlung* fElectronBremsStrahlung;
 
    //Positron physics
    G4eMultipleScattering* fPositronMultipleScattering;
    G4eIonisation* fPositronIonisation;
    G4eBremsstrahlung* fPositronBremsStrahlung;
    G4eplusAnnihilation* fAnnihilation;
};

#endif
