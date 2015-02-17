/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Winter 2015
*/

#ifndef SPcPMT_h
#define SPcPMT_h 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4OpticalSurface.hh"

#include "DetectorConstruction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SPcPMT : public G4PVPlacement
{
	public:
		SPcPMT(G4RotationMatrix *pRot,
				const G4ThreeVector &tlate,
				G4LogicalVolume *pMotherLogical,
				G4bool pMany,
				G4int pCopyNo,
				DetectorConstruction* c);

		//~SPcPMT();
	private:

		void VisAttributes();
		void SurfaceProperties();
		void CopyValues();

		G4bool fUpdated;

		DetectorConstruction* fConstructor;

		//Dimensions
		G4double fcaloX;
		G4double fcaloY;	
		G4double fthic;	
		int fNx;	
		int fNy;	

		//materials
		G4Material* fpmtMaterial; 
		G4Material* fphotocatMaterial; 

		//Volumes
		G4Box* pmtSolid;
		G4Box* photocathSolid;

		//Logical Volumes
		static G4LogicalVolume * pmtLogic;
		G4LogicalVolume * photocathLogic;

		//Sensitive Detectors
		static SPcPMTSD* fPmt_SD;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
