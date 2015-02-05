/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "globals.hh"
#include "HistoMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 class TFile;
 class TTree;
 class TH1D;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const G4int MaxHisto = 3;
class HistoManager
{
	public:

		HistoManager();
		~HistoManager();

		void book();
		void save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
		void FillNtupleEvt(G4int, G4int, G4int, G4int, G4int,G4int,G4int, G4double, G4int, G4int,G4double);
		void FillNtupleLost(G4int, G4int, G4int, G4int, G4double, G4double, G4double, G4double);
		void FillNtuplePrimP(G4int, G4int, G4int, G4int, G4double,G4double, G4double, G4double, G4double, G4double, G4double);
		
		void PrintStatistic();

		void SetRootName(G4String name){fName = name;}
		void SetEventTree(G4bool b ){fEvent = b;}
		void SetPPTree(G4bool b){fPP = b;}
		void SetLostTree(G4bool b){fLost = b;}
	
		G4bool IsEventAna(){return fEvent;}
		G4bool IsPPAna(){return fPP;}
		G4bool IsLostAna(){return fLost;}
	private:

		HistoMessenger* fHistoMessenger;
		
		G4String fName;
		G4bool fEvent;
		G4bool fPP;
		G4bool fLost;
		
		TFile*   rootFile;
		TH1D*    histo[MaxHisto];
		TTree*   ntplEvt;    
		TTree*   ntplLost;    
		TTree*   ntplPrimP;    

		G4int runID;
		G4int evtID;
		
		//event
		G4int nOptTotal;
		G4int nScint;
		G4int nCher;
		G4int nPhotoE;
		G4int nAbsorbSQ;
		G4double eDep;
		G4int nScintOut;
		G4int nScintOutP;
		G4double X0;
		
		//lost
		G4int pdgID;
		G4int noPar; //particle counter
		G4double eKin;
		G4double x;
		G4double y;
		G4double z;

		//PrimP
		G4int step;
		G4int nsec;
		G4double eKinP;
		G4double dEnP;
		G4double xP;
		G4double yP;
		G4double zP;
		G4double sL;
		G4double tL;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

