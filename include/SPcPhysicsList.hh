/* 
	@author Jelena Ilic
	@mail   jelenailic.sk@gmail.com
	@date   Summer 2014
*/
#ifndef SPcPhysicsList_h
#define SPcPhysicsList_h 1


#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class SPcPhysicsList: public G4VModularPhysicsList
{
  public:

    SPcPhysicsList();
    virtual ~SPcPhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};

#endif
