/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SPcPMTSD.hh"
#include "SPcScintSD.hh"
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

#include "G4SDManager.hh"
// units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "SPcCalorimeter.hh"
#include "SPcPMT.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
:ej309_mt(NULL)
{

	worldSolid=mirrorSolid =preshowerSolid=NULL;
	worldLogic=mirrorLogic=preshowerLogic=NULL;
	worldPhys =mirrorPhys =preshowerPhys =NULL;
	
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
	vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,0.1*kelvin, 1.e-19*pascal);

	air     = man->FindOrBuildMaterial("G4_AIR");
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
void DetectorConstruction::SetDefaults() 
{
	//This function defines the defaults of the geometry construction

	//---- World
	halfWorldLength = 4000*mm;

	//--- EM calo 
	emCaloWidth               = 50.0*mm;//50*mm
	emCaloLength              = 100.0*mm;
	posEmCalo = -G4ThreeVector(0,0,-emCaloLength/2);

	// ----Mirror
	halfMirrorSizeZ = 0.1*mm;
	// ----Preshower
	halfPreshowerSizeZ = 0.18*cm; //X0(W) =0.36cm

	// ----Tubes and Capillaries
	holeOutherR 		= 0.41*mm;
	capillaryOutherR	= 0.40*mm;
	coreOutherR 		= 0.20*mm;
	pitch 				= 1.4*mm;

	//bool
	fcalorimeter= true; 	
	dice5=false;
	fpresh = true;
	fmirror = true;      	
	fpmts=false;
	ffield= false;       	
	
	// ----- Magnetic Field
	fieldValueX = 0.0*tesla;
	fieldValueY = 0.0*tesla;
	fieldValueZ = 3.8*tesla;

	// -----PMTs 
	fNx = fNy =5;
	fD_mtl 	=2.0*mm; // pmt's tickness

	fUpdated=true;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct(){
	DefineMaterials();
	
	return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
	// World
	worldSolid= new G4Box("sWorld",halfWorldLength,halfWorldLength,halfWorldLength);
	worldLogic= new G4LogicalVolume( worldSolid, vacuum, "lWorld", 0, 0, 0);

	//  Must place the World Physical volume unrotated at (0,0,0).
	worldPhys= 
		new G4PVPlacement(0,     // no rotation
				G4ThreeVector(), // at (0,0,0)
				worldLogic,      // its logical volume
				"pWorld",        // its name
				0,               // its mother  volume
				false,           // no boolean operations
				0);              // copy number
	
	worldLogic->SetVisAttributes(G4VisAttributes::Invisible);

	//Place the calorimeter
	if(fcalorimeter){ new SPcCalorimeter(0,posEmCalo,worldLogic,false,0,this); }
	
	//Place the PMTs 
	if(fpmts){

		//Arrange pmts at the back of the calo block
		G4double dx =emCaloWidth/(fNx) ;
		G4double dy = emCaloWidth/(fNy);
		G4double x,y,z; 
		G4double xmin = -emCaloWidth/2.0 + dx/2.;
		G4double ymin = -emCaloWidth/2.0 + dy/2.;

		z = emCaloLength + fD_mtl/2;

		x=xmin;
		G4int k =0;
		for(G4int j=1;j<=fNx;j++){
			y=ymin;
			for(G4int i=1;i<=fNx;i++){
				new SPcPMT(0,G4ThreeVector(x,y,z),worldLogic,false,k,this);
				k++;
				y+=dy;
			}
			x+=dx;
		}
	}	

	//create the Al mirror volume
	if(fmirror){
	G4double mirrorShift = halfMirrorSizeZ;

	//make solid, logic & phys
	mirrorSolid = new G4Box("sMirror",emCaloWidth/2,emCaloWidth/2,halfMirrorSizeZ);

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
	G4VisAttributes * Yellow     = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255.   ));
	 mirrorLogic->SetVisAttributes(Yellow);
    
	 // Al Mirror surface properties
	const G4int NUM =3;
	G4double pp1[NUM] = {2.36*eV,2.91*eV, 3.28*eV};
	G4double mirr_EFF[NUM]={0.0,0.0, 0.0};
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
	
	}
	
	
	//-------create the W preshower
	//make solid, logic & phys
	if(fpresh){
		preshowerSolid = new G4Box("sPreshower",emCaloWidth/2,emCaloWidth/2,halfPreshowerSizeZ);

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
		G4VisAttributes * LightGreen = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255. ));	
	LightGreen->SetForceSolid(true);
		preshowerLogic->SetVisAttributes(LightGreen);
	}
	//------------------------------
	//Magnetic Field
	//------------------------------
	if(ffield) ConstructField(); 

	// Always return the physical World
	return worldPhys;
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
void DetectorConstruction::SetDiceType(G4bool b){
	dice5=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetMagField(G4bool b){
	ffield=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetMirror(G4bool b){
	fmirror=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetPsw(G4bool b){
	fpresh=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCalo(G4bool b){
	fcalorimeter=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetPmts(G4bool b){
	fpmts=b;
	fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetPitch(G4double p){

	if ( p < (2 *holeOutherR)){
		G4cout << "***********Proposed pitch is out of range it should be greather  two holeOutherR" <<G4endl;	
	}else{

		pitch	=p;
		fUpdated=true;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetHoleR(G4double r){

	if (r<capillaryOutherR){
		G4cout << "***********Proposed hole radius is out of range it should be < capillaryOutherR" <<G4endl;	
	}else{

		holeOutherR	=r;
		fUpdated=true;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCapillaryR(G4double r){

	if (r<coreOutherR && r>holeOutherR){
		G4cout << "***********Proposed tube radius is out of range it should be < holeOutherR and > capillaryOutherR" <<G4endl;	
	}else{

		capillaryOutherR	=r;
		fUpdated=true;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCoreR(G4double r){

	if ( r>capillaryOutherR){
		G4cout << "***********Proposed core radius is out of range it should be < capillaryOutherR " <<G4endl;	
	}else{

		coreOutherR	=r;
		fUpdated=true;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetNbPmts(G4int i){

		fNx= i;
		fNy= i;
		fUpdated=true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetNbRadLenghtsPsw(G4double d){
		G4double tungstenX0= 0.36*cm;
		halfPreshowerSizeZ= 0.5*tungstenX0*d;
		fUpdated=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::UpdateGeometry()
{

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

