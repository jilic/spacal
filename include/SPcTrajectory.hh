/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcTrajectory_h
#define SPcTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class SPcTrajectory : public G4Trajectory
{
	public:

		SPcTrajectory();
		SPcTrajectory(const G4Track* aTrack);
		SPcTrajectory(SPcTrajectory &);
		virtual ~SPcTrajectory();

		virtual void DrawTrajectory() const;
		virtual void DrawTrajectory(G4int i_mode=0) const;

		inline void* operator new(size_t);
		inline void  operator delete(void*);

		void SetDrawTrajectory(G4bool b){fDrawit=b;}
		void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
		void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

	private:

		G4bool fDrawit;
		G4bool fForceNoDraw;
		G4bool fForceDraw;
		G4ParticleDefinition* fParticleDefinition;
};

extern G4Allocator<SPcTrajectory> SPcTrajectoryAllocator;

inline void* SPcTrajectory::operator new(size_t)
{
	void* aTrajectory;
	aTrajectory = (void*)SPcTrajectoryAllocator.MallocSingle();
	return aTrajectory;
}

inline void SPcTrajectory::operator delete(void* aTrajectory)
{
	SPcTrajectoryAllocator.FreeSingle((SPcTrajectory*)aTrajectory);
}

#endif
