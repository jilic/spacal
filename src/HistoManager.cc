/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager():
fName("test"),
	fEvent(false),
	fPP(false),
	fLost(false),
	rootFile(0),
	ntplEvt(0), 
	ntplLost(0), 
	ntplPrimP(0), 
	runID(0),
	evtID(0),
	nOptTotal(0), 
	nScint(0) ,
	nPhotoE(0), 
	nAbsorbSQ(0),
	eDep(0),
	nScintOut(0), 
	nScintOutP(0), 
	X0(0), 
	pdgID(-999),
	noPar(-1),
	eKin(-1),
	x(0),
	y(0),
	z(0),
	step(-1),
	nsec(-1),
	eKinP(-1),
	dEnP(-1),
	xP(0),
	yP(0),
	zP(0),
	sL(0),
	tL(0)
{
    
  // ntuples
  ntplEvt	= 0;
  ntplLost	= 0;
  ntplPrimP	= 0;
//histoghatms
	 for (G4int k=0; k<MaxHisto; k++) histo[k] = 0;
  fHistoMessenger = new HistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if ( rootFile ) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{ 
	// Creating a root file
	//
	G4String fileName = fName + ".root";
	rootFile = new TFile(fileName,"RECREATE");
	if(!rootFile) {
		G4cout << " HistoManager::book : problem creating the ROOT TFile "<< G4endl;
		return;
	}

	//histograms
	histo[1] = new TH1D("1", "Ek vs z", 100, -101.5*CLHEP::mm, -1.5*CLHEP::mm);
	if (!histo[1]) G4cout << "\n can't create histo 1" << G4endl;

	histo[2] = new TH1D("2", "z", 100, -101.5*CLHEP::mm, -1.5*CLHEP::mm);
	if (!histo[2]) G4cout << "\n can't create histo 2" << G4endl;

	// create Event  tree"
	if(fEvent){
		ntplEvt = new TTree("events", "Event tree");
		ntplEvt->Branch("runID",		&runID,		"runID/I");
		ntplEvt->Branch("evtID",		&evtID,		"evtID/I");
		ntplEvt->Branch("nOptTotal",	&nOptTotal,	"nOptTotal/I");
		ntplEvt->Branch("nScint",		&nScint,	"nScint/I");
		ntplEvt->Branch("nCher",		&nCher,		"nCher/I");
		ntplEvt->Branch("nPhotoE",		&nPhotoE,	"nPhotoE/I");
		ntplEvt->Branch("nAbsorbSQ", 	&nAbsorbSQ,	"nAbsorbSQ/I");
		ntplEvt->Branch("eDep", 		&eDep, 		"eDep/D");
		ntplEvt->Branch("nScintOut",		&nScintOut,		"nScintOut/I");
		ntplEvt->Branch("nScintOutP",		&nScintOutP,	"nScintOutP/I");
		ntplEvt->Branch("X0",		&X0,		"X0/D");
	}
	if(fLost){
		// create Lost  tree"
		ntplLost = new TTree("lostP", "Lost particles tree");
		ntplLost->Branch("runID",	&runID,	"runID/I");
		ntplLost->Branch("evtID",	&evtID,	"evtID/I");
		ntplLost->Branch("pdgID",	&pdgID,	"pdgID/I");
		ntplLost->Branch("noPar",	&noPar,	"noPar/I");//particle counter
		ntplLost->Branch("eKin", 	&eKin, 	"eKin/D");
		ntplLost->Branch("x", 		&x, 	"x/D");
		ntplLost->Branch("y", 		&y, 	"y/D");
		ntplLost->Branch("z", 		&z, 	"z/D");
	}
	if(fPP){
		// create PrimP  tree"
		ntplPrimP	= new TTree("primP", "Primary particle tree");
		ntplPrimP->Branch("runID",		&runID,	"runID/I");
		ntplPrimP->Branch("evtID",		&evtID,	"evtID/I");
		ntplPrimP->Branch("step",			&step,	"step/I");//step counter
		ntplPrimP->Branch("nsec",			&nsec,	"nsec/I");//nsec
		ntplPrimP->Branch("eKinP", 		&eKinP, 	"eKinP/D");
		ntplPrimP->Branch("dEnP", 		&dEnP, 	"dEnP/D");
		ntplPrimP->Branch("xP", 			&xP, 		"xP/D");
		ntplPrimP->Branch("yP", 			&yP, 		"yP/D");
		ntplPrimP->Branch("zP", 			&zP, 		"zP/D");
		ntplPrimP->Branch("sL", 			&sL, 		"sL/D");
		ntplPrimP->Branch("tL", 			&tL, 		"tL/D");
	//	ntplPrimP->Branch("vol", 			(void*)std::string, 	"vol/C", 1024);
	//	ntplPrimP->Branch("proc", 		(void*)std::string, 	"proc/C", 1024);
	}


	G4cout << "\n----> Root file is opened:  " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
  if (rootFile) {
    rootFile->Write();       //
    rootFile->Close();        // 
    G4cout << "\n----> Root file is saved \n" << G4endl;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
	if (ih >= MaxHisto) {
		G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
		<< " does not exist. (xbin=" << xbin << " weight=" << weight << ")"<< G4endl;
		return;
	}
	if  (histo[ih]) { histo[ih]->Fill(xbin, weight); }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtupleEvt(G4int run, G4int evt, G4int tot, G4int scint, G4int cher,G4int photoe, G4int abssq, G4double un, G4int scout,G4int scoutp, G4double evtx0){
	runID			= run;
	evtID			= evt;
	nOptTotal	= tot;
	nScint 		= scint;
	nCher			= cher;
	nPhotoE		= photoe;
	nAbsorbSQ	= abssq;
	eDep			= un;
	nScintOut	= scout;
	nScintOutP= scoutp;
	X0	= evtx0;

	if (ntplEvt) ntplEvt->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtupleLost(G4int run, G4int evt,  G4int pdg, G4int npar, G4double kine, G4double xc, G4double yc, G4double zc){
	runID		= run;
	evtID		= evt;
	pdgID		= pdg;
	noPar		= npar;
	eKin		= kine;
	x			= xc;
	y			= yc;
	z			= zc;

	if (ntplLost) ntplLost->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillNtuplePrimP(G4int a, G4int b,  G4int c, G4int d, G4double k, G4double de, G4double xc, G4double yc, G4double zc, G4double sl,
		G4double tl){
	runID		= a;
	evtID		= b;
	step		= c;
	nsec		= d;
	eKinP		= k;
	dEnP		= de;
	xP			= xc;
	yP			= yc;
	zP			= zc;
	sL			= sl;
	tL			= tl;

	if (ntplPrimP) ntplPrimP->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


