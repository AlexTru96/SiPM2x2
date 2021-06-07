#include "MuUserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuUserTrackInformation::MuUserTrackInformation()
  : fStatus(active),fReflections(0),fForcedraw(false) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuUserTrackInformation::~MuUserTrackInformation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuUserTrackInformation::AddTrackStatusFlag(int s)
{
  if(s&active) //track is now active
    fStatus&=~inactive; //remove any flags indicating it is inactive
  else if(s&inactive) //track is now inactive
    fStatus&=~active; //remove any flags indicating it is active
  fStatus|=s; //add new flags
}
