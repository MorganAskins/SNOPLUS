#ifndef SCOUTSCINTSD_h
#define SCOUTSCINTSD_h

#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include "ScoutScintHit.hh"

class ScoutDetectorConstruction;
class G4Step;
class G4HCofThisEvent;

/*!
  @class ScoutScintSD
  @brief Defines the sensitive detector of SCOUT
  @author Morgan Askins
*/

class ScoutScintSD : public G4VSensitiveDetector
{
  ScoutScintHitsCollection* mScintillatorCollection;
  ScoutDetectorConstruction* mScoutDetector;
  G4int mHitID;
public:
  ScoutScintSD(G4String, ScoutDetectorConstruction*);
  ~ScoutScintSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void clear() {}
  void DrawAll() {}
  void PrintAll() {}
};

#endif
