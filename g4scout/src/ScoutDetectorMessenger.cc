#include "ScoutDetectorMessenger.hh"
#include "ScoutDetectorConstruction.hh"

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

ScoutDetectorMessenger::ScoutDetectorMessenger(ScoutDetectorConstruction* DC)
  :mDetectorConstruction(DC)
{
  // something = new G4UIcmdWithADoubleAndUnit("/Scout/something", this);
  // something->SetGuidance("Place a description here");
  // something->SetParameterName("variable", false, false);
  // something->SetRange("variable>=250*eV");
  // something->SetDefaultUnit("eV");
  // something->SetUnitCategory("Energy");
  // something->AvailabeForStates(G4State_PreInit, G4State_Idle);
}

ScoutDetectorMessenger::~ScoutDetectorMessenger()
{
  //delete something;
}

void ScoutDetectorMessenger::SetNewValue(G4UIcommand* command
					 G4String newValue)
{
  // if(command == something)
  //   mDetectorConstruction->SetSomething(something->GetNewDoubleValue(newValue));
}
