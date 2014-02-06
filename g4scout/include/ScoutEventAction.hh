#ifndef SCOUTEVENTACTION_h
#define SCOUTEVENTACTION_h

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "G4ios.hh"

#include "ScoutScintHit.hh"
#include "ScoutPmtHit.hh"

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

public:
  ScoutEventAction(ScoutRunAction*, ScoutPrimaryGeneratorAction*);
  ~ScoutEventAction();

  // inherited from G4UserEventAction
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  // Specific scout methods
  void FillRootEvent(G4int);
};

#endif
