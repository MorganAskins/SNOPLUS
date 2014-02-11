#ifndef SCOUTRUNACTION_h
#define SCOUTRUNACTION_h

#include "G4UserRunAction.hh"
#include "globals.hh"

// Root
#include "TFile.h"
#include "TTree.h"

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

  // TODO Move to another class
  TTree* mScoutTree;
  // data
  G4double mPhotoelectrons;

public:
  ScoutRunAction();
  ~ScoutRunAction();

  // Root Trees
  TTree* GetTree(){return mScoutTree;}
  void SetTree(TTree* tree){mScoutTree=tree;}

  // inherited from G4UserRunAction
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  // Set commands to save
  void SetPhotoelectrons(G4int pe){mPhotoelectrons = pe;}

};

#endif
  
