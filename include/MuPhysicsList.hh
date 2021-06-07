#ifndef MuPhysicsList_h
#define MuPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class MuPhysicsList: public G4VModularPhysicsList
{
  public:

    MuPhysicsList();
    virtual ~MuPhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};

#endif
