#ifndef MuTrajectory_h
#define MuTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class MuTrajectory : public G4Trajectory
{
  public:

    MuTrajectory();
    MuTrajectory(const G4Track* aTrack);
    MuTrajectory(MuTrajectory &);
    virtual ~MuTrajectory();
 
    virtual void DrawTrajectory() const;
    virtual void DrawTrajectory(G4int i_mode=0) const;
 
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetDrawTrajectory(G4bool b){fDrawit=b;}
    void WLS(){fWls=true;}
    void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

  private:

    G4bool fWls;
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4Allocator<MuTrajectory> MuTrajectoryAllocator;

inline void* MuTrajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)MuTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void MuTrajectory::operator delete(void* aTrajectory)
{
  MuTrajectoryAllocator.FreeSingle((MuTrajectory*)aTrajectory);
}

#endif
