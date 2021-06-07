#ifndef MuDetectorConstruction_H
#define MuDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class MuMainVolume;

#include "MuPMTSD.hh"
#include "MuDetectorMessenger.hh"
#include "MuMainVolume.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"

class MuDetectorConstruction : public G4VUserDetectorConstruction
{
public:

    MuDetectorConstruction();

    virtual ~MuDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

//Functions to modify the geometry scintillator (Disabled)

    G4double GetScintX(){return fScint_x;}
    G4double GetScintY(){return fScint_y;}
    G4double GetScintZ(){return fScint_z;}   
    void SetDimensions(G4ThreeVector );
    G4double GetHousingThickness(){return fD_mtl;}
    void SetHousingThickness(G4double );
    G4double GetFiberRadius(){return fFiberRadius;};
    void SetFiberRadius(G4double );
    G4double GetD_mtl(){return fD_mtl;};
    void SetD_mtl(G4double );
    G4double GetSlab_num(){return fSlab_num;};
    void SetSlab_num(G4int );
     
//  tuning scintillator

    G4double GetHousingReflectivity(){return fSRefl;};
    void SetHousingReflectivity(G4double );
    G4double SetWLSScintYield(){return fFSYield;};
    void SetWLSScintYield(G4double );
    void SetDefaults();
    
    
//  rebuild the geometry based on changes. 
    
    void UpdateGeometry();
    G4bool GetUpdated(){return fUpdated;}
        
    
private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();
    
    MuDetectorMessenger* fDetectorMessenger;

    G4bool fUpdated;

    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    G4Box* fScint_box;
    G4LogicalVolume* fScint_log;
    G4VPhysicalVolume* fScint_phys;


//Materials & Elements

    G4Element* fN;
    G4Element* fO;
    G4Element* fC;
    G4Element* fH;
    G4Element* fTi;
    G4Material* fVacuum;
    G4Material* fAl;
    G4Material* fGlass;
    G4Material* fWater;
    G4Material* fTiO2;
    G4Material* fAir;
    G4Material* fPstyrene;
    G4Material* fPMMA;
    G4Material* fPethylene1;
    G4Material* fPethylene2;

// geometry scintillator
    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    G4double fFiberRadius;
    G4double fD_mtl;    
    G4int fSlab_num;
    
       
//  tuning scintillator
    G4double	fSRefl;
    G4double    fFSYield;

//  optical properties tables.
    G4MaterialPropertiesTable* fScint_mt;
    G4MaterialPropertiesTable* fMPTPStyrene;
//    G4MaterialPropertiesTable* Al_mt;
//    G4MaterialPropertiesTable* glass_mt;
    
//  SiPM construction
    G4Tubs* fPmt;
    G4Tubs* fPhotocath;
    G4LogicalVolume* fPmt_log;   
    G4LogicalVolume* fPhotocath_log;
};

#endif
