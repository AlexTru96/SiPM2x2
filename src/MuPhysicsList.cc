#include "MuPhysicsList.hh"

#include "MuGeneralPhysics.hh"

#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPhysicsList::MuPhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;

  // General Physics
  RegisterPhysics( new MuGeneralPhysics("general") );

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );

  opticalPhysics->SetWLSTimeProfile("delta");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);

  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPhysicsList::~MuPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuPhysicsList::SetCuts(){

  //   the default cut value for all particle types
  SetCutsWithDefault();

}
