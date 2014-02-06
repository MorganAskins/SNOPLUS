#ifndef SCOUTPRIMARYGENERATORACTION_h
#define SCOUTPRIMARYGENERATORACTION_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class ScoutParticleSource;
class G4Event;

class ScoutPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  //G4GeneralParticleSource* mParticleGun
  ScoutParticleSource* mParticleGun;
  long mSeeds[2];
  G4double mPrimaryEnergy;

public:
  ScoutPrimaryGeneratorAction();
  ~ScoutPrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* anEvent);
  const long* GetEventsSeeds(){return mSeeds;}
  G4double GetPrimaryEnergy(){return mPrimaryEnergy;}

};

#endif
  
