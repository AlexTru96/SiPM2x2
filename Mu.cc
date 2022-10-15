#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "MuDetectorConstruction.hh"
#include "MuPhysicsList.hh"
#include "MuPrimaryGeneratorAction.hh"
#include "MuHistoManager.hh" 
#include "MuTrackingAction.hh"
#include "MuSteppingAction.hh"
#include "MuRecorderBase.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#endif

using namespace std;

 int main(int argc, char** argv)
 
 {
   G4String filename; 
   G4String session;
   // construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager *runManager = new G4MTRunManager;
#else
  G4RunManager *runManager = new G4RunManager;
#endif
   MuHistoManager*  histo = new MuHistoManager(&filename);

   // set mandatory initialization classes
  runManager->SetUserInitialization(new MuDetectorConstruction);
  runManager->SetUserInitialization(new MuPhysicsList);

  runManager->SetUserAction(new MuPrimaryGeneratorAction);

  MuRecorderBase* recorder = NULL; 
  runManager->SetUserAction(new MuTrackingAction(recorder));
  runManager->SetUserAction(new MuSteppingAction(recorder,histo));



  runManager->Initialize();
  G4UImanager* UI = G4UImanager::GetUIpointer();

   // GUI mode (Argument : ./Mu)
   if(argc==1){
  #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
  #endif
	   
    // INTERFACE MANAGER
    G4UIExecutive *ui = 0;
    ui = new G4UIExecutive(argc, argv, session);

    // get the pointer to the UI manager and set verbosities
   G4UImanager *UI = G4UImanager::GetUIpointer();
   UI->ApplyCommand("/run/verbose 0");
   UI->ApplyCommand("/tracking/verbose 0");
   UI->ApplyCommand("/control/execute init_vis.mac");


    if (ui->IsGUI()) {
      UI->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
#ifdef G4VIS_USE 
    delete visManager;
#endif
    

    }

   // terminal mode (Argument: ./Mu + data_name.dat + output_name.dat)
   else if(argc==3){


    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 2");
    UImanager->ApplyCommand("/tracking/verbose 0");


    filename = argv[2];

    ifstream inc(argv[1],ios::in);

    if ( !inc ){
    	G4cout<<"Error al abrir archivo de datos"<<G4endl;
        }
    else {  
        ofstream myfile(filename, ios::out | ios::app);
	 myfile << "***************************************************************************************"<< G4endl;

	myfile <<"# # Incident_Particle "<<"fPartId "<<"Index "<<"intl_posX(mm) "<<"intl_posY(mm) "<<"intl_posZ(mm) "<<"Direction X  "<<"Y  "<<"Z  "<<"Energy(GeV)"<<G4endl;

        myfile <<"## Photoe_Data"<<" Energy_Photon(eV) "<<"global_time (ns) "<<"Position (mm) X  "<<"Y  "<<"Z  "<<"Direction X  "<<"Y  "<<"Z  "<<"#SiPM  "<<"#CeldaX  "<<"#CeldaY  "<<"Pos_celdaX  "<<"Pos_celdaY"<< G4endl;

 myfile << "***************************************************************************************"<< G4endl;
	 G4String particleName;
         G4String fUx,fUy,fUz,fX,fY,fZ,fTimeDelay,fTEnergy;
         G4int dig4,fPartId;

         while (inc>>fPartId>>dig4>>fUx>>fUy>>fUz>>fX>>fY>>fZ>>fTEnergy)
        {
           
          if ( fPartId == 1)
            {
              particleName = "gamma";
            }
          else if ( fPartId == 2 )
            {                       //e+
              particleName = "e+";
            }
          else if ( fPartId == 3 )
            {                       //e-
              particleName = "e-";
            }
          else if ( fPartId == 5 )
            {                       //mu+
              particleName = "mu+";
            }
          else if ( fPartId == 6 )
            {                       //mu-
              particleName = "mu-";
            }
	  else if ( fPartId == 13 )
            {                       //n
              particleName = "neutron";
            }
	  else if ( fPartId == 14 )
            {                       //p
              particleName = "proton";
            }
          else //do not simulate other particles
          continue;
          fTimeDelay=0.0;

	  histo->printParticle(fPartId , dig4 , fUx , fUy , fUz , fX , fY , fZ , fTEnergy );

          UImanager->ApplyCommand("/gun/particle " + particleName);
          UImanager->ApplyCommand("/gun/energy " + fTEnergy +" GeV");
          UImanager->ApplyCommand("/gun/position " + fUx + " " + fUy + " " + fUz + " mm");  
          UImanager->ApplyCommand("/gun/direction " + fX + " " + fY + " " + fZ );   
          UImanager->ApplyCommand("/run/beamOn");   
          
         }  

myfile.close();

      }



   }

    // job termination
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
 
   
   delete runManager;
   return 0;
 }

