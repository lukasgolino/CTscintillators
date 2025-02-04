//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN02.cc,v 1.16 2009-10-30 14:59:59 allison Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4SystemOfUnits.hh"


#include "G4VisExecutive.hh"


#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
  CLHEP::HepRandom::setTheSeed(time(NULL));
   
  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  PhysicsList* physics = new PhysicsList;
  physics->SetVerboseLevel(0);
  runManager->SetUserInitialization(physics);
   
  // User Action classes
  //
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);
  //
  RunAction* run_action = new RunAction;
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction;
  runManager->SetUserAction(event_action);
  //

  StackingAction* stack_action = new StackingAction;
  G4cout<<"Kill Tracks with KE < "<<stack_action->GetEnergyThreshold()/MeV<<" MeV"<<G4endl;
  runManager->SetUserAction(stack_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
    
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else           // interactive mode : define UI session
    { 

      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
      UImanager->ApplyCommand("/control/execute ../vis.mac");     
      ui->SessionStart();
      std::cout << "/n/n/n We do get here /n/n/n";
      delete ui;    
    }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete visManager;    
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

