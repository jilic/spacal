/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/

#ifndef TubeParameterisation_H
#define TubeParameterisation_H 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Tubs;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Torus;
class G4Para;
class G4Hype;
class G4Box;
class G4Polycone;
class G4Polyhedra;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TubeParameterisation : public G4VPVParameterisation
{ 
	public:
		TubeParameterisation(G4double pitch, G4double sirina, G4double duzina , G4double radius, G4bool dice5);

		virtual                                 
			~TubeParameterisation();

		void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const;


		void ComputeDimensions(G4Tubs & scintTubs, const G4int copyNo, const G4VPhysicalVolume* physVol) const;


	private:  // Dummy declarations to get rid of warnings ...

		void ComputeDimensions (G4Trd&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Trap&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Cons&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Sphere&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Orb&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Torus&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Para&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Hype&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Box&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Polycone&,const G4int,const G4VPhysicalVolume*) const {}
		void ComputeDimensions (G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {}

	private:
		G4double fpitch;
		G4double fsirina;
		G4double fduzina;
		G4double fradius;
		G4bool fdice5;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


