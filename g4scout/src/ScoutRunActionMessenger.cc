#include "ScoutRunActionMessenger.hh"
#include "ScoutRunAction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include "globals.hh"

ScoutRunActionMessenger::ScoutRunActionMessenger(ScoutRunAction* run)
  :mRunAction(run)
{};

ScoutRunActionMessenger::~ScoutRunActionMessenger()
{};

void ScoutRunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{};
