#include "MuSteppingAction.hh"
#include "MuTrackingAction.hh"
#include "MuTrajectory.hh"
#include "MuPMTSD.hh"
#include "MuUserTrackInformation.hh"
#include "MuUserEventInformation.hh"
#include "MuRecorderBase.hh"
#include "MuHistoManager.hh"
#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuSteppingAction::MuSteppingAction(MuRecorderBase* r, MuHistoManager* histoMan)
  : fRecorder(r),fOneStepPrimaries(false)
{
 
 fhistoMan=histoMan;
 fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuSteppingAction::~MuSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuSteppingAction::UserSteppingAction(const G4Step * theStep){
  G4Track* theTrack = theStep->GetTrack();

  
 
  MuUserTrackInformation* trackInformation
    =(MuUserTrackInformation*)theTrack->GetUserInformation();
  
  // Using PostStep to obtain particle information
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4OpBoundaryProcess* boundary=NULL;



  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }


  if(!thePostPV){//out of world
    return;
  }


  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){

    boundaryStatus=boundary->GetStatus();
 

    if(thePostPoint->GetStepStatus()==fGeomBoundary){

      fExpectedNextStatus=Undefined;

      switch(boundaryStatus){

      case Detection: //Note, this assumes that the volume causing detection
                      //is the photocathode because it is the only one with
                      //non-zero efficiency
        {
        //Triger sensitive detector manually since photon is
        //absorbed but status was Detection
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String sdName="/MuDet/pmtSD";
        MuPMTSD* pmtSD = (MuPMTSD*)SDman->FindSensitiveDetector(sdName);
        if(pmtSD)pmtSD->ProcessHits_constStep(theStep,NULL);
        trackInformation->AddTrackStatusFlag(hitPMT);
	
	//if detected gather particle data
	G4double ener=theStep->GetTrack()->GetKineticEnergy();

	G4double time=theStep->GetPostStepPoint()->GetGlobalTime();

	G4double dirX = theStep->GetPostStepPoint()->GetMomentumDirection().x();
	G4double dirY = theStep->GetPostStepPoint()->GetMomentumDirection().y();
	G4double dirZ = theStep->GetPostStepPoint()->GetMomentumDirection().z();
	
        G4double posX = theStep->GetTrack()->GetPosition().x();
	G4double posY = theStep->GetTrack()->GetPosition().y();
	G4double posZ = theStep->GetTrack()->GetPosition().z();

        G4int pmtNumber=theStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
	
	// sending particle data
        fhistoMan->fill(pmtNumber, ener, time, dirX,dirY,dirZ, posX, posY,posZ);

        break;
        }

	default:
        break;
      }
      
    }
  }

  if(fRecorder)fRecorder->RecordStep(theStep);
}
