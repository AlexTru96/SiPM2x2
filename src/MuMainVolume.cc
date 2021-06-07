#include "MuMainVolume.hh"
#include "MuPMTSD.hh"

#include "globals.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"

// MuPMTSD* MuMainVolume::fPmt_SD=NULL; // Disabled

G4LogicalVolume* MuMainVolume::fTyvek_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuMainVolume::MuMainVolume(G4RotationMatrix *pRot,
                             const G4ThreeVector &tlate,
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
                             MuPMTSD* fPmt_SD,
                             MuDetectorConstruction* c)
  //Pass info to the G4PVPlacement constructor
  :G4PVPlacement(pRot,tlate,
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Tubs("temp",0,1,1,0.,360.*deg),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "tyvek",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();


  if(!fTyvek_log || fUpdated){
 
    G4double fTyvek_r=fWcd_r+fT_mtl;
    G4double fTyvek_z=fWcd_z+fT_mtl;

 
    fWater_tube = new G4Tubs("water_tank",0.,fWcd_r,fWcd_z/2.,0.,360.*deg);

    fTyvek_tube = new G4Tubs("tyvek_tank",0.,fTyvek_r,fTyvek_z/2.,0.,360.*deg);

    fTyvek_log = new G4LogicalVolume(fTyvek_tube,G4Material::GetMaterial("Polystyrene"),"tyvek_log",0,0,0); 

    fWater_log = new G4LogicalVolume(fWater_tube,G4Material::GetMaterial("Water"),"water_log",0,0,0);
   

    new G4PVPlacement(0,G4ThreeVector(),fWater_log,"wcd",fTyvek_log,false,0);

  }

  SetLogicalVolume(fTyvek_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuMainVolume::CopyValues(){

  fUpdated=fConstructor->GetUpdated();

}




