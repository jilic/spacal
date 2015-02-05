/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcScintHit_h
#define SPcScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class SPcScintHit : public G4VHit
{
	public:

		SPcScintHit();
		SPcScintHit(G4VPhysicalVolume* pVol);
		virtual ~SPcScintHit();
		SPcScintHit(const SPcScintHit &right);
		const SPcScintHit& operator=(const SPcScintHit &right);
		G4int operator==(const SPcScintHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void *aHit);

		virtual void Draw();
		virtual void Print();

		inline void SetEdep(G4double de) { fEdep = de; }
		inline void AddEdep(G4double de) { fEdep += de; }
		inline G4double GetEdep() { return fEdep; }

		inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
		inline G4ThreeVector GetPos() { return fPos; }

		inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

	private:
		G4double fEdep;
		G4ThreeVector fPos;
		const G4VPhysicalVolume* fPhysVol;

};

typedef G4THitsCollection<SPcScintHit> SPcScintHitsCollection;

extern G4Allocator<SPcScintHit> SPcScintHitAllocator;

inline void* SPcScintHit::operator new(size_t)
{
	void *aHit;
	aHit = (void *) SPcScintHitAllocator.MallocSingle();
	return aHit;
}

inline void SPcScintHit::operator delete(void *aHit)
{
	SPcScintHitAllocator.FreeSingle((SPcScintHit*) aHit);
}

#endif
