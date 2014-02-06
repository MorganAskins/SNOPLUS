#include "ScoutEventAction.hh"
#include "ScoutRunAction.hh"
#include "ScoutPrimaryGeneratorAction.hh"
#include "ScoutEventActionMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

ScoutEventAction::ScoutEventAction(ScoutRunAction* runAction,
				   ScoutPrimaryGeneratorAction* genAction)
  : mRunAction(runAction),mGeneratorAction(genAction)
{
  mActionMessenger = new ScoutEventActionMessenger(this);
  // will add scout digitizer here
}

ScoutEventAction::~ScoutEventAction()
{
  delete mActionMessenger;
}

void ScoutEventAction::BeginOfEventAction(const G4Event*){}

void ScoutEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "Processing event: " << event->GetEventID() << G4endl;
  FillRootEvent(1);
}

void ScoutEventAction::FillRootEvent(G4int)
{
  return;
}
