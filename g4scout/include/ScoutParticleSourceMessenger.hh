#ifndef SCOUTPARTICLESOURCEMESSENGER_h
#define SCOUTPARTICLESOURCEMESSENGER_h

#include "G4UImessenger.hh"
#include "globals.hh"

class ScoutParticleSource;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

/*!
  @class ScoutParticleSourceMessenger
  @brief Relay commands from the .mac
  to the particle generator
  @author Morgan Askins
*/

class ScoutParticleSourceMessenger : public G4UImessenger
{
  ScoutParticleSource* mParticleSource;
  G4ParticleTable* mParticleTable;

  // top level directory for particle gun
  G4UIdirectory* mGunDirectory;
  // components of a particle gun
  G4UIcmdWithAString* mPositionType;
  G4UIcmdWithAString* mShape;
  G4UIcmdWith3VectorAndUnit* mCenter;
  G4UIcmdWithADoubleAndUnit* mHalfZ;
  G4UIcmdWithADoubleAndUnit* mRadius;
  G4UIcmdWithAString* mConfine;
  G4UIcmdWithAString* mAngType;
  G4UIcmdWithAString* mEnergyType;
  G4UIcmdWithAString* mParticle;
  G4UIcmdWith3VectorAndUnit* mPosition;
  G4UIcmdWith3Vector* mDirection;
  G4UIcmdWithADoubleAndUnit* mEnergy;
  G4UIcmdWithoutParameter* mList;
  
public:
  ScoutParticleSourceMessenger(ScoutParticleSource* particleSource);
  ~ScoutParticleSourceMessenger();
  
  void SetNewValue(G4UIcommand* command, G4String newValues);
};

#endif
