// --------------------------------------------------------------
// main program
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// Scout Objects
#include "ScoutDetectorConstruction.hh"
//#include "ScoutPhysicsList.hh"
#include "ScoutPrimaryGeneratorAction.hh"
#include "ScoutRunAction.hh"
#include "ScoutEventAction.hh"

// Borrowed from WCSim
#include "WCSimPhysicsListFactory.hh"
#include "WCSimPhysicsList.hh"

// STL objects
#include <vector>
#include <map>
#include <algorithm>

int main(int argc,char** argv) {

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes (physics list and detector)
  ScoutDetectorConstruction* detector = new ScoutDetectorConstruction;
  runManager->SetUserInitialization(detector);  
  //runManager->SetUserInitialization(new ScoutPhysicsList);
  WCSimPhysicsListFactory* physFactory = new WCSimPhysicsListFactory();
  physFactory->InitializeList();
  runManager->SetUserInitialization(physFactory);
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
    
  // set user action classes
  ScoutPrimaryGeneratorAction* ScoutGenerator = new ScoutPrimaryGeneratorAction;
  runManager->SetUserAction(ScoutGenerator);
  
  ScoutRunAction* ScoutRun = new ScoutRunAction;
  runManager->SetUserAction(ScoutRun);
  ScoutEventAction* eventAction = new ScoutEventAction(ScoutRun,ScoutGenerator);
  G4cout << "Process this LINE!!!" << G4endl;
  runManager->SetUserAction(eventAction);
  // eventAction is inherited by SteppingAction in order to switch colour
  // flag:
  //runManager->SetUserAction(new ScoutSteppingAction(eventAction));
  // runManager->SetUserAction(new ScoutStackingAction);

  //Initialize G4 kernel
  runManager->Initialize();
    
  // get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  // Define UI session for interactive mode.
  if(argc == 1)
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute macros/initInter.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
    }
  // Batch mode
  else
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

#ifdef G4VIS_USE
  if(visManager) delete visManager;
#endif
  delete runManager;

  return 0;
}

