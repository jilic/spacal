/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Winter 2015
*/

#ifndef SPcCalorimeter_h
#define SPcCalorimeter_h 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4OpticalSurface.hh"

#include "DetectorConstruction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SPcCalorimeter : public G4PVPlacement
{
	public:
		SPcCalorimeter(G4RotationMatrix *pRot,
				const G4ThreeVector &tlate,
				G4LogicalVolume *pMotherLogical,
				G4bool pMany,
				G4int pCopyNo,
				DetectorConstruction* c);

		//~SPcCalorimeter();
	private:

		void VisAttributes();
		void SurfaceProperties();
		void ConstructTubes();
		void PlaceTubes();
		void CopyValues();

		G4bool fUpdated;

		DetectorConstruction* fConstructor;

		//Dimensions
		G4double fcaloX;
		G4double fcaloY;	
		G4double fcaloZ;	
		G4double fholeR;	
		G4double fcapillR;
		G4double fcoreR;	
		G4double fspacing;
		G4bool	fdice5;		

		//calculated values
		G4int fnoHoles; 
		G4double fshift; 
		G4double ffirstPoint; 

		//materials
		G4Material* caloMaterial; 
		G4Material* tubeMaterial; 
		G4Material* scintMaterial;
		G4Material* holeMaterial; 

		//Volumes
		G4Box* caloSolid;
		G4Tubs*	holeSolid;
		G4Tubs*	capillarySolid;
		G4Tubs*	coreSolid;

		//Logical Volumes
		static	G4LogicalVolume * caloLogic;
		G4LogicalVolume * holeLogic;
		G4LogicalVolume * capillaryLogic;
		G4LogicalVolume * coreLogic;

		//Phys Volumes
		G4VPhysicalVolume * caloPhys;
		G4VPhysicalVolume * holePhys;
		G4VPhysicalVolume * capillaryPhys;
		G4VPhysicalVolume * corePhys;


		//Sensitive Detectors
		static SPcScintSD* fScint_SD;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
