#include "MuTrajectory.hh"
#include "MuTrackingAction.hh"
#include "MuUserTrackInformation.hh"
#include "MuDetectorConstruction.hh"
#include "MuRecorderBase.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuTrackingAction::MuTrackingAction(MuRecorderBase* r)
  : fRecorder(r) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new MuTrajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new MuUserTrackInformation);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuTrackingAction::PostUserTrackingAction(const G4Track* aTrack){
  MuTrajectory* trajectory=(MuTrajectory*)fpTrackingManager->GimmeTrajectory();
  MuUserTrackInformation*
    trackInformation=(MuUserTrackInformation*)aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the SiPM
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

    const G4VProcess* creator=aTrack->GetCreatorProcess();
    if(creator && creator->GetProcessName()=="OpWLS"){
      trajectory->WLS();
      trajectory->SetDrawTrajectory(true);
    }

      if(trackInformation->GetTrackStatus()&hitPMT)
        trajectory->SetDrawTrajectory(true);
    
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);

  if(fRecorder)fRecorder->RecordTrack(aTrack);
}
