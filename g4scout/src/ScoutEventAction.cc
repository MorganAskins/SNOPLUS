#include "ScoutEventAction.hh"
#include "ScoutRunAction.hh"
#include "ScoutPrimaryGeneratorAction.hh"
#include "ScoutEventActionManager.hh"

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
  mActionManager = new ScoutEventActionManager(this);
  // will add scout digitizer here
}

ScoutEventAction::~ScoutEventAction()
{
  delete ScoutEventActionManager;
}

ScoutEventAction::BeginOfEventAction(const G4Event* event){}

ScoutEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "Processing event: " << event->GetEventID() << G4endl;
  FillRootEvent(1);
}

void ScoutEventAction::FillRootEvent(G4int evt)
{
  return;
}
