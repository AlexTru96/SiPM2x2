#include "MuDetectorMessenger.hh"
#include "MuDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Scintillation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuDetectorMessenger::MuDetectorMessenger(MuDetectorConstruction* detector)
 : fMuDetector(detector)
{
  //Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("/Mu/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");
 
//  Geometry of the scintillator

  fScintGeometryDir = new G4UIdirectory("/Mu/detector/ScintGeometry/");
  fScintGeometryDir->SetGuidance("Geometry parameters of the Scint");

  fHousingThicknessCmd = new G4UIcmdWithADoubleAndUnit("/Mu/detector/ScintGeometry/HousingThickness",this);
  fHousingThicknessCmd->SetGuidance("Set the thickness of the housing.");
  fHousingThicknessCmd->SetParameterName("D_mtl",false);
  fHousingThicknessCmd->SetDefaultUnit("cm");

  fDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/Mu/detector/ScintGeometry/Dimensions",this);
  fDimensionsCmd->SetGuidance("Set the dimensions of the detector scintillator bars.");
  fDimensionsCmd->SetParameterName("scint_x","scint_y","scint_z",false);
  fDimensionsCmd->SetDefaultUnit("cm");

  fSlab_numCmd = new G4UIcmdWithAnInteger("/Mu/detector/ScintGeometry/ScintBarNumber",this);
  fSlab_numCmd ->SetGuidance("Set the number of slabs of the scintillator bars."); 
  fSlab_numCmd ->SetParameterName("Slab_num",false);

  fFiberRadiusCmd =  new G4UIcmdWithADoubleAndUnit("/Mu/detector/ScintGeometry/FiberRadius",this);
  fFiberRadiusCmd->SetGuidance("Set the radio of the Fiber.");
  fFiberRadiusCmd->SetParameterName("FiberRadius",false);
  fFiberRadiusCmd->SetDefaultUnit("cm");

  fD_mtlCmd =  new G4UIcmdWithADoubleAndUnit("/Mu/detector/ScintGeometry/D_mtl",this);
  fD_mtlCmd->SetGuidance("Set the D_mtl.");
  fD_mtlCmd->SetParameterName("D_mtl",false);
  fD_mtlCmd->SetDefaultUnit("cm");

 
  //  Tunning  of the scintillator
 
  fScintTuningDir = new G4UIdirectory("/Mu/detector/ScintTunning/");
  fScintTuningDir->SetGuidance("Tunning parameters of the Scint");
 
  fReflCmd =new G4UIcmdWithADouble("/Mu/detector/ScintTunning/MainScintYield",this);
  fReflCmd->SetGuidance("Set Reflectivity.");
  fReflCmd->SetGuidance("From 0 to 1");
 
  fWLSScintYieldCmd=new G4UIcmdWithADouble("/Mu/detector/ScintTunning/MainScintYield",this);
  fWLSScintYieldCmd->SetGuidance("Set scinitillation yield of main volume.");
  fWLSScintYieldCmd->SetGuidance("Specified in photons/MeV");

///////////////////////////////////////////////////////////////////////////////

// Defaults an update

  fUpdateCmd = new G4UIcommand("/Mu/detector/update",this);
  fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  fUpdateCmd->SetGuidance
    ("Must be run before beamOn if detector has been changed.");

  fDefaultsCmd = new G4UIcommand("/Mu/detector/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->SetGuidance("(Update still required)");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuDetectorMessenger::~MuDetectorMessenger()
{
    
//  Geometry of the scintillator
    delete   fDimensionsCmd;
    delete   fHousingThicknessCmd;
    delete   fDimensionsCmd;
    delete   fSlab_numCmd;
    delete   fFiberRadiusCmd;       
    delete   fD_mtlCmd;

//  Optical properties of the scintillator
    delete   fScintTuningDir; 
    delete   fReflCmd;        
    delete   fWLSScintYieldCmd;
    

    delete   fDetectorDir;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  //Scint tunning
  if (command == fReflCmd){
    fMuDetector
      ->SetHousingReflectivity(fReflCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fWLSScintYieldCmd){
    fMuDetector->SetWLSScintYield(fWLSScintYieldCmd->GetNewDoubleValue(newValue));
  }

  //update and defaults
  else if (command == fUpdateCmd){
    fMuDetector->UpdateGeometry();
  }
  else if (command == fDefaultsCmd){
    fMuDetector->SetDefaults();
  }

/////////////////////////////////////////////////////



}
