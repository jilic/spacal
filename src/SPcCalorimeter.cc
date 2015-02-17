/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   February 2015
*/

#include "SPcCalorimeter.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "SPcScintSD.hh"
#include "G4SystemOfUnits.hh"

SPcScintSD* SPcCalorimeter::fScint_SD=NULL;
G4LogicalVolume* SPcCalorimeter::caloLogic=NULL;

//G4SDManager* SDman = G4SDManager::GetSDMpointer();
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcCalorimeter::SPcCalorimeter(G4RotationMatrix *pRot,const G4ThreeVector &tlate,G4LogicalVolume *pMotherLogical,G4bool pMany,G4int pCopyNo,
		DetectorConstruction* c):
	G4PVPlacement(pRot,
		tlate,
		//Temp logical volume must be created here
		new G4LogicalVolume(new G4Box("temp",1,1,1),G4Material::GetMaterial("Vacuum"),"temp",0,0,0),
		"pCalo",
		pMotherLogical,
		pMany,
		pCopyNo),
	fConstructor(c)
{
	CopyValues();

	if(!caloLogic || fUpdated){

		/*  
		 *  create calo block
		 *  create and place tubes and capillaries
		 *  set sensitive detectors
		 *
		 */

		//-------create the W block volume
		caloSolid = new G4Box("sCalo",fcaloX/2,fcaloY/2,fcaloZ/2);

		caloLogic = new G4LogicalVolume( caloSolid, caloMaterial, "lCalo");

		//-------create & place tubes (capillary+core)
		//Setup Sensitive Detectors
		//Created here so it exists as tubes are being placed
		
		G4SDManager* SDman = G4SDManager::GetSDMpointer();
		if(fScint_SD){
	    fScint_SD=NULL;	
		}	
		if(!fScint_SD){//determine if it has already been created
			fScint_SD = new SPcScintSD("/SPcDet/scintSD");
			SDman->AddNewDetector(fScint_SD);
		}
		G4double innerR = 0.0*mm;
		//Air hole 
		holeSolid = new G4Tubs("sHole",
				innerR,         //inner r  
				fholeR, 		//outher r
				fcaloZ/2,	//length
				0,              //start angle
				CLHEP::twopi);  //end angle 
		holeLogic = new G4LogicalVolume(holeSolid, holeMaterial, "lHole");

		PlaceTubes();

		//Capillary - quartz
		capillarySolid = new G4Tubs("sCapillary",
				innerR,		    //inner r  
				fcapillR,		//outher r
				fcaloZ/2,		//length
				0,				//start angle
				CLHEP::twopi);	//end angle	
		capillaryLogic = new G4LogicalVolume(capillarySolid, tubeMaterial, "lCapillary");

		//Place the capilary inside the hole
		G4ThreeVector position(0, 0, 0);
		corePhys = new G4PVPlacement( 0 , 
				position, 
				capillaryLogic ,//logic
				"pCapillary",   //name 
				holeLogic,	    //mother
				false, 
				0);

		//ej309 core 
		coreSolid = new G4Tubs("sCore",
				innerR,		//inner r 
				fcoreR,		//outher r
				fcaloZ/2,	//length
				0,			//start angle
				CLHEP::twopi);	//end angle	

		coreLogic = new G4LogicalVolume(coreSolid, scintMaterial, "lCore");

		//Place the core inside the capillary
		corePhys = new G4PVPlacement( 0 , 
				position, 
				coreLogic ,   //logic
				"pCore", 	  //name 
				capillaryLogic,	//mother
				false, 
				0);

		//the core is sensitive detector
		coreLogic->SetSensitiveDetector(fScint_SD);


		VisAttributes();
		SurfaceProperties();
	}

	SetLogicalVolume(caloLogic);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcCalorimeter::CopyValues(){

	fUpdated=fConstructor->GetUpdated();
	//dimensions
	fcaloX	= fConstructor->GetemCaloWidth();
	fcaloY	= fConstructor->GetemCaloWidth();
	fcaloZ	= fConstructor->GetemCaloLength();
	fholeR	= fConstructor->GetHoleR();
	fcapillR= fConstructor->GetCapillR();
	fcoreR	= fConstructor->GetCoreR();
	fspacing= fConstructor->GetPitch();
	fdice5	= fConstructor->GetDice();

	//materials
	caloMaterial  = fConstructor->GetCaloMaterial();
	tubeMaterial  = fConstructor->GetTubeMaterial();
	scintMaterial = fConstructor->GetScintMaterial();
	holeMaterial  = fConstructor->GetHoleMaterial();

	//calculate additional information

	//number of tubes/holes/capillaries
	int brX = int(fcaloX/fspacing);
	if( brX%2 ) {
		fnoHoles= (brX - 1);
		fshift =0.5*fspacing*( fcaloX/fspacing - brX ) ;
		ffirstPoint = fshift + fspacing;
	}
	else {
		fnoHoles = brX ;
		fshift =0.5*fspacing*( fcaloX/fspacing - brX ) ;
		ffirstPoint = fshift + fspacing/2.0;

	}


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcCalorimeter::PlaceTubes(){


	std::cout<<"Number of holes = "<<fnoHoles <<"x"<< fnoHoles<<std::endl;

	int counter(0);

	if(!fdice5){
		G4double positionX 	 = -fcaloX/2;
		for(int capNoX = 0; capNoX<fnoHoles; capNoX++){

			if (capNoX ==0) positionX = positionX + ffirstPoint;
			else positionX = positionX + fspacing;

			G4double positionY 	 = -fcaloY/2;
			for(int capNoY = 0; capNoY<fnoHoles; ++capNoY){
				counter++;

				if (capNoY ==0) positionY = positionY + ffirstPoint;
				else positionY = positionY + fspacing;

				G4double positionZ = 0*mm;

				G4ThreeVector position(positionX, positionY, positionZ);
				holePhys = new G4PVPlacement( 0, 
						position, 
						holeLogic, 
						"pHole", 
						caloLogic,
						false, 
						counter);
			}//Y
		}//X
	}//dice


	if(fdice5){
		G4double positionY 	 = -fcaloY/2;

		for(int capNoY = 0; capNoY<fnoHoles; capNoY++){

			if (capNoY ==0) positionY = positionY + ffirstPoint;
			else positionY = positionY + fspacing;

			G4double positionX =  -fcaloY/2 ;
			for(int capNoX = 0; capNoX<fnoHoles ; ++capNoX){

				counter++;

				if (capNoY%2 ==0) {

					if(capNoX == 0)	positionX = positionX+ ffirstPoint; 
					else positionX = positionX + fspacing;
				}else{

					if(capNoX == 0)	positionX = positionX + ffirstPoint +fspacing/2.0; 
					else {
						if (capNoX==fnoHoles -1) break; 
						else positionX = positionX + fspacing;

					}

				}

				G4double positionZ = 0*mm;

				G4ThreeVector position(positionX, positionY, positionZ);
				holePhys = new G4PVPlacement( 0 , 
						position, 
						holeLogic , 
						"phole", 
						caloLogic ,
						false, 
						counter);

			}//X
		}//Y
	}//dice
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SPcCalorimeter::SurfaceProperties(){


	//Tungsten surface property 
	const G4int NUM =3;
	G4double pp1[NUM] = {2.36*eV,2.91*eV, 3.28*eV};
	G4double efficiency1[NUM] = {0.0, 0.0, 0.0}; //
	G4double rind_R[NUM]={3.45, 3.28, 3.21};
	G4double rind_C[NUM]={2.73, 2.57, 2.52};

	G4MaterialPropertiesTable* wolfram_mt  = new G4MaterialPropertiesTable();
	wolfram_mt->AddProperty("EFFICIENCY",	  pp1,	efficiency1,NUM);
	wolfram_mt->AddProperty("REALRINDEX", 	  pp1,	rind_R,	    NUM);
	wolfram_mt->AddProperty("IMAGINARYRINDEX",pp1,	rind_C,	    NUM);

	G4OpticalSurface * wolframSurface = new G4OpticalSurface("wolframOptSurf", glisur,polished,dielectric_metal);
	wolframSurface->SetMaterialPropertiesTable(wolfram_mt);

	//Create logical skin surfaces
	new G4LogicalSkinSurface("wolfram_surf",caloLogic,wolframSurface);


	// all other optBoundaries are dielectric_dieletric  -polished. I do not need to
	// explicitely define any of them - G4 will take care of it. 

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcCalorimeter::VisAttributes(){

	G4VisAttributes* calo_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
	caloLogic->SetVisAttributes(calo_va);

	G4VisAttributes* capill_va = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	capillaryLogic->SetVisAttributes(capill_va);

	G4VisAttributes* core_va = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	coreLogic->SetVisAttributes(core_va);

}
