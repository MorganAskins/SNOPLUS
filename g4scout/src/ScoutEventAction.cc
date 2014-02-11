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

#include "TFile.h"
#include "TTree.h"

ScoutEventAction::ScoutEventAction(ScoutRunAction* runAction,
				   ScoutPrimaryGeneratorAction* genAction)
  : mRunAction(runAction),mGeneratorAction(genAction)
{
  mActionMessenger = new ScoutEventActionMessenger(this);
  // will add scout digitizer here

  //mScoutTree = mRunAction->GetTree();

  mScintillatorCollID = -1;
  mPmtCollID = -1;
}

ScoutEventAction::~ScoutEventAction()
{
  delete mActionMessenger;
}

void ScoutEventAction::BeginOfEventAction(const G4Event*)
{
  if(mScintillatorCollID == -1)
  {
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    mScintillatorCollID = SDman->GetCollectionID("mScintillatorCollection");
  }
  if(mPmtCollID == -1)
  {
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    mPmtCollID = SDman->GetCollectionID("mPmtCollection");
  }
}

void ScoutEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "Processing event: " << event->GetEventID() << G4endl;
  FillRootEvent(1);

  ScoutScintHitsCollection* SHC = NULL;
  ScoutPmtHitsCollection* PHC = NULL;
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(HCE)
  {
    SHC = (ScoutScintHitsCollection*)(HCE->GetHC(mScintillatorCollID));
    PHC = (ScoutPmtHitsCollection*)(HCE->GetHC(mPmtCollID));
  }

  // scintillator -- TODO
  mRunAction->SetPhotoelectrons(0);

  // pmt
  if(PHC)
  {
    mRunAction->SetPhotoelectrons(PHC->entries());
  }
  
  mScoutTree = mRunAction->GetTree();
  mScoutTree->Fill();
  TFile* hfile=mScoutTree->GetCurrentFile();
  hfile->Write("", TObject::kOverwrite);

}

void ScoutEventAction::FillRootEvent(G4int)
{
  return;
}
