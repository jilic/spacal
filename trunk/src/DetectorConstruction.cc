/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcPMTSD.hh"
#include "SPcScintSD.hh"
#include "TubeParameterisation.hh"
#include "G4PVParameterised.hh"
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4ExplicitEuler.hh"
#include "G4ChordFinder.hh"
#include "G4EqMagElectricField.hh"
#include "G4PropagatorInField.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
 
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"
#include "G4PropagatorInField.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"

//#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
// units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

SPcPMTSD* DetectorConstruction::fPmt_SD=NULL;
SPcScintSD* DetectorConstruction::fScint_SD=NULL;

G4SDManager* SDman = G4SDManager::GetSDMpointer();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
:ej309_mt(NULL)
{
	air = vacuum = wolfram = quartz = ej309 = fGlass = fAl = NULL;
	
	fUpdated = false;
	
	SetDefaults();

	fDetectorMessenger = new DetectorMessenger(this);

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetDefaults() 
{
	//This function defines the defaults of the geometry construction

	//---- World
	halfWorldLength = 4000*mm;

	//--- EM calo 
	emCaloWidth               = 50.0*mm;//50*mm
	emCaloLength              = 100.0*mm;
	posEmCalo = -G4ThreeVector(0,0,-emCaloLength/2);
	//posEmCalo = -G4ThreeVector(0,0,0);

	// ----Tubes and Capillaries
	param=false;
	dice5=false;
	holeOutherR 		= 0.41*mm;
	capillaryOutherR	= 0.40*mm;
	coreOutherR 		= 0.20*mm;
	pitch 				= 1.4*mm;

	// ----Preshower
    halfPreshowerSizeZ = 0.18*cm; //X0(W) =0.36cm
	
	// ----Mirror
    halfMirrorSizeZ = 0.1*mm;
	
	// ----- Magnetic Field
	fieldValueX = 0.0*tesla;
	fieldValueY = 0.0*tesla;
	fieldValueZ = 3.8*tesla;

	// -----PMTs 
	fNx = fNy =0;
	fD_mtl 	=0.0635*cm; // pmt's tickness
	pmtZ 	= emCaloLength + fD_mtl; 
	
	fUpdated=true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructField() 
{
	static G4TransportationManager* trMgr = G4TransportationManager::GetTransportationManager(); 

	// A field object is held by a field manager
	// Find the global Field Manager
	G4FieldManager* globalFieldMgr = trMgr->GetFieldManager();

	static G4bool fieldIsInitialized = false;
	if(!fieldIsInitialized){

		G4MagneticField* myField = 0;
		G4ThreeVector  fieldVector(  fieldValueX, fieldValueY, fieldValueZ );      

		myField = new G4UniformMagField( fieldVector ); 
		//The ChordFinder is an helper class to track particles 
		//in magnetic fields, it sets the accuracy to be used.      
		globalFieldMgr->CreateChordFinder(myField);      
		globalFieldMgr->SetDetectorField(myField);

		G4cout << "Magnetic field has been constructed " << "in DetectorConstruction::ConstructField()" << G4endl;
		fieldIsInitialized = true; 
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineMaterials() 
{
	G4double a;  // atomic mass
	G4double z;  // atomic number
	G4double density;
	//Get Materials from NIST database
	G4NistManager* man = G4NistManager::Instance();
	man->SetVerbose(0);
	//Elements
	G4Element* O  = new G4Element("Oxygen",	"O",	8.,	16.00*g/mole);
	G4Element* Si = new G4Element("Silicon","Si",	14.,28.09*g/mole);
	G4Element* H = new G4Element("Hidrogen","H",	1.,	1.00794*g/mole);
	G4Element* C = new G4Element("Carbon",	"C",	6., 12.011*g/mole);

	//Build materials

	
	// -------Tungsten____________________
	wolfram   = man->FindOrBuildMaterial("G4_W");
	//Do not add optical properties to the volume (W) medium. 
	//It is a metal and does not have a pure real index of refraction.
	
	
	// -------EJ309____________________
	G4double density_ej309 = 0.959*g/cm3;
	ej309 = new G4Material("ej309", density_ej309, 2);
	ej309->AddElement(H, 0.09849); //H:C ratio = 1.25 (from www.eljentechnology.com)
	ej309->AddElement(C, 0.90151);  //H:C ratio = 1.25 (from www.eljentechnology.com)

	//Add optical properties
	const G4int enBin=11;
	G4double energy_ej309[enBin] = { 
		2.360*eV, 2.626*eV, 2.848*eV, 2.888*eV, 2.933*eV,
		3.017*eV, 3.052*eV, 3.122*eV, 3.175*eV, 3.210*eV,
		3.264*eV};// lambda range 
	G4double rindex_ej309[enBin] = { 
		1.57, 1.57, 1.57, 1.57, 1.57,
		1.57, 1.57, 1.57, 1.57, 1.57,
		1.57};          // ref index
	G4double absorp_ej309[enBin] = { 
		2.0*m, 2.0*m, 2.0*mm, 2*m, 2*m,
		2.0*m, 2.0*m, 2.0*mm, 2*m, 2*m,
		2.0*m};       // atten length www.eljentechnology.com says >1m
	G4double ScintilSlow[enBin]={ 
		0.1,0.4,0.8,0.9,1.0,
		0.8,0.7,0.6,0.4,0.2,
		0.0};
	// trick to reduce the number of optical photons tracked:
	// default values
	G4double	actSY = 11500.; // actual scintillator yield (photons/MeV) www.eljentechnology.com
	G4double	pmtEff = 0.20;  // 20% Q.E. of PMTs (PMT quantum efficiency )
	G4double	effSY = actSY*pmtEff;

	ej309_mt = new G4MaterialPropertiesTable();
	ej309_mt->AddProperty("RINDEX",				energy_ej309, rindex_ej309,	enBin);
	ej309_mt->AddProperty("ABSLENGTH", 		energy_ej309, absorp_ej309,	enBin);
	ej309_mt->AddProperty("SLOWCOMPONENT",energy_ej309, ScintilSlow,	enBin);
	ej309_mt->AddConstProperty("SCINTILLATIONYIELD", 	actSY/MeV );
	ej309_mt->AddConstProperty("SLOWTIMECONSTANT",		3.5*ns);
	ej309_mt->AddConstProperty("RESOLUTIONSCALE",			1.0);
	ej309->SetMaterialPropertiesTable(ej309_mt);

    // Set the Birks Constant for the SPc scintillator
    ej309->GetIonisation()->SetBirksConstant(0.01*mm/MeV);

	//----------Quartz (fused silica)_____________
	quartz = new G4Material("quartz", 2.203*g/cm3, 2);
	quartz->AddElement(Si, 1);
	quartz->AddElement(O , 2);

	// from http://www.crystran.co.uk/optical-materials/quartz-crystal-sio2
	const G4int enBin_quartz = 5;
	G4double energy_quartz[enBin_quartz] = {
		2.36*eV,2.48*eV,2.76*eV,3.10*eV,3.28*eV
	};

	G4double rindex_quartz[enBin_quartz] = {
		1.46,1.46,1.47,1.47,1.48 
	};
	G4double absorp_quartz[enBin_quartz] = {
		3.0*m,3.0*m,3.0*m,3.0*m,3.0*m
	};

	G4MaterialPropertiesTable *quartz_mt = new G4MaterialPropertiesTable();
	quartz_mt->AddProperty("RINDEX", 		energy_quartz, rindex_quartz, enBin_quartz);
	quartz_mt->AddProperty("ABSLENGTH", energy_quartz, absorp_quartz, enBin_quartz);
	quartz->SetMaterialPropertiesTable(quartz_mt);

	//---------Air & Vacuum________________
	air     = man->FindOrBuildMaterial("G4_AIR");
	vacuum  = man->FindOrBuildMaterial("G4_Galactic");
	const G4int enBin_v=3;
	G4double vacuum_Energy[enBin_v]={2.36*eV,2.91*eV,3.28*eV};
	G4double vacuum_RIND[enBin_v]={1.,1.,1.};  
	
	G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
	vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,enBin_v);
	vacuum->SetMaterialPropertiesTable(vacuum_mt);
	air->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex

	//---------Glass & Aluminum________________
	G4double sc_Energy[enBin_v]    = { 2.3*eV , 3.0*eV, 3.3*eV };

	//Glass
	fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
	fGlass->AddElement(C,91.533*perCent);
	fGlass->AddElement(H,8.467*perCent);

	G4double glass_RIND[enBin_v]={1.49,1.49,1.49};
	G4double glass_AbsLength[enBin_v]={420.*cm,420.*cm,420.*cm};
	G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
	glass_mt->AddProperty("ABSLENGTH",sc_Energy,glass_AbsLength,enBin_v);
	glass_mt->AddProperty("RINDEX",sc_Energy,glass_RIND,enBin_v);
	fGlass->SetMaterialPropertiesTable(glass_mt);

	//Aluminum
	fAl = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);
	//Do not add optical properties to the volume (Al) medium. 
	//It is a metal and does not have a pure real index of refraction.


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct(){
	DefineMaterials();
	return ConstructDetector();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
	//------------------------------
	// World
	//------------------------------
	
	//G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2.*halfWorldLength);
	//G4cout << "Computed tolerance = "<< G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm << " mm" << G4endl;

	G4Box * worldSolid= new G4Box("sWorld",halfWorldLength,halfWorldLength,halfWorldLength);
	worldLogic= new G4LogicalVolume( worldSolid, vacuum, "lWorld", 0, 0, 0);

	//  Must place the World Physical volume unrotated at (0,0,0).
	G4VPhysicalVolume * worldPhys= 
		new G4PVPlacement(0,     // no rotation
				G4ThreeVector(), // at (0,0,0)
				worldLogic,      // its logical volume
				"pWorld",        // its name
				0,               // its mother  volume
				false,           // no boolean operations
				0);              // copy number

	//------------------------------
	//EM calorimeter
	//------------------------------
	ConstructEMCalo();

	//------------------------------
	//PMTs
	//------------------------------
	ConstructPMTs();

	//------------------------------
	//Magnetic Field
	//------------------------------
	ConstructField(); 

	// Visualization attributes
	SetVisibilityAttributes();

	// Always return the physical World
	return worldPhys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::ConstructEMCalo()
{

	/*
	 * create a Wolfram block
	 * place the Quartz tubes
	 * in each tube place a scint. core
	 * define optical boundaries
	 */

	//-----------------------------------------------------------------------
	//-------create the Al mirror volume
	//-----------------------------------------------------------------------
	//G4double mirrorShift = emCaloLength + halfMirrorSizeZ;
	G4double mirrorShift = halfMirrorSizeZ;
	
	G4double halfEmCaloSizeX = emCaloWidth/2;
	G4double halfEmCaloSizeY = emCaloWidth/2;
	G4double halfEmCaloSizeZ = emCaloLength/2;

	//make solid, logic & phys
	G4Box* mirrorSolid = new G4Box("sMirror",halfEmCaloSizeX,halfEmCaloSizeY,halfMirrorSizeZ);

	mirrorLogic = new G4LogicalVolume( mirrorSolid,//its solid
			fAl, //material
			"lMirror");//its name

   G4ThreeVector posMirror = -G4ThreeVector(0,0,mirrorShift);
	
	mirrorPhys = new G4PVPlacement(0, //no rotation
			posMirror,	//translation
			mirrorLogic, 			//logical volume
			"pMirror",				//name
			worldLogic,				//mother volume
			false, 					//no boolean operation
			0); 					//copy number

	//-----------------------------------------------------------------------
	//-------create the W preshower
	//-----------------------------------------------------------------------
	
	//make solid, logic & phys
	G4Box* preshowerSolid = new G4Box("sPreshower",halfEmCaloSizeX,halfEmCaloSizeY,halfPreshowerSizeZ);

	preshowerLogic = new G4LogicalVolume( preshowerSolid,//its solid
			vacuum, // wolfram //material
			"lPreshower");//its name

	G4double preshowerShift = 2*halfMirrorSizeZ + halfPreshowerSizeZ;
   	G4ThreeVector posPreshower = -G4ThreeVector(0,0,preshowerShift);
	
	preshowerPhys = new G4PVPlacement(0, //no rotation
			posPreshower,		//translation
			preshowerLogic, 	//logical volume
			"pPreshower",		//name
			worldLogic,				//mother volume
			false, 					//no boolean operation
			0); 					//copy number


	//-----------------------------------------------------------------------
	//-------create the W block volume
	//-----------------------------------------------------------------------

	//make solid, logic & phys
	G4Box* caloSolid = new G4Box("sCalo",halfEmCaloSizeX,halfEmCaloSizeY,halfEmCaloSizeZ);

	caloLogic = new G4LogicalVolume( caloSolid,//its solid
			wolfram, //material
			"lCalo");//its name

	caloPhys = new G4PVPlacement(0, //no rotation
			posEmCalo,	//translation
			caloLogic, 	//logical volume
			"pCalo",	//name
			worldLogic,	//mother volume
			false, 		//no boolean operation
			0); 		//copy number

	//inside Wolfram do not follow electrons with energy below 511 keV
	//caloLogic->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,511*keV));

	//-----------------------------------------------------------------------
	//-------create & place tubes (capillary+core)
	//-----------------------------------------------------------------------
	
	
	if(!param) {
		ConstructTubes();
	}else {
		ConstructTubesPARAM();
	}


	//----------------------------------------------------------
	//				OPTICAL BOUNDARIES
	//----------------------------------------------------------

	//1) Tungsten surface property 

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
	

    //2) Al Mirror surface properties
	
	G4double mirr_EFF[NUM]={0.,0., 0.0};
	G4double mirr_ReR[NUM]={0.88, 0.54, 0.41};
	G4double mirr_ImR[NUM]={6.14, 5.02, 4.44};
	
	G4MaterialPropertiesTable* mirr_mt = new G4MaterialPropertiesTable();
	mirr_mt->AddProperty("EFFICIENCY",pp1,mirr_EFF,NUM);
	mirr_mt->AddProperty("REALRINDEX",pp1,mirr_ReR,NUM);
	mirr_mt->AddProperty("IMAGINARYRINDEX",pp1,mirr_ImR,NUM);

	G4OpticalSurface* mirr_opsurf=  new G4OpticalSurface("mirr_opsurf",glisur,polished,dielectric_metal);
	mirr_opsurf->SetMaterialPropertiesTable(mirr_mt);

	//Create logical skin surfaces
	new G4LogicalSkinSurface("mirror_surf",mirrorLogic,mirr_opsurf);

	//3) all other optBoundaries are dielectric_dieletric  -polished. I do not need to
	// explicitely define any of them - G4 will take care of it. 

	return caloPhys;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetDiceType(G4bool b){
	dice5=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPMTzDistance(G4double z){
	pmtZ = z;
	fUpdated=true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::UpdateGeometry(){

	// clean-up previous geometry
	G4GeometryManager::GetInstance()->OpenGeometry();

	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalSkinSurface::CleanSurfaceTable();
	G4LogicalBorderSurface::CleanSurfaceTable();
	G4SurfaceProperty::CleanSurfacePropertyTable();

	//define the new one
	G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
	G4RunManager::GetRunManager()->GeometryHasBeenModified();

	fUpdated=false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructTubesPARAM()
{
	G4double innerR = 0.0*mm;
	G4double halfEmCaloSizeZ = emCaloLength/2;

	//Setup Sensitive Detectors***************
	//Created here so it exists as tubes are being placed
	if(!fScint_SD){//determine if it has already been created
		fScint_SD = new SPcScintSD("/SPcDet/scintSD");
		SDman->AddNewDetector(fScint_SD);
	}

	//Air hole 
	
	//does not matter what is the value of poluprecnik; it will be recalculated in parametrisaton
	G4double poluprecnik = 0.5 *mm;//0.41 

	G4Tubs* holeSolid = new G4Tubs("sHole",
			innerR,         //inner r  
			poluprecnik, 	//outher r
			halfEmCaloSizeZ,//length
			0,              //start angle
			CLHEP::twopi);  //end angle 
	holeLogic = new G4LogicalVolume(holeSolid, air, "lHole");
	//------------------------------------------------ 
	//Place air holes using parametrisation
	//------------------------------------------------ 
	int noHoles(0);
	int brX = int(emCaloWidth/pitch);
	if( brX%2 ) noHoles= (brX - 1)*(brX - 1);
	else noHoles = brX*brX  ;
	std::cout<<"pitch = "<<pitch << "  capillaryOutherR = "<< capillaryOutherR <<std::endl;	
	std::cout<<"noHoles = "<<noHoles<<std::endl;	

	G4VPVParameterisation* tubeParam = new TubeParameterisation(
			pitch,
			emCaloWidth,
			emCaloLength,
			holeOutherR,
			dice5
			);


	holePhys = new G4PVParameterised(
			"pHole",    //name
			holeLogic,  //their logic
			caloLogic,  //mother logic
			kYAxis,	    //how they are placed
			noHoles,    //total number
			tubeParam, 	//the parametrisation
			false);		//check for overlaping

	//------------------------------------------------------

	//1) Capillary - quartz
	G4Tubs* capillarySolid = new G4Tubs("sCapillary",
			innerR,		        //inner r  
			capillaryOutherR,	//outher r
			halfEmCaloSizeZ,	//length
			0,					//start angle
			CLHEP::twopi);		//end angle	
	capillaryLogic = new G4LogicalVolume(capillarySolid, quartz, "lCapillary");

	//Place the capilary inside the hole
	G4ThreeVector position(0, 0, 0);
	corePhys = new G4PVPlacement( 0 , 
			position, 
			capillaryLogic ,//logic
			"pCapillary",   //name 
			holeLogic,	    //mother
			false, 
			0);

	//2 ej309 core 
	G4Tubs* coreSolid = new G4Tubs("sCore",
			innerR,			    //inner r 
			coreOutherR,	  //outher r
			halfEmCaloSizeZ,//length
			0,				      //start angle
			CLHEP::twopi);	//end angle	

	coreLogic = new G4LogicalVolume(coreSolid, ej309, "lCore");

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



}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructTubes()
{
	//dimensions
	G4double innerR = 0.0*mm;
	G4double halfEmCaloSizeZ = emCaloLength/2;

	//Setup Sensitive Detectors***************
	//Created here so it exists as tubes are being placed
	if(!fScint_SD){//determine if it has already been created
		fScint_SD = new SPcScintSD("/SPcDet/scintSD");
		SDman->AddNewDetector(fScint_SD);
	}

	//Air hole 
	G4double poluprecnik = 0.41 *mm; 
	G4Tubs* holeSolid = new G4Tubs("sHole",
			innerR,         //inner r  
			poluprecnik, 	//outher r
			halfEmCaloSizeZ,//length
			0,              //start angle
			CLHEP::twopi);  //end angle 
	holeLogic = new G4LogicalVolume(holeSolid, air, "lHole");

	PlaceTubes();



	//1) Capillary - quartz
	G4Tubs* capillarySolid = new G4Tubs("sCapillary",
			innerR,		        //inner r  
			capillaryOutherR,	//outher r
			halfEmCaloSizeZ,	//length
			0,					//start angle
			CLHEP::twopi);		//end angle	
	capillaryLogic = new G4LogicalVolume(capillarySolid, quartz, "lCapillary");

	//Place the capilary inside the hole
	G4ThreeVector position(0, 0, 0);
	corePhys = new G4PVPlacement( 0 , 
			position, 
			capillaryLogic ,//logic
			"pCapillary",   //name 
			holeLogic,	    //mother
			false, 
			0);

	//2 ej309 core 
	G4Tubs* coreSolid = new G4Tubs("sCore",
			innerR,			    //inner r 
			coreOutherR,	  //outher r
			halfEmCaloSizeZ,//length
			0,				      //start angle
			CLHEP::twopi);	//end angle	

	coreLogic = new G4LogicalVolume(coreSolid, ej309, "lCore");

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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::PlaceTubes(){

	int noCapillaries(0); 
	double shift(0); 
	double firstPoint(0); 
	if( int(emCaloWidth/pitch)%2 ) {

		noCapillaries= int(emCaloWidth/pitch)-1;
		shift =0.5*pitch*( (emCaloWidth/pitch) - int(emCaloWidth/pitch) ) ;
		firstPoint = shift +pitch;
	} else {

		noCapillaries = int(emCaloWidth/pitch);
		shift =0.5*pitch*( (emCaloWidth/pitch) - int(emCaloWidth/pitch) ) ;
		firstPoint = shift +pitch/2.0;
	}
	std::cout<<"PITCH = "<<pitch<<std::endl;
	std::cout<<"noCapillaries = "<<noCapillaries<<std::endl;


	int counter(0);

	if(!dice5){
		G4double positionX 	 = -emCaloWidth/2;
		for(int capNoX = 0; capNoX<noCapillaries; capNoX++){

			if (capNoX ==0) positionX = positionX + firstPoint;
			else positionX = positionX + pitch;

			G4double positionY 	 = -emCaloWidth/2;
			for(int capNoY = 0; capNoY<noCapillaries; ++capNoY){
				counter++;

				if (capNoY ==0) positionY = positionY + firstPoint;
				else positionY = positionY + pitch;

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


	if(dice5){
		G4double positionY 	 = -emCaloWidth/2;

		for(int capNoY = 0; capNoY<noCapillaries; capNoY++){

			if (capNoY ==0) positionY = positionY + firstPoint;
			else positionY = positionY + pitch;

			G4double positionX =  -emCaloWidth/2 ;
			for(int capNoX = 0; capNoX<noCapillaries ; ++capNoX){

				counter++;
				
				if (capNoY%2 ==0) {

					if(capNoX == 0)	positionX = positionX+ firstPoint; 
					else positionX = positionX + pitch;
				}else{

					if(capNoX == 0)	positionX = positionX + firstPoint +pitch/2.0; 
					else {
						if (capNoX==noCapillaries -1) break; 
						else positionX = positionX + pitch;

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
void DetectorConstruction::ConstructPMTs()
{
	// Build PMTs
	G4double halfSide;
	if(fNx==0){
		halfSide = emCaloWidth/2; //any value different from 0 will work. 
	}else {
		halfSide = emCaloWidth/(fNx*2);
	}
	G4double height_pmt = fD_mtl/2.; //this is actually half height

	pmtSolid = new G4Box("sPmt",halfSide,halfSide,height_pmt);
	pmtLogic = new G4LogicalVolume(pmtSolid,
			G4Material::GetMaterial("Glass"),
			"lPmt");

	// The "photocathode" is a metal slab at the back of the glass that
	// is only a very rough approximation of the real thing since it only
	// absorbs or detects the photons based on the efficiency set below
	photocathSolid = new G4Box("sPhotocath", halfSide, halfSide, height_pmt/2);
	photocathLogic = new G4LogicalVolume(photocathSolid,
			G4Material::GetMaterial("Al"),
			"lPhotocath");

	//place photocath inside pmt
	new G4PVPlacement(0,						//no rotation
			G4ThreeVector(0,0,height_pmt/2),	//translation
			photocathLogic,						//it's log volume
			"pPhotocath",						//it's name
			pmtLogic,							// mother volume
			false,								//
			0);									// copy no


	//Arrange pmts
	//pmt sensitive detector
	if(!fPmt_SD){
		fPmt_SD = new SPcPMTSD("/SPcDet/pmtSD");
		SDman->AddNewDetector(fPmt_SD);
		//Created here so it exists as pmts are being placed
	}
	fPmt_SD->InitPMTs(fNx*fNy); //let pmtSD know # of pmts
	//-------

	G4double dx =(halfSide)*2 ;
	G4double dy = (halfSide)*2;
	G4double x,y,z; 
	G4double xmin = -emCaloWidth/2.0 + dx/2.;
	G4double ymin = -emCaloWidth/2.0 + dy/2.;

	z = pmtZ;

	x=xmin;
	G4int k =0;
	for(G4int j=1;j<=fNx;j++){
		y=ymin;
		for(G4int i=1;i<=fNx;i++){
			new G4PVPlacement(0,			//no rotation
					G4ThreeVector(x,y,z),	//translation
					pmtLogic,				//it's log volume
					"pPmt",					//it's name
					worldLogic,				// mother volume
					false,					//
					k);						// copy no
			k++;
			y+=dy;
		}
		x+=dx;
	}

	//-------------------------------------------------------
	// Sensitive detector is not actually on the photocathode.
	// ProcessHits gets done manually by the stepping action.
	// It is used to detect when photons hit and get absorbed&detected at the
	// boundary to the photocathode (which doesnt get done by attaching it to a
	// logical volume.
	// It does however need to be attached to something or else it doesnt get
	// reset at the begining of events
	//-------------------------------------------------------

	photocathLogic->SetSensitiveDetector(fPmt_SD);


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
void DetectorConstruction::SetVisibilityAttributes()
{
	G4Colour  white   (1.0, 1.0, 1.0) ;  // white
	G4Colour  ogray    (0.5, 0.5, 0.5) ;  // gray
	G4Colour  darkgray (0.75, 0.75, 0.75) ;  // dark gray
	G4Colour  black   (0.0, 0.0, 0.0) ;  // black
	G4Colour  red     (1.0, 0.0, 0.0) ;  // red
	G4Colour  green   (0.0, 1.0, 0.0) ;  // green
	G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
	G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
	G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta
	G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow
	G4Colour  medyellow  (0.75, 0.75, 0.0) ;  // medium yellow


	// Make invisible
	inVisAtt = new G4VisAttributes(black); 
	//inVisAtt->SetVisibility(false);

	// White
	whiteVisAtt = new G4VisAttributes(white);
	//whiteVisAtt->SetVisibility(true);
	//whiteVisAtt->SetForceSolid(true);

	// Dark Grey
	dkgreyVisAtt = new G4VisAttributes(darkgray);
	//dkgreyVisAtt->SetVisibility(true);
	//dkgreyVisAtt->SetForceSolid(true);

	// Medium Yellow
	medyelVisAtt = new G4VisAttributes(medyellow);
	//medyelVisAtt->SetVisibility(true);
	//medyelVisAtt->SetForceSolid(true);

	// Yellow
	yellowVisAtt = new G4VisAttributes(yellow);
	//yellowVisAtt->SetVisibility(true);
	//yellowVisAtt->SetForceSolid(true);

	// Red
	redVisAtt = new G4VisAttributes(red);
	//redVisAtt->SetVisibility(true);
	//redVisAtt->SetForceSolid(true);
	
	//Green
	greenVisAtt = new G4VisAttributes(green);
	blueVisAtt = new G4VisAttributes(blue);

	// ogray
	ograyVisAtt = new G4VisAttributes(ogray);
	//whiteVisAtt->SetVisibility(true);
	ograyVisAtt->SetForceSolid(true);
	
	//---------------
	//asign vis atributes to the logical volumes
	//---------------

	//world
	worldLogic -> SetVisAttributes(whiteVisAtt);
	worldLogic -> SetVisAttributes(G4VisAttributes::Invisible);

	//W block
	caloLogic->SetVisAttributes(yellowVisAtt);

	//capillaries
	capillaryLogic->SetVisAttributes(redVisAtt);

	//core
	coreLogic->SetVisAttributes(greenVisAtt);
   

	//preshower
	preshowerLogic->SetVisAttributes(greenVisAtt);

	//photocathLogic->SetVisAttributes(greenVisAtt);
	photocathLogic->SetVisAttributes(ograyVisAtt);
	
	pmtLogic->SetVisAttributes(redVisAtt);
    

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

