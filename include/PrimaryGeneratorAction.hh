/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class DetectorConstruction;
class G4VPrimaryGenerator;
 
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		PrimaryGeneratorAction( DetectorConstruction* );

		virtual ~PrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event*);

	private:  

		G4VPrimaryGenerator* InitializeGPS();
		G4VPrimaryGenerator* gun;
		DetectorConstruction* myDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
