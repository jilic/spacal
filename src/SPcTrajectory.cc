/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#include "SPcTrajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Trajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4Polymarker.hh"

G4Allocator<SPcTrajectory> SPcTrajectoryAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcTrajectory::SPcTrajectory():
	G4Trajectory(),
	fDrawit(false),
	fForceNoDraw(false),
	fForceDraw(false)
{
  fParticleDefinition=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcTrajectory::SPcTrajectory(const G4Track* aTrack):
	G4Trajectory(aTrack),
	fDrawit(false)
{
  fParticleDefinition=aTrack->GetDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcTrajectory::SPcTrajectory(SPcTrajectory &right):
	G4Trajectory(right),
	fDrawit(right.fDrawit)
{
  fParticleDefinition=right.fParticleDefinition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPcTrajectory::~SPcTrajectory() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcTrajectory::DrawTrajectory() const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::DrawTrajectory();
  // ... or override with your own code here.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPcTrajectory::DrawTrajectory(G4int i_mode) const{
	//Taken from G4VTrajectory and modified to select colours based on particle
	//type and to selectively eliminate drawing of certain trajectories.

	if(!fForceDraw && (!fDrawit || fForceNoDraw))
		return;

	// If i_mode>=0, draws a trajectory as a polyline and, if i_mode!=0,
	// adds markers - yellow circles for step points and magenta squares
	// for auxiliary points, if any - whose screen size in pixels is
	// given by std::abs(i_mode)/1000.  E.g: i_mode = 5000 gives easily
	// visible markers.

	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if (!pVVisManager) return;

	const G4double markerSize = std::abs(i_mode)/1000;
	G4bool lineRequired (i_mode >= 0);
	G4bool markersRequired (markerSize > 0.);

	G4Polyline trajectoryLine;
	G4Polymarker stepPoints;
	G4Polymarker auxiliaryPoints;

	for (G4int i = 0; i < GetPointEntries() ; i++) {
		G4VTrajectoryPoint* aTrajectoryPoint = GetPoint(i);
		const std::vector<G4ThreeVector>* auxiliaries
			= aTrajectoryPoint->GetAuxiliaryPoints();
		if (auxiliaries) {
			for (size_t iAux = 0; iAux < auxiliaries->size(); ++iAux) {
				const G4ThreeVector pos((*auxiliaries)[iAux]);
				if (lineRequired) {
					trajectoryLine.push_back(pos);
				}
				if (markersRequired) {
					auxiliaryPoints.push_back(pos);
				}
			}
		}
		const G4ThreeVector pos(aTrajectoryPoint->GetPosition());
		if (lineRequired) {
			trajectoryLine.push_back(pos);
		}
		if (markersRequired) {
			stepPoints.push_back(pos);
		}
	}

	if (lineRequired) {
		G4Colour colour;

		if(fParticleDefinition==G4OpticalPhoton::OpticalPhotonDefinition()){
			colour = G4Colour(0.,1.,0.);
		}
		else //All other particles are blue
			colour = G4Colour(0.,0.,1.);

		G4VisAttributes trajectoryLineAttribs(colour);
		trajectoryLine.SetVisAttributes(&trajectoryLineAttribs);
		pVVisManager->Draw(trajectoryLine);
	}
	if (markersRequired) {
		auxiliaryPoints.SetMarkerType(G4Polymarker::squares);
		auxiliaryPoints.SetScreenSize(markerSize);
		auxiliaryPoints.SetFillStyle(G4VMarker::filled);
		G4VisAttributes auxiliaryPointsAttribs(G4Colour(0.,1.,1.));  // Magenta
		auxiliaryPoints.SetVisAttributes(&auxiliaryPointsAttribs);
		pVVisManager->Draw(auxiliaryPoints);

		stepPoints.SetMarkerType(G4Polymarker::circles);
		stepPoints.SetScreenSize(markerSize);
		stepPoints.SetFillStyle(G4VMarker::filled);
		G4VisAttributes stepPointsAttribs(G4Colour(1.,1.,0.));  // Yellow
		stepPoints.SetVisAttributes(&stepPointsAttribs);
		pVVisManager->Draw(stepPoints);
	}
}
