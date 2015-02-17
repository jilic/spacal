/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  gun = InitializeGPS();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  gun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPrimaryGenerator* PrimaryGeneratorAction::InitializeGPS()
{
	G4GeneralParticleSource * gps = new G4GeneralParticleSource();

	// setup details easier via UI commands see gps.mac

	// particle type
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* ele = particleTable->FindParticle("e-");  
	gps->GetCurrentSource()->SetParticleDefinition(ele);

	// set energy distribution
	G4SPSEneDistribution *eneDist = gps->GetCurrentSource()->GetEneDist() ;
	eneDist->SetEnergyDisType("Mono"); // or gauss
	eneDist->SetMonoEnergy(200.0*MeV);

	// set position distribution
	G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
	
	//**pod uglom od 3deg u odnosu na osu calo bloka
	double beta =3.0*pi/180.0;
    G4double positionZ = -200.0*mm;	
    G4double positionY = -200.0*std::tan(beta);
	posDist->SetCentreCoords(G4ThreeVector(0.0*mm,positionY,positionZ));
	//set direction
	G4SPSAngDistribution *angDist = gps->GetCurrentSource()->GetAngDist();
	angDist->SetParticleMomentumDirection( G4ThreeVector(0., std::tan(beta), 1.) );
	
	//****
	
	//paralelno osi calo bloka
    //G4double positionZ = -200.0*mm;	
    //G4double positionY = 0.0*mm;	
    //G4double positionX = 0.0*mm;	
	//posDist->SetCentreCoords(G4ThreeVector(0.0*mm,0.0*mm,positionZ));
	//// set direction
	//G4SPSAngDistribution *angDist = gps->GetCurrentSource()->GetAngDist();
	//angDist->SetParticleMomentumDirection( G4ThreeVector(0., 0, 1.) );

	//****

	std::cout<<"Beam position (y, z) = " << positionY <<", "<<positionZ<< std::endl;	
	
	
	
	
	return gps;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

