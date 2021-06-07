#ifndef MuScintBar_H
#define MuScintBar_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "MuDetectorConstruction.hh"

class MuScintBar : public G4PVPlacement
{
  public:

    MuScintBar(G4RotationMatrix *pRot,
                 const G4ThreeVector &tlate,
                 G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 MuPMTSD* fPmt_SD, 
                 MuDetectorConstruction* c);

  private:

    void VisAttributes();
    void SurfaceProperties();

    void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot, G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin, G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k,MuPMTSD* sd);

    void CopyValues();

    G4bool fUpdated;
  
    MuDetectorConstruction* fConstructor;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fD_mtl;
    
    G4double fFiberRadius;

    G4double fRefl;

    //Basic Volumes
    //
    G4Box* fScint_box;
    G4Box* fHousing_box;

    G4Box* fPmt;
    G4Box* fPhotocath;
    

    // Logical volumes
    //
    G4LogicalVolume* fScint_log;

    static G4LogicalVolume* fHousing_log;
    G4LogicalVolume* fPmt_log;
   
    G4LogicalVolume* fPhotocath_log;
    


    //Sensitive Detectors
    static MuPMTSD* fPmt_SD;
};

#endif
