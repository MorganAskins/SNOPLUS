#include "ScoutEventActionMessenger.hh"
#include "ScoutEventAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"
#include "globals.hh"

ScoutEventActionMessenger::ScoutEventActionMessenger(ScoutEventAction* EvAct)
  : mEventAction(EvAct){;}

ScoutEventActionMessenger::~ScoutEventActionMessenger(){;}

void ScoutEventActionMessenger::SetNewValue(G4UIcommand*, G4String){}
