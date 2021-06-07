#include <fstream>
#include <iostream>

#include "MuHistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4UIcommand.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuHistoManager::MuHistoManager(G4String* file)
{
   fileName = file;

   auto analysisManager = G4AnalysisManager::Instance();
   G4String RootName = "SiPM2x2";
   analysisManager->OpenFile(RootName);
   G4int nbins = 40;
   analysisManager->CreateH1("ScE1", "SE1", nbins, 0.*eV, 6.*eV, "eV");
   analysisManager->CreateH1("ScE2", "SE2", nbins, 0.*eV, 6.*eV, "eV");
   analysisManager->CreateH1("ScE3", "SE3", nbins, 0.*eV, 6.*eV, "eV");
   analysisManager->CreateH1("ScE4", "SE4", nbins, 0.*eV, 6.*eV, "eV");
   analysisManager->CreateH1("ScT1", "ST1", nbins, 0.*ns, 60.*ns, "ns");
   analysisManager->CreateH1("ScT2", "ST2", nbins, 0.*ns, 60.*ns, "ns");
   analysisManager->CreateH1("ScT3", "ST3", nbins, 0.*ns, 60.*ns, "ns");
   analysisManager->CreateH1("ScT4", "ST4", nbins, 0.*ns, 60.*ns, "ns");
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuHistoManager::~MuHistoManager()
{ 
  delete G4AnalysisManager::Instance(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuHistoManager::printParticle(G4int fPartId ,G4int dig4 ,G4String fUx ,G4String fUy ,G4String fUz ,G4String fX ,G4String fY ,G4String fZ ,G4String fTEnergy)
{
ofstream myfile(*fileName, ios::out | ios::app);			
myfile << "# #" << " " << fPartId << " " << dig4 << " " << fUx << " " << fUy << " " << fUz << " " << fX << " " << fY <<" "<<fZ<< " "   << fTEnergy << G4endl;
    return;                  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuHistoManager::fill(G4int pmt, G4double ener, G4double time, G4double dirX,G4double dirY,G4double dirZ, G4double X, G4double Y, G4double Z)
{
  auto analysisManager = G4AnalysisManager::Instance();
  ofstream myfile(*fileName, ios::out | ios::app);

  // Cell assignation (3D projection to 2D)

if(pmt==3)
	{	
	initial_posX = Y-21.3500;
	initial_posY = Z-14.5500;
	
	celdaX=int(initial_posX/0.05)+1;
	celdaY=int(initial_posY/0.05)+1;

	pos_celdaX = initial_posX/0.05 - int(initial_posX/0.05);
	pos_celdaY = initial_posY/0.05 - int(initial_posY/0.05);
	}
if(pmt==1)
	{	
	initial_posX = Y-21.3500;
	initial_posY = Z+4.8500;
	
	celdaX=int(initial_posX/0.05)+1;
	celdaY=int(initial_posY/0.05)+1;

	pos_celdaX = initial_posX/0.05 - int(initial_posX/0.05);
	pos_celdaY = initial_posY/0.05 - int(initial_posY/0.05);
	}

   // Writing

	myfile<<ener*10e5<<" "<<time<<"  "<<X<<" "<<Y<<" "<<Z<<" "<<dirX<<" "<<dirY<<" "<<dirZ<<" "<< pmt+1<<" "<<celdaX<<" "<< celdaY<<" "<<pos_celdaX<<" "<<pos_celdaY<<G4endl;

  // Histogram filling
  analysisManager->FillH1(pmt, ener);
  analysisManager->FillH1(pmt+4, time);

    return;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


