#ifndef SCOUTEVENTACTION_h
#define SCOUTEVENTACTION_h

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "G4ios.hh"

#include "ScoutScintHit.hh"
#include "ScoutPmtHit.hh"

#include "TTree.h"
#include "TFile.h"

class TTree;
class TFile;
class ScoutRunAction;
class ScoutPrimaryGeneratorAction;
class ScoutEventActionMessenger;

/*!
  @class ScoutEventAction
  @brief Describe scout events during
  a run
  @author Morgan Askins
*/

class ScoutEventAction : public G4UserEventAction
{
  ScoutRunAction* mRunAction;
  ScoutPrimaryGeneratorAction* mGeneratorAction;
  ScoutEventActionMessenger* mActionMessenger;

  TTree* mScoutTree;

public:
  ScoutEventAction(ScoutRunAction*, ScoutPrimaryGeneratorAction*);
  ~ScoutEventAction();

  // inherited from G4UserEventAction
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  // Hits collection
  G4int mScintillatorCollID;
  G4int mPmtCollID;

  // Specific scout methods
  void FillRootEvent(G4int);
  void FillHDF5Event(G4int);
};

#endif
