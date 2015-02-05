/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "SPcPMTSD.hh"
#include "SPcScintSD.hh"
#include "DetectorMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class G4ChordFinder;
class G4UniformMagField;

//class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\brief This mandatory user class defines the geometry.

It is responsible for
 - Definition of material, and
 - Construction of geometry
\sa Construct()
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction();
		~DetectorConstruction();
	public:
		//! Construct geometry of the setup
		G4VPhysicalVolume* Construct();
		G4VPhysicalVolume* ConstructDetector();

		//Functions to modify the geometry
		void SetDiceType(G4bool );
		void SetPMTzDistance(G4double);

		//rebuild the geometry based on changes. must be called
		void UpdateGeometry();
		G4bool GetUpdated(){return fUpdated;}
		//! initialize geometry parameters
		void SetDefaults();


		//! \name some simple set & get functions
		//@{
		G4double      FieldValue() const { return fieldValueZ; } 
		G4double      SetFieldValue( G4double value) { return fieldValueZ= value; }  

		G4double      GetCaloSizeX() { return emCaloWidth; }  
		G4double      GetCaloSizeY() { return emCaloWidth; }  
		G4double      GetCaloZ() { return emCaloLength; }  
		//@}
	private:
		DetectorMessenger* fDetectorMessenger;
		void DefineMaterials();
		G4VPhysicalVolume* ConstructEMCalo();
		void ConstructTubes();
		void ConstructTubesPARAM();
		void PlaceTubes();
		void ConstructField();
		void ConstructPMTs();
		void SetVisibilityAttributes();

	private:
		G4bool fUpdated;	

		//! \name global parameters
		//@{
		G4double halfWorldLength;
		G4double emCaloLength;
		G4double emCaloWidth;
		G4double halfMirrorSizeZ;
		G4double halfPreshowerSizeZ;
		G4ThreeVector posEmCalo;
		G4double holeOutherR;
		G4double capillaryOutherR;
		G4double coreOutherR;
		G4double pitch;
		G4double fieldValueX;
		G4double fieldValueY;
		G4double fieldValueZ;
		G4int fNx;
		G4int fNy;
		G4double fD_mtl; // pmt's tickness
		G4double pmtZ;
		G4bool dice5;
		G4bool param;
		//@}

		//! \name Materials
		//@{
		G4Material* air;
		G4Material* vacuum;
		G4Material* wolfram;
		G4Material* quartz;
		G4Material* ej309;
		G4Material* fGlass;
		G4Material* fAl;
		G4MaterialPropertiesTable* ej309_mt;
		//@}


		//! \name Solids
		//@{
		G4Box* pmtSolid;
		G4Box* photocathSolid;
		//@}

		//! \name Logic volume
		//@{
		G4LogicalVolume * worldLogic;
		G4LogicalVolume * mirrorLogic;
		G4LogicalVolume * preshowerLogic;
		G4LogicalVolume * caloLogic;
		G4LogicalVolume * holeLogic;
		G4LogicalVolume * capillaryLogic;
		G4LogicalVolume * coreLogic;
		G4LogicalVolume * pmtLogic;
		G4LogicalVolume * photocathLogic;
		//@}

		//! \name Physical volume
		//@{
		G4VPhysicalVolume* holePhys;
		G4VPhysicalVolume* mirrorPhys;
		G4VPhysicalVolume* preshowerPhys;
		G4VPhysicalVolume* corePhys;
		G4VPhysicalVolume* capillaryPhys;
		G4VPhysicalVolume* caloPhys;
		//@}


		//! \name Visualisation attribute
		//@{
		G4VisAttributes* inVisAtt;
		G4VisAttributes* whiteVisAtt;
		G4VisAttributes* dkgreyVisAtt;
		G4VisAttributes* medyelVisAtt;
		G4VisAttributes* yellowVisAtt ;
		G4VisAttributes* redVisAtt;
		G4VisAttributes* greenVisAtt;
		G4VisAttributes* blueVisAtt;
		G4VisAttributes* ograyVisAtt;
		//@}

		//! \name Sensitive Detectors
		//@{
		static SPcScintSD* fScint_SD;
		static SPcPMTSD* fPmt_SD;
		//@}

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
