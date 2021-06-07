#include "MuScintBar.hh"
#include "MuWLSFiber.hh"
#include "MuPMTSD.hh"
#include "G4SDManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "globals.hh"

MuPMTSD* MuScintBar::fPmt_SD=NULL;

G4LogicalVolume* MuScintBar::fHousing_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuScintBar::MuScintBar(G4RotationMatrix *pRot,
                             const G4ThreeVector &tlate,
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
			     MuPMTSD* fPmt_SD,	
                             MuDetectorConstruction* c)
  //Pass info to the G4PVPlacement constructor
  :G4PVPlacement(pRot,tlate,
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "housing",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();

  if(!fHousing_log || fUpdated){
 
    G4double housing_x=fScint_x+fD_mtl;
    G4double housing_y=fScint_y+fD_mtl;
    G4double housing_z=fScint_z+fD_mtl;

    //*************************** housing and scintillator
    fScint_box = new G4Box("scint_box",fScint_x/2.,fScint_y/2.,fScint_z/2.);
    
    fHousing_box = new G4Box("housing_box",housing_x/2.,housing_y/2.,housing_z/2.);

    fScint_log = new G4LogicalVolume(fScint_box,G4Material::GetMaterial("Polystyrene"), "scint_log",0,0,0);
 
    fHousing_log = new G4LogicalVolume(fHousing_box,G4Material::GetMaterial("TiO2"),"housing_log",0,0,0);

    new G4PVPlacement(0,G4ThreeVector(),fScint_log,"scintillator",fHousing_log,false,pCopyNo); 

  // Placing Fiber

    new MuWLSFiber(0,G4ThreeVector(0.,fScint_y/2-0.8*mm,0.),fScint_log,false,pCopyNo,fConstructor);


    //****************** Build PMTs connected with scintillation bar*******************

    G4double height_pmt = 1*mm;

    fPmt = new G4Box("pmt_tube",1.3/2.*mm,1.3/2.*mm,1.0/2.*mm);
 
    //the "photocathode" is a metal slab at the back of the glass that
    //is only a very rough approximation of the real thing since it only
    //absorbs or detects the photons based on the efficiency set below

    fPhotocath = new G4Box("photocath_tube",1.3/2.*mm,1.3/2.*mm,0.5/2.*mm);
 
    fPmt_log = new G4LogicalVolume(fPmt,G4Material::GetMaterial("Glass"),"pmt_log");
    fPhotocath_log = new G4LogicalVolume(fPhotocath,G4Material::GetMaterial("Al"),"photocath_log");
 
    new G4PVPlacement(0,G4ThreeVector(0,0,height_pmt/2),fPhotocath_log,"photocath",fPmt_log,false,pCopyNo);
    G4double z = housing_z/2 + height_pmt/2; 

    new G4PVPlacement(0,G4ThreeVector(0,fScint_y/2-0.8*mm,z),fPmt_log,"pmt",fHousing_log,false,pCopyNo); 

    //sensitive detector is not actually on the photocathode.
    //processHits gets done manually by the stepping action.
    //It is used to detect when photons hit and get absorbed&detected at the
    //boundary to the photocathode (which doesnt get done by attaching it to a
    //logical volume.
    //It does however need to be attached to something or else it doesnt get
    //reset at the begining of events

    fPmt_SD->SetPMTPos(pCopyNo,0,0,z); 
    fPhotocath_log->SetSensitiveDetector(fPmt_SD);

    VisAttributes();
    SurfaceProperties();
  }

  SetLogicalVolume(fHousing_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuScintBar::CopyValues(){
  fUpdated=fConstructor->GetUpdated();

  fScint_x=fConstructor->GetScintX();
  fScint_y=fConstructor->GetScintY();
  fScint_z=fConstructor->GetScintZ();
  fD_mtl=fConstructor->GetHousingThickness();
  fFiberRadius=fConstructor->GetFiberRadius();
  fRefl=fConstructor->GetHousingReflectivity();
}

void MuScintBar::VisAttributes(){
  G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  fHousing_log->SetVisAttributes(housing_va);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuScintBar::SurfaceProperties(){
const G4int num = 5;

//**Scintillator housing properties

  G4double ephoton2[] ={2.00*eV,3.0*eV,4.0*eV,5.0*eV,6.0*eV};  

  G4double ephotonRefl[] ={1.86969*eV,2.05436*eV,2.21852*eV,2.44422*eV,2.71918*eV,2.9572*eV,3.19112*eV,3.47428*eV,3.73282*eV,
3.99546*eV,4.21297*eV,4.38943*eV,4.47151*eV,4.52896*eV,4.55769*eV,4.59462*eV,4.63155*eV,4.66439*eV,4.68901*eV,4.72184*eV,4.76288*eV,
4.80802*eV,4.89009*eV,5.03783*eV,5.2225*eV,5.41949*eV,5.58364*eV,5.72727*eV,5.92836*eV,6.09661*eV};

  G4double reflectivity[] = {0.96965,0.97496,0.97313,0.97621,0.97709,0.97549,0.97152,0.96771,0.95674,0.94343,0.93469,0.91403,
0.84118,0.74938,0.67872,0.59393,0.49027,0.40076,0.31357,0.21698,0.14399,0.09225,0.0477,0.02223,0.02754,0.03053,0.03341,0.03387,
0.03451,0.04212};


  G4double efficiency[] = {0.0, 0.0, 0.0, 0.0, 0.0};
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", ephotonRefl, reflectivity, 30);
  scintHsngPT->AddProperty("EFFICIENCY", ephoton2, efficiency, num);

  G4OpticalSurface* OpScintHousingSurface =
  new G4OpticalSurface("HousingSurface",unified,polished,dielectric_metal);

  OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT); 
//////////////////////////////////////////////////////////////////////////////////////////////
  //**Photocathode surface properties

  G4double energy_photon[15] = {1.4490697674*eV,1.5724921136*eV,1.7129896907*eV,1.8881818182*eV,2.01*eV,2.1348179872*eV,2.2555656109*eV,
2.408115942*eV,2.68*eV,2.9671428571*eV,3.1449842271*eV,3.301192053*eV,3.4259793814*eV,3.6518681319*eV,3.8344615385*eV};  
 

  G4double photocath_EFF[15]={0.0490584269,0.0863872785,0.1319754533,0.1920315776,0.2499650508,0.2872296575,0.3275831586,0.3741427162,0.399044036,
0.3805415073,0.3547638872,0.3114140044,0.2887149994,0.2825629086,0.2567698697}; 


//Enables 'detection' of photons
  G4double photocath_ReR[num]={1.92,1.92,1.92,1.92,1.92};
  G4double photocath_ImR[num]={1.69,1.69,1.69,1.69,1.92};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",energy_photon,photocath_EFF,15);
  photocath_mt->AddProperty("REALRINDEX",ephoton2,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton2,photocath_ImR,num);

  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,dielectric_metal);

  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //**Create logical skin surfaces
  new G4LogicalSkinSurface("TiO2",fHousing_log,OpScintHousingSurface);
  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);

}
