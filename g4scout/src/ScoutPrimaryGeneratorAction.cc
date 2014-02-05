#include "ScoutPrimaryGeneratorAction.hh"
#include "ScoutParticleSource.hh"

#include "G4Event.hh"
#include "Randomize.hh"
#include "globals.hh"

ScoutPrimaryGeneratorAction::ScoutPrimaryGeneratorAction()
{
  mParticleGun = new ScoutParticleSource();

  mPrimaryEnergy = 0;
  mSeeds[0]=-1;
  mSeeds[1]=-1;
}

ScoutPrimaryGeneratorAction::~ScoutPrimaryGeneratorAction()
{
  delete mParticleGun;
}

void ScoutPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  mPrimaryEnergy = 0;
  mSeeds[0] = *CLHEP::HepRandom::getTheSeeds();
  mSeeds[1] = *(CLHEP::HepRandom::getTheSeeds()+1);

  mParticleGun->GeneratePrimaryVertex(anEvent);
  mPrimaryEnergy = particleGun->GetParticleEnergy();
}
