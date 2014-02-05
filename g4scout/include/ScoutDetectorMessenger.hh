#ifndef SCOUTDETECTORMESSENGER_h
#define SCOUTDETECTORMESSENGER_h

#include "globals.hh"
#include "G4UImessenger.hh"

class ScoutDetectorConstruction;
class G4UIdirectory;

class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

/*!
  @class ScoutDetectorMessenger
  @brief Relay commands between geant4 and
  the macro files
  @author Morgan Askins
*/

class ScoutDetectorMessenger : public G4UImessenger
{
  ScoutDetectorConstruction* mDetectorConstruction;
  //G4UIcmdWithADoubleAndUnit* something;

public:
  ScoutDetectorMessenger(ScoutDetectorConstruction*);
  ~ScoutDetectorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);

};

#endif
