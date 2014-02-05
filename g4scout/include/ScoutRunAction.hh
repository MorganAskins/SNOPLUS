#ifndef SCOUTRUNACTION_h
#define SCOUTRUNACTION_h

#include "G4UserRunAction.hh"
#include "globals.hh"

/*!
  @class ScoutRunAction
  @brief Tell the simulation what to save at the
  beginning and end of a run
  @author Morgan Askins
*/

class ScoutRunActionMessenger;
class G4Run;

class ScoutRunAction : public G4UserRunAction
{
  // Messenger members
  ScoutRunActionMessenger* mRunMessenger;

public:
  ScoutRunAction();
  ~ScoutRunAction();

  // inherited from G4UserRunAction
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  // Set commands to save
  
};

#endif
  
