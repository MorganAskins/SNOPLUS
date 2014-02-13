#include "ScoutScintSD.hh"

#include "ScoutScintHit.hh"
#include "ScoutDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Ions.hh"
#include "G4ios.hh"

ScoutScintSD::ScoutScintSD(G4String name, ScoutDetectorConstruction* ScoutSD)
  :G4VSensitiveDetector(name), mScoutDetector(ScoutSD)
{
  G4String HCname="mScintillatorCollection";
  collectionName.insert(HCname);
}

ScoutScintSD::~ScoutScintSD(){}

void ScoutScintSD::Initialize(G4HCofThisEvent*)
{
  mScintillatorCollection = new ScoutScintHitsCollection(SensitiveDetectorName, collectionName[0]);
  mHitID=-1;
}

G4bool ScoutScintSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  G4String particleName = particleType->GetParticleName();

  G4double stepl = 0;
  if(particleType->GetPDGCharge() != 0)
    stepl = aStep->GetStepLength();

  G4VPhysicalVolume* particleVolume = aStep->GetTrack()->GetNextVolume();
  if( particleVolume->GetName() != "mLabPhys" ) return false;

  //if ((edep==0)&&(stepl==0)) return false;
  // Only care about photons
  if( particleName != "opticalphoton" && aStep->GetTrack()->GetTrackID() != 1)
    return false;

  // fill in hit
  ScoutScintHit* newHit = new ScoutScintHit();
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());
  newHit->SetParticle(particleName);
  newHit->SetParticleEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());

  mHitID = mScintillatorCollection->insert(newHit);

  return true;
}

void ScoutScintSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4String HCname = collectionName[0];
  static G4int HCID = -1;
  if(HCID<0)
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
  HCE->AddHitsCollection(HCID, mScintillatorCollection);

  G4int nHits = mScintillatorCollection->entries();
  if(verboseLevel>=1)
    G4cout << "  Scintillator Collection: " << nHits << " hits" << G4endl;
  if(verboseLevel>=2)
    mScintillatorCollection->PrintAllHits();
}
