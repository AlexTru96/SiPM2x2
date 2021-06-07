#ifndef MuTrackingAction_h
#define MuTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class MuRecorderBase;

class MuTrackingAction : public G4UserTrackingAction {

  public:

    MuTrackingAction(MuRecorderBase*);
    virtual ~MuTrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:

    MuRecorderBase* fRecorder;

};

#endif
