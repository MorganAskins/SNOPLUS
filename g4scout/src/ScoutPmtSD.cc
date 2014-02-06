#include "ScoutPmtSD.hh"
#include "ScoutDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

ScoutPmtSD::ScoutPmtSD(G4String name, ScoutDetectorConstruction* detector)
  :G4VSensitiveDetector(name), mScoutDetector(detector)
{
  G4String HCname = "pmtCollection";
  collectionName.insert(HCname);
}

ScoutPmtSD::~ScoutPmtSD(){;}

void ScoutPmtSD::Initialize(G4HCofThisEvent*)
{
  mPmtCollection = new ScoutPmtHitsCollection(SensitiveDetectorName, collectionName[0]);
  mHitID = -1;
}

G4bool ScoutPmtSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  ScoutPmtHit* aPmtHit = new ScoutPmtHit();
  aPmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  aPmtHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
  mHitID=mPmtCollection->insert(aPmtHit);

  return true;
}

void ScoutPmtSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4String HCname = collectionName[0];
  
  static G4int HCID = -1;
  if(HCID<0)
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID, mPmtCollection);

  //G4int nHits = mPmtCollection->entries();
}

void ScoutPmtSD::clear() {;}
void ScoutPmtSD::DrawAll() {;}
void ScoutPmtSD::PrintAll() {;}
