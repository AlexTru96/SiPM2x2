#ifndef MuMainVolume_H
#define MuMainVolume_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "MuDetectorConstruction.hh"

class MuMainVolume : public G4PVPlacement
{
  public:

    MuMainVolume(G4RotationMatrix *pRot,
                 const G4ThreeVector &tlate,
                 G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 MuPMTSD* fPmt_SD, 
                 MuDetectorConstruction* c);

  private:

    void VisAttributes();

    void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot, G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin, G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k,MuPMTSD* sd);

    void CopyValues();

    G4bool fUpdated;
  
    MuDetectorConstruction* fConstructor;

    G4double fWcd_r;
    G4double fWcd_z;
    G4double fT_mtl;
    

    //Basic Volumes
    //
 
    G4Tubs* fTyvek_tube;
    G4Tubs* fWater_tube;


    G4LogicalVolume* fWater_log; 

    // Logical volumes

    static G4LogicalVolume* fTyvek_log; 

    //Sensitive Detectors
    // static MuPMTSD* fPmt_SD; // Disabled
};

#endif
