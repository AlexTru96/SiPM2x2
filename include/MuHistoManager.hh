//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef MuHistoManager_h
#define MuHistoManager_h 1
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuHistoManager
{
  public:

    MuHistoManager(G4String*);
   ~MuHistoManager();

    void fill(G4int, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double); 
    void printParticle(G4int , G4int , G4String , G4String , G4String , G4String , G4String , G4String , G4String );

  private:

    G4String*     fileName;
    G4int	  celdaX;
    G4int	  celdaY;
    G4double	  initial_posX;
    G4double	  initial_posY;
    G4double	  pos_celdaX;
    G4double	  pos_celdaY;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

