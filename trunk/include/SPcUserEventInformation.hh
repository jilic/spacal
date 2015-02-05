/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#ifndef SPcUserEventInformation_h
#define SPcUserEventInformation_h 1

class SPcUserEventInformation : public G4VUserEventInformation
{
	public:

		SPcUserEventInformation();
		virtual ~SPcUserEventInformation();

		inline virtual void Print()const{};

		void IncPhotonCount_Scint(){fPhotonCount_Scint++;}
		void IncPhotonCount_Ceren(){fPhotonCount_Ceren++;}
		void IncExitCount(){fExitCount++;}

		void IncEDep(G4double dep){fTotE+=dep;}
		void IncAbsorption(){fAbsorptionCount++;}
		void IncBoundaryAbsorption(){fBoundaryAbsorptionCount++;}
		void IncHitCount(G4int i=1){fHitCount+=i;}
		
		void IncScintExit(){fExitCount++;}
		void IncPosScintExit(){fPosExitCount++;}
		
		
		void SetEWeightPos(const G4ThreeVector& p){fEWeightPos=p;}
		void SetReconPos(const G4ThreeVector& p){fReconPos=p;}
		void SetConvPos(const G4ThreeVector& p){fConvPos=p;fConvPosSet=true;}
		void SetPosMax(const G4ThreeVector& p,G4double edep){fPosMax=p;fEdepMax=edep;}
		void SetEvtID(const G4int id){fEvt = id;}
		void SetFound(const G4bool ff){fFound = ff;}
		void X0measurement(const G4double en){fX0 = en;}

		G4int GetPhotonCount_Scint()const {return fPhotonCount_Scint;}
		G4int GetPhotonCount_Ceren()const {return fPhotonCount_Ceren;}
		
		G4int GetScintExit()const {return fExitCount;}
		G4int GetPosScintExit()const {return fPosExitCount;}
		
		G4int GetHitCount()const {return fHitCount;}
		G4double GetEDep()const {return fTotE;}
		G4int GetAbsorptionCount()const {return fAbsorptionCount;}
		G4int GetBoundaryAbsorptionCount() const {return fBoundaryAbsorptionCount;}

		G4ThreeVector GetEWeightPos(){return fEWeightPos;}
		G4ThreeVector GetReconPos(){return fReconPos;}
		G4ThreeVector GetConvPos(){return fConvPos;}
		G4ThreeVector GetPosMax(){return fPosMax;}
		G4double GetEDepMax(){return fEdepMax;}
		G4double IsConvPosSet(){return fConvPosSet;}
		G4int getEvtID(){return fEvt;}

		G4bool isFound()const {return fFound;}
		G4double GetX0()const {return fX0;}

		//Gets the total photon count produced
		G4int GetPhotonCount(){return fPhotonCount_Scint+fPhotonCount_Ceren;}

		void IncPMTSAboveThreshold(){fPMTsAboveThreshold++;}
		G4int GetPMTSAboveThreshold(){return fPMTsAboveThreshold;}

	private:

		G4int fHitCount;
		G4int fPhotonCount_Scint;
		G4int fPhotonCount_Ceren;
		G4int fExitCount; 
		G4int fPosExitCount; 
		G4int fAbsorptionCount;
		G4int fBoundaryAbsorptionCount;

		G4double fTotE;

		//These only have meaning if totE > 0
		//If totE = 0 then these wont be set by EndOfEventAction
		G4ThreeVector fEWeightPos;
		G4ThreeVector fReconPos; //Also relies on hitCount>0
		G4ThreeVector fConvPos;//true (initial) converstion position
		G4bool fConvPosSet;
		G4ThreeVector fPosMax;
		G4double fEdepMax;

		G4int fPMTsAboveThreshold;
		G4int fEvt;
		G4bool fFound;
		G4double fX0;


};

#endif
