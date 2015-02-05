/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcUserEventInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcUserEventInformation::SPcUserEventInformation():
	fHitCount(0),
	fPhotonCount_Scint(0),
	fPhotonCount_Ceren(0),
	fExitCount(0),
	fPosExitCount(0),
	fAbsorptionCount(0),
  fBoundaryAbsorptionCount(0),
	fTotE(0.),
	fEWeightPos(0.),
	fPosMax(0.),
	fEdepMax(0.),
	fPMTsAboveThreshold(0), 
	fEvt(0),
	fFound(false),
	fX0(-99999)

{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcUserEventInformation::~SPcUserEventInformation() {}
