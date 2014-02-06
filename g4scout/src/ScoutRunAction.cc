#include "ScoutRunAction.hh"
#include "ScoutRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

ScoutRunAction::ScoutRunAction()
{
  mRunMessenger = new ScoutRunActionMessenger(this);
}

ScoutRunAction::~ScoutRunAction()
{
  delete mRunMessenger;
}

void ScoutRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void ScoutRunAction::EndOfRunAction(const G4Run* aRun){}
