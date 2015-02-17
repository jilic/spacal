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
class SPcPMTSD;
class SPcScintSD;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction();
		~DetectorConstruction();
	public:

		G4VPhysicalVolume* Construct();
		G4VPhysicalVolume* ConstructDetector();

		void UpdateGeometry();
		void SetDefaults();
		G4bool GetUpdated(){return fUpdated;}

		//some methods for changing geometry
		void   SetPitch(G4double);  
		void   SetHoleR(G4double);  
		void   SetCapillaryR(G4double);  
		void   SetCoreR(G4double);  

		void   SetDiceType(G4bool );
		void   SetMagField(G4bool);  
		void   SetMirror(G4bool);  
		void   SetPsw(G4bool);  
		void   SetCalo(G4bool);  
		void   SetPmts(G4bool);  
		void   SetNbRadLenghtsPsw(G4double);  
		void   SetNbPmts(G4int);  

		G4double   GetemCaloWidth()  { return emCaloWidth; }  
		G4double   GetemCaloLength() { return emCaloLength; }  
		G4double   GetCaloZ() { return emCaloLength; }
		G4double   GetHoleR() { return holeOutherR; }  
		G4double   GetCapillR() { return capillaryOutherR; }  
		G4double   GetCoreR() { return coreOutherR; }  
		G4double   GetPitch() { return pitch; }  
		G4bool     GetDice() { return dice5; }  
		G4double GetPmtThickness(){return fD_mtl;}
		G4int GetNx() {return fNx;}
		G4int GetNxy(){return fNy;}
		

		G4Material* GetVacuum(){return vacuum;}
		G4Material* GetPmtMaterial(){return fGlass;}
		G4Material* GetPhotocatMaterial(){return fAl;}
		G4Material* GetCaloMaterial(){return wolfram;}
		G4Material* GetTubeMaterial(){return quartz;}
		G4Material* GetScintMaterial(){return ej309;}
		G4Material* GetHoleMaterial(){return air;} 	
		



	private:
		DetectorMessenger* fDetectorMessenger;
		void DefineMaterials();
		void ConstructField();

	private:
		G4bool fUpdated;	

		//solids and logic
		G4Box * worldSolid;
		G4Box * mirrorSolid;
		G4Box * preshowerSolid;

		G4LogicalVolume * worldLogic;
		G4LogicalVolume * mirrorLogic;
		G4LogicalVolume * preshowerLogic;

		G4VPhysicalVolume* worldPhys;
		G4VPhysicalVolume* mirrorPhys;
		G4VPhysicalVolume* preshowerPhys;
		
		//dimensions
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
		//
		 G4bool dice5;
		 G4bool fpresh;
		 G4bool fpmts;
		 G4bool fmirror;
		 G4bool ffield;
		 G4bool fcalorimeter;

		// Materials
		G4Material* air;
		G4Material* vacuum;
		G4Material* wolfram;
		G4Material* quartz;
		G4Material* ej309;
		G4Material* fGlass;
		G4Material* fAl;
		G4MaterialPropertiesTable* ej309_mt;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
