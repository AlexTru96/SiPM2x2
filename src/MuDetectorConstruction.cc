#include "MuDetectorConstruction.hh"
#include "MuPMTSD.hh"
#include "MuScintBar.hh"
#include "MuMainVolume.hh"
#include "MuDetectorMessenger.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


MuDetectorConstruction::MuDetectorConstruction():fMPTPStyrene(NULL) 
{
  fExperimentalHall_box = NULL;
  fExperimentalHall_log = NULL;
  fExperimentalHall_phys = NULL;

  fPstyrene = fPMMA = fPethylene1 = fPethylene2 = NULL;

  fWater = fAl = fVacuum = fAir = fGlass = fTiO2 = NULL;


  fN = fO = fC = fH = fTi = NULL;
  

  fUpdated = false;

  SetDefaults();

  fDetectorMessenger = new MuDetectorMessenger(this);
}

MuDetectorConstruction::~MuDetectorConstruction() {}

void MuDetectorConstruction::DefineMaterials(){

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density; 

  G4int polyPMMA = 1;
  G4int nC_PMMA = 3+2*polyPMMA;
  G4int nH_PMMA = 6+2*polyPMMA;


  G4int polyeth = 1;
  G4int nC_eth = 2*polyeth;
  G4int nH_eth = 4*polyeth;

  // Elements
  fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
  fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
  fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
  fO = new G4Element("O", "O", z=8., a= 16.00*g/mole);
  fTi = new G4Element("Ti", "Ti", z=22., a= 47.867*g/mole);
  fAl = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3); 

  // Glass
  fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
  fGlass->AddElement(fC,91.533*perCent);
  fGlass->AddElement(fH,8.467*perCent);

  // Water
  fWater = new G4Material("Water", density= 1*g/cm3, 2);
  fWater->AddElement(fH, 2);
  fWater->AddElement(fO, 1);	

  //Air
  fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
  fAir->AddElement(fN, 70*perCent);
  fAir->AddElement(fO, 30*perCent);

  //TiO2
  fTiO2 = new G4Material("TiO2", density= 4.26*g/cm3, 2);
  fTiO2 -> AddElement(fTi, 1);
  fTiO2 -> AddElement(fO, 2);

  //Vacuum
  fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,0.1*kelvin,1.e-19*pascal);

  //Polystyrene
  fPstyrene = new G4Material("Polystyrene", density= 1.03*g/cm3, 2);
  fPstyrene->AddElement(fC, 8);
  fPstyrene->AddElement(fH, 8);

  //Fiber(PMMA)
  fPMMA = new G4Material("PMMA", density=1190*kg/m3,3);
  fPMMA->AddElement(fH,nH_PMMA);
  fPMMA->AddElement(fC,nC_PMMA);
  fPMMA->AddElement(fO,2);

  //Cladding(polyethylene)
  fPethylene1 = new G4Material("Pethylene1", density=1200*kg/m3,2);
  fPethylene1->AddElement(fH,nH_eth);
  fPethylene1->AddElement(fC,nC_eth);

  //Double cladding(flourinated polyethylene)
  fPethylene2 = new G4Material("Pethylene2", density=1400*kg/m3,2);
  fPethylene2->AddElement(fH,nH_eth);
  fPethylene2->AddElement(fC,nC_eth);


//OPTICAL PROPERTIES////////////////////////////////////////////////////////

  const G4int Opnum = 40;
  
  G4double Energy[Opnum] =
	{1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
	2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
	2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
	2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
	2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
	2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
	3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
	3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
	3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
	4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV};

  // glass 

  G4double RINDEX_glass[Opnum] =
      { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600 }; 
  
 G4double ABSORPTION_glass[Opnum]= 
     { 1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm, 
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm};  
 
 G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
 
 glass_mt->AddProperty("ABSLENGTH",Energy,ABSORPTION_glass,Opnum);
 glass_mt->AddProperty("RINDEX",Energy,RINDEX_glass,Opnum);
 fGlass->SetMaterialPropertiesTable(glass_mt);

 // photocatode

 G4double RINDEX_Al[Opnum]=
	{1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,
	 1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,
	 1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,
	 1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49};

 G4double ABSORPTION_Al[Opnum]={.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,
				.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,
				.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,
				.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,.1*mm,};


 G4MaterialPropertiesTable *Al_mt = new G4MaterialPropertiesTable();
 Al_mt->AddProperty("ABSLENGTH",Energy,ABSORPTION_Al,Opnum);
 Al_mt->AddProperty("RINDEX",Energy,RINDEX_Al,Opnum);
 fAl->SetMaterialPropertiesTable(Al_mt);

//  DOW Styron 663 W (Scintillator) ///////////////////////////////////////////////////////////////////////////////////

  const G4int wlsnum = 4;
  G4double wls_Energy[] = {2.00*eV,2.77*eV,3.30*eV,3.87*eV};

  G4double scint_photon[] = {2.12778*eV,2.19117*eV,2.25256*eV,2.31*eV,2.37336*eV,2.42483*eV,2.48619*eV,2.54752*eV,2.60492*eV,2.65834*eV,
2.70976*eV,2.74535*eV,2.79086*eV,2.82054*eV,2.8542*eV,2.87592*eV,2.89172*eV,2.90551*eV,2.91731*eV,2.9311*eV,2.94289*eV,2.95863*eV,
2.97635*eV,2.99009*eV,3.0177*eV,3.06521*eV,3.10276*eV,3.14629*eV,3.17194*eV,3.19973*eV,3.21766*eV,3.23757*eV,3.25552*eV,3.27347*eV,
3.29536*eV,3.31527*eV,3.33519*eV,3.35113*eV,3.36311*eV,3.37114*eV,3.37915*eV,3.39313*eV,3.39916*eV,3.40717*eV,3.41919*eV,3.43513*eV,
3.44709*eV,3.46301*eV,3.4849*eV,3.51863*eV};

const G4int scinnum = sizeof(scint_photon)/sizeof(G4double);

G4double trans_photon[] = {2.11264*eV,2.17414*eV,2.23565*eV,2.32095*eV,2.39833*eV,2.50942*eV,2.60861*eV,2.71575*eV,2.83478*eV,
2.93597*eV,3.0431*eV,3.10462*eV,3.15825*eV,3.19409*eV,3.21409*eV,3.22419*eV,3.23227*eV,3.23641*eV,3.24456*eV,3.25265*eV,3.27459*eV,
3.34798*eV,3.45309*eV,3.52452*eV};

const G4int transnum = sizeof(trans_photon)/sizeof(G4double);

G4double rIndexPstyrene[wlsnum]={ 1.5, 1.5, 1.5, 1.5};
G4double absorption1[wlsnum]={5.5*cm, 5.5*cm, 5.5*cm, 5.5*cm};

  G4double transmittance[]={0.69374,0.69382,0.68995,0.69204,0.68819,0.69229,0.69045,0.68862,0.68878,0.68298,0.68313,0.6753,0.63783,
0.57463,0.49363,0.40471,0.32765,0.24465,0.13596,0.0589,0.0,0.00369,0.02162,0.01776};

  G4double scintilFast[]={0.01779,0.02569,0.03755,0.04545,0.06522,0.08696,0.11462,0.16206,0.18775,0.22332,0.27273,0.30632,0.33794,
0.35573,0.36561,0.40119,0.43083,0.4664,0.51186,0.55336,0.60079,0.65415,0.71146,0.77866,0.8419,0.85968,0.91107,0.93478,0.99012,
0.96443,0.91502,0.86561,0.80435,0.74308,0.69368,0.64625,0.59289,0.54545,0.49802,0.44862,0.40514,0.3498,0.30435,0.25889,0.19368,
0.1502,0.11067,0.0751,0.02964,0.00198};

  fMPTPStyrene = new G4MaterialPropertiesTable();
  fMPTPStyrene->AddProperty("RINDEX",wls_Energy,rIndexPstyrene,wlsnum);
  fMPTPStyrene->AddProperty("ABSLENGTH",wls_Energy,absorption1,wlsnum);
  fMPTPStyrene->AddProperty("FASTCOMPONENT",scint_photon, scintilFast,scinnum);
  fMPTPStyrene->AddProperty("TRANSMITTANCE",trans_photon, transmittance,transnum);
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",16000./MeV);
  fMPTPStyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
  fMPTPStyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns); // approx.
  fPstyrene->SetMaterialPropertiesTable(fMPTPStyrene);


  // Set the Birks Constant for the Polystyrene scintillator

  fPstyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);


  // air properties

  G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};
  const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
  G4double vacuum_RIND[]={1.,1.,1.};
  assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex


  // Properties of BCF-99-29AMC (Fiber)

  G4double absphoton[] = {2.00*eV,2.30*eV,2.50792*eV,2.59266*eV,2.61755*eV,2.62562*eV,2.63366*eV,2.65367*eV,2.6697*eV,2.68573*eV,2.69975*eV,2.71977*eV,2.74179*eV,2.75582*eV,
2.77185*eV,2.79786*eV,2.81389*eV,2.8279*eV,2.84391*eV,2.85792*eV,2.87989*eV,2.90388*eV,2.94182*eV,2.98572*eV,3.01964*eV,3.05555*eV,
3.08746*eV,3.1134*eV,3.14131*eV,3.17121*eV,3.20909*eV,3.24497*eV,3.27686*eV,3.30476*eV,3.32865*eV,3.35256*eV,3.37647*eV,3.39639*eV,
3.43822*eV,3.47008*eV};

const G4int absnum = sizeof(absphoton)/sizeof(G4double);

  G4double emiphoton[] = {2.07448*eV,2.08919*eV,2.10881*eV,2.13171*eV,2.15296*eV,2.18074*eV,2.2036*eV,2.22647*eV,2.26403*eV,2.28524*eV,
2.30806*eV,2.32275*eV,2.33742*eV,2.35372*eV,2.3749*eV,2.39119*eV,2.40747*eV,2.42049*eV,2.43678*eV,2.44816*eV,2.4628*eV,2.47579*eV,
2.48712*eV,2.50015*eV,2.51642*eV,2.53927*eV,2.56542*eV,2.58674*eV,2.59992*eV,2.60654*eV,2.61483*eV,2.61491*eV,2.61992*eV,2.62657*eV,
2.62828*eV,2.63162*eV,2.63499*eV,2.63669*eV,2.63839*eV,2.6401*eV,2.64181*eV,2.64678*eV,2.65015*eV,2.65185*eV,2.65518*eV,2.66178*eV,
2.67167*eV,2.67989*eV,2.69794*eV};

  const G4int eminum = sizeof(emiphoton)/sizeof(G4double);

  G4double RefractiveIndexFiber[wlsnum]={1.60, 1.60, 1.60, 1.60};

  G4double AbsFiber[wlsnum]={9.00*m,9.00*m,0.1*mm,0.1*mm};
  G4double AbsFiberWLS[]={2*m,2*m,993.6*mm,380.0*mm,255.24*mm,170.32*mm,125.15*mm,88.51*mm,62.59*mm,43.412*mm,31.903*mm,22.13*mm,14.772*mm,10.648*mm,7.245*mm,
4.744*mm,3.3546*mm,2.5621*mm,1.9569*mm,1.5835*mm,1.2816*mm,0.979*mm,0.7481*mm,0.6295*mm,0.5831*mm,0.51979*mm,0.49088*mm,0.46353*mm,
0.47279*mm,0.47304*mm,0.50154*mm,0.51163*mm,0.5529*mm,0.5861*mm,0.6711*mm,0.7393*mm,0.8144*mm,0.8632*mm,1.0274*mm,1.2464*mm};

  G4double EmissionFibWLS[]={0.05899,0.0728,0.07618,0.083,0.09331,0.11748,0.14167,0.16586,0.20734,0.24542,0.29392,0.31468,0.34933,
0.38744,0.43942,0.48795,0.53648,0.57808,0.61966,0.66821,0.71675,0.77919,0.85552,0.89017,0.94565,0.98026,0.99401,0.96613,0.90703,
0.86186,0.79237,0.74376,0.68471,0.6187,0.57356,0.53187,0.47283,0.43463,0.39642,0.3478,0.30265,0.26444,0.20886,0.17066,0.13592,
0.09769,0.05598,0.02816,0.00029};

  G4MaterialPropertiesTable* fiberProperty = new G4MaterialPropertiesTable();

  fiberProperty->AddProperty("RINDEX",wls_Energy,RefractiveIndexFiber,wlsnum);
  fiberProperty->AddProperty("WLSABSLENGTH",absphoton,AbsFiberWLS,absnum);
  fiberProperty->AddProperty("WLSCOMPONENT",emiphoton,EmissionFibWLS,eminum);
  fiberProperty->AddConstProperty("WLSTIMECONSTANT", 3.5*ns);
  fPMMA->SetMaterialPropertiesTable(fiberProperty);


  G4double RefractiveIndexClad1[wlsnum]={1.49, 1.49, 1.49, 1.49};
  G4MaterialPropertiesTable* clad1Property = new G4MaterialPropertiesTable();
  clad1Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad1,wlsnum);
  clad1Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
  fPethylene1->SetMaterialPropertiesTable(clad1Property);


  G4double RefractiveIndexClad2[wlsnum]={ 1.42, 1.42, 1.42, 1.42};
  G4MaterialPropertiesTable* clad2Property = new G4MaterialPropertiesTable();
  clad2Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad2,wlsnum);
  clad2Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
  fPethylene2->SetMaterialPropertiesTable(clad2Property);
  }

G4VPhysicalVolume* MuDetectorConstruction::Construct(){

	DefineMaterials();
	return ConstructDetector();

}


G4VPhysicalVolume* MuDetectorConstruction::ConstructDetector(){

  //The experimental hall walls are all 25 cm away from housing walls
  G4double expHall_x = 25*cm;
  G4double expHall_y = 25*cm;
  G4double expHall_z = 25*cm;

  //Create experimental hall
  fExperimentalHall_box  = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  fExperimentalHall_log  = new G4LogicalVolume(fExperimentalHall_box,fAir,"expHall_log",0,0,0);
  fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),fExperimentalHall_log,"expHall",0,false,0);
  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);


   G4int num_slab=fSlab_num; // Number of Slabs (2x2 sandwich)

   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   MuPMTSD *fPmt_SD;

   fPmt_SD = new MuPMTSD("/MuDet/pmtSD");
   SDman->AddNewDetector(fPmt_SD);
   fPmt_SD->InitPMTs(2*num_slab+1); // set SiPM index


   // Placing scintillators
   G4double dis_slab=fScint_x+2*mm;

   G4RotationMatrix* rm_X90 = new G4RotationMatrix();	   
   rm_X90->rotateX(90*deg);

   G4RotationMatrix* rm_Y90 = new G4RotationMatrix();	    
   rm_Y90->rotateY(90*deg);
   rm_Y90->rotateZ(90*deg);
   
   for(int i=0;i<num_slab;i++){
   G4double pos_slab=-1*(dis_slab/2)*(num_slab-1)+dis_slab*i;

   fPmt_SD->SetPMTPos(i,-fScint_z/2,-1.4*cm,1*m); 
   new MuScintBar(rm_X90,G4ThreeVector(pos_slab,0,0*cm),fExperimentalHall_log,false,i,fPmt_SD,this);
   
   fPmt_SD->SetPMTPos(i+fSlab_num,-fScint_z/2,-1.4*cm,0*m); 
   new MuScintBar(rm_Y90,G4ThreeVector(0,pos_slab,1.1*cm),fExperimentalHall_log,false,i+num_slab,fPmt_SD,this);
   }; 
  

  return fExperimentalHall_phys;

  }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MuDetectorConstruction::SetDefaults(){
  // Resets to default values

  // Default variables for scintillator

  // geometry scintillator
    fScint_x		= 42*mm;
    fScint_y		= 10*mm;
    fScint_z		= 80*mm;
    fFiberRadius	= 0.6*mm;
    fD_mtl              = 2*mm;
    fSlab_num           = 2; 
       
//  tuning scintillator
//    G4double	  fSRefl  = 1.0;
//    G4double    fFSYield= 1.0;
  
  fUpdated=true;

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Scintillator parameters (Disabled messenger) 


// Geometry scintillator and fibers parameters

void MuDetectorConstruction::SetHousingThickness(G4double d_mtl){
  this->fD_mtl=d_mtl;
  fUpdated=true;
}

void MuDetectorConstruction::SetDimensions(G4ThreeVector dims){
  this->fScint_x=dims[0];
  this->fScint_y=dims[1];
  this->fScint_z=dims[2];
  fUpdated=true;
  }

void MuDetectorConstruction::SetD_mtl(G4double D_mtl){
  this->fD_mtl=D_mtl;
  fUpdated=true;
}

void MuDetectorConstruction::SetSlab_num(G4int Slab_num){
  this->fSlab_num=Slab_num;
  fUpdated=true;
}

void MuDetectorConstruction::SetFiberRadius(G4double FiberRadius){
  this->fFiberRadius=FiberRadius;
  fUpdated=true;
}

//  tuning scintillator

void MuDetectorConstruction::SetHousingReflectivity(G4double SRefl){
  this->fSRefl=SRefl;
  fUpdated=true;
}
    
void MuDetectorConstruction::SetWLSScintYield(G4double y){
  fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD",y/MeV);
  }

void MuDetectorConstruction::UpdateGeometry(){
  
  // clean-up previous geometry
  
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();
  G4SurfaceProperty::CleanSurfacePropertyTable();
      
  // define new one

  DefineMaterials();
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  
  fUpdated=false;

  }

  
