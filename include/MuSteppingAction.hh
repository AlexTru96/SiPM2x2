//
#ifndef MuSteppingAction_H
#define MuSteppingACtion_H 1

#include "MuHistoManager.hh" 
#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class MuRecorderBase;
class MuEventAction;
class MuTrackingAction;
class MuHistoManager;


class MuSteppingAction : public G4UserSteppingAction
{
  public:

    MuSteppingAction(MuRecorderBase*, MuHistoManager*);
    virtual ~MuSteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    MuRecorderBase* fRecorder;
    G4bool fOneStepPrimaries;
    MuHistoManager* fhistoMan;


    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
