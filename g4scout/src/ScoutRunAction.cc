#include "ScoutRunAction.hh"
#include "ScoutRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include <H5Cpp.h>

ScoutRunAction::ScoutRunAction()
{
  mRunMessenger = new ScoutRunActionMessenger(this);

  file = new H5::H5File("output.h5", H5F_ACC_TRUNC);
}

ScoutRunAction::~ScoutRunAction()
{
  delete mRunMessenger;
  delete file;
}

void ScoutRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void ScoutRunAction::EndOfRunAction(const G4Run* aRun)
{
}

void ScoutRunAction::ClearLastEvent()
{
  mPmtID.clear();
  mEnergy.clear();
  mPosition.clear();
  mDirection.clear();
  mPolarization.clear();
  mTime.clear();
  return;
}
