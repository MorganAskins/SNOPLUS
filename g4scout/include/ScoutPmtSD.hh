#ifndef SCOUTPMTSD_h
#define SCOUTPMTSD_h

#include <map>

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "ScoutPmtHit.hh"

class ScoutDetectorConstruction;
class G4Step;
class G4HofThisEvent;

/*!
  @class ScoutPmtSD
  @brief Describe pmts as sensitive
  detectors
  @author Morgan Askins
*/

class ScoutPmtSD : public G4VSensitiveDetector
{
  ScoutPmtHitsCollection* mPmtCollection;
  ScoutDetectorConstruction* mScoutDetector;
  G4int mHitID;
  std::map<int, int> mPmtHitMap;

public:
  ScoutPmtSD(G4String, ScoutDetectorConstruction*);
  ~ScoutPmtSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void clear();
  void DrawAll();
  void PrintAll();
};

#endif
