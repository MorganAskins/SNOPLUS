#ifndef SCOUTEVENTACTIONMESSENGER_h
#define SCOUTEVENTACTIONMESSENGER_h

#include "globals.hh"
#include "G4UImessenger.hh"

class ScoutEventAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/*!
  @class ScoutEventActionMessenger
  @brief Controls messaging between
  the user and actions during an event
  @author Morgan Askins
*/

class ScoutEventActionMessenger : public G4UImessenger
{
  ScoutEventAction* mEventAction;
  // commands here
public:
  ScoutEventActionMessenger(ScoutEventAction*);
  ~ScoutEventActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);
};

#endif
