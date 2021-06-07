#ifndef MuDetectorMessenger_h
#define MuDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MuDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class MuDetectorMessenger: public G4UImessenger
{
  public:

    MuDetectorMessenger(MuDetectorConstruction*);
    virtual ~MuDetectorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    MuDetectorConstruction*     fMuDetector;

    G4UIdirectory*               fDetectorDir;
    
//  Geometry of the scintillator
    G4UIdirectory*               fScintGeometryDir;
    G4UIcmdWithADoubleAndUnit*   fHousingThicknessCmd;
    G4UIcmdWith3VectorAndUnit*   fDimensionsCmd;
    G4UIcmdWithAnInteger*        fSlab_numCmd;
    G4UIcmdWithADoubleAndUnit*   fFiberRadiusCmd;       
    G4UIcmdWithADoubleAndUnit*   fD_mtlCmd;
    G4UIcmdWithABool*            fMuCmd;

//  Optical properties of the scintillator
    G4UIdirectory*               fWCDTuningDir;	
    G4UIdirectory*               fScintTuningDir; 
    G4UIcmdWithADouble*		 fReflCmd;        
    G4UIcmdWithADouble*          fWLSScintYieldCmd;


    G4UIcommand* 	         fUpdateCmd;     
    G4UIcommand*                 fDefaultsCmd;
   

};

#endif
