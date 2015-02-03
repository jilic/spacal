/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "SPcEMPhysics.hh"
#include "G4UserSpecialCuts.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcEMPhysics::SPcEMPhysics(const G4String& name)
               :  G4VPhysicsConstructor(name)
{
    fPhotoEffect = NULL;
    fComptonEffect = NULL;
    fPairProduction = NULL;
    fElectronMultipleScattering = NULL;
    fElectronIonisation = NULL; 
    fElectronBremsStrahlung = NULL;
    fPositronMultipleScattering = NULL;
    fPositronIonisation = NULL; 
    fPositronBremsStrahlung = NULL;
    fAnnihilation = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcEMPhysics::~SPcEMPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

void SPcEMPhysics::ConstructParticle()
{
  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ProcessManager.hh"

void SPcEMPhysics::ConstructProcess()
{
    fPhotoEffect = new G4PhotoElectricEffect();
    fComptonEffect = new G4ComptonScattering();
    fPairProduction = new G4GammaConversion();

    // Electron physics
    fElectronMultipleScattering = new G4eMultipleScattering();
    fElectronIonisation = new G4eIonisation();
    fElectronBremsStrahlung = new G4eBremsstrahlung();

    //Positron physics
    fPositronMultipleScattering = new G4eMultipleScattering();
    fPositronIonisation = new G4eIonisation();
    fPositronBremsStrahlung = new G4eBremsstrahlung();
    fAnnihilation = new G4eplusAnnihilation();

    G4ProcessManager* pManager = 0;

    // Gamma Physics
    pManager = G4Gamma::Gamma()->GetProcessManager();
    pManager->AddDiscreteProcess(fPhotoEffect);
    pManager->AddDiscreteProcess(fComptonEffect);
    pManager->AddDiscreteProcess(fPairProduction);

    //====================================
    /*
      ______NOTE:JELENA_____
     
      AddProcess sintax: 
     G4int G4ProcessManager::AddProcess(
     G4VProcess *aProcess,
     G4int      ordAtRestDoIt,
     G4int      ordAlongStepDoIt,
     G4int      ordPostStepDoIt 
     )
     
      //                                                       atRest,    AlongStep,  PostStep        
      // pmanager->AddProcess(new G4eMultipleScattering(),       a,       b,          c);
     
      There are *3 ordered types of processes* (3 kind of acctions) per particle which are 
      active at different stages of the Geant4 tracking:
      
      -AtRest         (decay, e+ annihilation, …)
      -AlongStep      (to describe continuous (inter)actions occuring along the path of the particle like ionisation, Bremsstrahlung, …)
      -PostStep       (point like (inter)actions, like decay in flight, hard radiation, photo-electric, Compton, Cerenkov,….)
     
      NOTE 1 : 
      - Transportation is the FIRST (order=0) for AlongStep and PostStep
      - if order<0  - process is not active
      - othervise the integer gives *order* of DoIt method
      - Descreat Processess - have only PostStep actions
      - Inverse order of GetInteractionLenght 
     
      NOTE 2 : 
      -Process ordering is NOT CRITICAL except for multiple-scattering and transportation!
      -In the physicsList one SHOULD ALWAYS have for the *ordering AlongGetPhysicalInteractionLength(...)* metods
      (i.e. inverse to AddProcess ordering)
      -transportation -last; for all particles
      -multiple scaterring - second last 
     
      so, of *n* processes the ordering must be:
      [n-3] ... other processes
      [n-2] ... other processes
      [n-1] multiple scaterring
      [n]  transportation
     
     */

    // Electron Physics
    pManager = G4Electron::Electron()->GetProcessManager();

    pManager->AddProcess(fElectronMultipleScattering, -1, 1, 1);
    pManager->AddProcess(fElectronIonisation,         -1, 2, 2);
    pManager->AddProcess(fElectronBremsStrahlung,     -1, 3, 3);  
    //kill all electrons in tungsten with energy below 500 keV
    pManager->AddProcess(new G4UserSpecialCuts(),     -1,-1, 4);

    //Positron Physics
    pManager = G4Positron::Positron()->GetProcessManager();

    pManager->AddProcess(fPositronMultipleScattering, -1, 1, 1);
    pManager->AddProcess(fPositronIonisation,         -1, 2, 2);
    pManager->AddProcess(fPositronBremsStrahlung,     -1, 3, 3);  
    pManager->AddProcess(fAnnihilation,                0,-1, 4);  

}
