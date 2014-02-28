#ifndef SCOUTRUNACTION_h
#define SCOUTRUNACTION_h

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"
// HDF5
#include <H5Cpp.h>

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

  // hdf5 -- create a dataspace for each variable to be stored
  H5::H5File* file;

  // Todo move data handling to another class
  static const G4int numpmts=18;
  std::vector<G4int> mPmtID;
  std::vector<G4double> mEnergy;
  std::vector<G4ThreeVector> mPosition;
  std::vector<G4ThreeVector> mDirection;
  std::vector<G4ThreeVector> mPolarization;
  std::vector<G4double> mTime;
  G4int mPmthits[numpmts];

public:
  ScoutRunAction();
  ~ScoutRunAction();

  // inherited from G4UserRunAction
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  // Set commands to save
  void AddPmtID(G4int ID){mPmtID.push_back(ID);}
  void AddEnergy(G4double energy){mEnergy.push_back(energy);}
  void AddPosition(G4ThreeVector pos){mPosition.push_back(pos);}
  void AddDirection(G4ThreeVector dir){mDirection.push_back(dir);}
  void AddPolarization(G4ThreeVector pol){mPolarization.push_back(pol);}
  void AddTime(G4double time){mTime.push_back(time);}

  H5::H5File* GetFile(){return file;}
  std::vector<G4double>* GetEnergy(){return &mEnergy;}
  std::vector<G4ThreeVector>* GetPosition(){return &mPosition;}
  std::vector<G4int>* GetPmtID(){return &mPmtID;}
  std::vector<G4ThreeVector>* GetDirection(){return &mDirection;}
  std::vector<G4ThreeVector>* GetPolarization(){return &mPolarization;}
  std::vector<G4double>* GetTime(){return &mTime;}
};

#endif
  
