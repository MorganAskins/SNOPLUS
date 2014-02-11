#include "ScoutRunAction.hh"
#include "ScoutRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"

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
  
  TFile* hfile = new TFile("testing.root", "RECREATE", "Scout ROOT file");
  hfile->SetCompressionLevel(0);

  // Event Tree
  TTree* tree = new TTree("ScoutTree", "Scout Data Tree");
  SetTree(tree);

  tree->Branch("photoelectrons", &mPhotoelectrons, "photoelectrons/D");
}

void ScoutRunAction::EndOfRunAction(const G4Run* aRun)
{
  TFile* hfile = mScoutTree->GetCurrentFile();
  hfile->Close();
}
