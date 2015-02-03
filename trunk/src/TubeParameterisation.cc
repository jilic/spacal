/* 
	@author Jelena Ilic
	@mail   jelena.ilic.s@gmail.com
	@date   Summer 2014
*/
#include "TubeParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TubeParameterisation::TubeParameterisation(G4double pitch,G4double sirina, G4double duzina,G4double radius, G4bool dice5)
{
   fpitch = pitch; 
   fsirina = sirina ; 
   fduzina = duzina ; 
   fradius = radius ; 
   fdice5   = dice5 ; 
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TubeParameterisation::~TubeParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TubeParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{

	double shift(0); 
	double firstPoint(0); 
	int nrow(0); //the number of holes in a row
	double xspacing =fpitch;
	double yspacing =fpitch;

	if( int(fsirina/fpitch)%2 ) {
		nrow= int(fsirina/fpitch)-1;

		shift =0.5*fpitch*( (fsirina/fpitch) - int(fsirina/fpitch) ) ;
		firstPoint = -fsirina/2 + shift + fpitch;
	} else {
		nrow = int(fsirina/fpitch);
		shift =0.5*fpitch*( (fsirina/fpitch) - int(fsirina/fpitch) ) ;
		firstPoint = -fsirina/2 + shift + fpitch/2.0;
	}

	int irow = copyNo % nrow;
	int kcolumn = copyNo / nrow; 

	G4double Xposition(0);
	G4double Yposition(0);
	G4double Zposition(0);
    
	//Dice4 OK
	if(!fdice5){
		Xposition = firstPoint+ kcolumn * xspacing ; 
		Yposition = firstPoint+ irow * yspacing ; 
	}
	
	
	// Ddice5  TODO: not correct need 2 parametrisation for dice5. Not urgent	
	if(fdice5 ){

		if( irow % 2 == 1 ) {
			Xposition = firstPoint+ (kcolumn +0.5) * xspacing ;
			Yposition = firstPoint+ irow * yspacing ; 

		}
		else{
			Xposition = firstPoint+ kcolumn * xspacing ; 
			Yposition = firstPoint+ irow * yspacing ; 
		}
	
	}
		
	G4ThreeVector origin(Xposition,Yposition,Zposition);
	physVol->SetTranslation(origin);
	physVol->SetRotation(0);
	


}
	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TubeParameterisation::ComputeDimensions (G4Tubs& scintTubs, const G4int copyNo, const G4VPhysicalVolume*) const
{
  scintTubs.SetZHalfLength(fduzina/2.0);
  scintTubs.SetInnerRadius(0);
  scintTubs.SetOuterRadius(fradius);
}
