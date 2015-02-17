/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   February 2015
*/

#include "SPcPMT.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "SPcPMTSD.hh"
#include "G4SystemOfUnits.hh"

SPcPMTSD* SPcPMT::fPmt_SD=NULL;

G4LogicalVolume* SPcPMT::pmtLogic=NULL;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcPMT::SPcPMT(G4RotationMatrix *pRot,const G4ThreeVector &tlate,G4LogicalVolume *pMotherLogical,G4bool pMany,G4int pCopyNo,
		DetectorConstruction* c):
	G4PVPlacement(pRot,
		tlate,
		//Temp logical volume must be created here
		new G4LogicalVolume(new G4Box("temp",1,1,1),G4Material::GetMaterial("Vacuum"),"temp",0,0,0),
		"pPmt",
		pMotherLogical,
		pMany,
		pCopyNo),
	fConstructor(c)
{
	CopyValues();

	if(!pmtLogic || fUpdated){
		/*
		 * create pmts
		 * set sensitive detectors
		 */

		//-------create PMTs
		G4double pmtxy = fcaloX/fNx; 
		pmtSolid = new G4Box("sPmt",pmtxy/2.0,pmtxy/2.0, fthic/2.);
		pmtLogic = new G4LogicalVolume(pmtSolid, fpmtMaterial, "lPmt");

		// The "photocathode" is a metal slab at the back of the glass that
		// is only a very rough approximation of the real thing since it only
		// absorbs or detects the photons based on the efficiency set below
		photocathSolid = new G4Box("sPhotocath", pmtxy/2.0, pmtxy/2.0, fthic/4.);
		photocathLogic = new G4LogicalVolume(photocathSolid, fphotocatMaterial,"lPhotocath");

		//place photocath inside pmt
		new G4PVPlacement(0,					//no rotation
				G4ThreeVector(0,0,fthic/4.0),	//translation
				photocathLogic,					//it's log volume
				"pPhotocath",					//it's name
				pmtLogic,						// mother volume
				false,							//
				0);								// copy no


		//pmt sensitive detector
		//Created here so it exists as pmts are being placed

		G4SDManager* SDman = G4SDManager::GetSDMpointer();	
		if(!fPmt_SD){
			fPmt_SD = new SPcPMTSD("/SPcDet/pmtSD");
			SDman->AddNewDetector(fPmt_SD);
		}
		//let pmtSD know number of pmts
		fPmt_SD->InitPMTs(fNx*fNy); 
		//-------
		//Setup Sensitive detectors

		//-------------------------------------------------------
		// Sensitive detector is not actually on the photocathode.
		// ProcessHits gets done manually by the stepping action.
		// It is used to detect when photons hit and get absorbed&detected at the
		// boundary to the photocathode (which doesnt get done by attaching it to a
		// logical volume.
		// It does however need to be attached to something or else it doesn't get
		// reset at the begining of events
		//-------------------------------------------------------

		photocathLogic->SetSensitiveDetector(fPmt_SD);


		VisAttributes();
		SurfaceProperties();
	}
	SetLogicalVolume(pmtLogic);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMT::CopyValues(){
	
	fUpdated=fConstructor->GetUpdated();
	//dimensions
	fcaloX	= fConstructor->GetemCaloWidth();
	fcaloY	= fConstructor->GetemCaloWidth();
	fNx		= fConstructor->GetNx();
	fNy		= fConstructor->GetNxy();
	fthic	= fConstructor->GetPmtThickness();

	//materials & optical properties
	fpmtMaterial 	= fConstructor->GetPmtMaterial();
	fphotocatMaterial = fConstructor->GetPhotocatMaterial();

	}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcPMT::SurfaceProperties(){

	//Photocathode surface properties
	const G4int num1 = 2;
	G4double ephoton[num1] = {2.0*eV, 3.3*eV};
	G4double photocath_EFF[num1]={1.,1.}; //Enables 'detection' of photons
	G4double photocath_ReR[num1]={1.92,1.92};
	G4double photocath_ImR[num1]={1.69,1.69};
	G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
	photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num1);
	photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num1);
	photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num1);

	G4OpticalSurface* photocath_opsurf=  new G4OpticalSurface("photocath_opsurf",glisur,polished,dielectric_metal);
	photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

	//Create logical skin surfaces
	new G4LogicalSkinSurface("photocath_surf",photocathLogic,photocath_opsurf);


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcPMT::VisAttributes(){

	G4VisAttributes* pmt_va = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
	pmtLogic->SetVisAttributes(pmt_va);

	G4VisAttributes* photocat_va = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	photocathLogic->SetVisAttributes(photocat_va);
}
