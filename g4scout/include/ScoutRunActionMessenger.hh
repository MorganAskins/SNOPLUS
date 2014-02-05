#ifndef SCOUTRUNACTIONMESSENGER_h
#define SCOUTRUNACTIONMESSENGER_h

#include "globals.hh"
#include "G4UImessenger.hh"

class ScoutRunAction;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

/*!
  @class ScoutRunAction
  @brief Messenger for variables
  at run time
  @author Morgan Askins
*/

class ScoutRunActionMessenger : public G4UImessenger
{
  ScoutRunAction* mRunAction;
  
public:
  ScoutRunActionMessenger(ScoutRunAction*);
  ~ScoutRunActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);
};

#endif
