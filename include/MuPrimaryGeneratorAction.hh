
#ifndef MuPrimaryGeneratorAction_h
#define MuPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class MuPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    MuPrimaryGeneratorAction();
    virtual ~MuPrimaryGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);

  private:

    G4ParticleGun* fParticleGun;
};

#endif
