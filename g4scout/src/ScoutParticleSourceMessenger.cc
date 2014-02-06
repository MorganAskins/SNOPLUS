#include "ScoutParticleSourceMessenger.hh"
#include "ScoutParticleSource.hh"

#include "G4SystemOfUnits.hh"
#include "G4Geantino.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include "G4Tokenizer.hh"

ScoutParticleSourceMessenger::ScoutParticleSourceMessenger
(ScoutParticleSource* particleSource) : mParticleSource(particleSource)
{

  mParticleTable = G4ParticleTable::GetParticleTable();

  // create directory
  mGunDirectory = new G4UIdirectory("/scout/gun/");
  mGunDirectory->SetGuidance("Partticle source control commands.");

  // list available particles
  mList = new G4UIcmdWithoutParameter("/scout/gun/list", this);
  mList->SetGuidance("List available particles.");
  mList->SetGuidance(" Invoke G4ParticleTable.");

  // set particle
  mParticle = new G4UIcmdWithAString("/scout/gun/particle", this);
  mParticle->SetGuidance("Set particle to be generated.");
  mParticle->SetParameterName("mParticleName", true);
  mParticle->SetDefaultValue("geantino");
  G4String candidateList;
  G4int nParticles = mParticleTable->entries();
  for(G4int i=0; i<nParticles; i++)
    candidateList += particleTable->GetParticleName(i) + " ";
  mParticle->SetCandidates(candidateList);

  // particle direction
  mDirection = new G4UIcmdWith3Vector("/scout/gun/direction", this);
  mDirection->SetGuidance("Set momentum direction.");
  mDirection->SetParameterName("Px", "Py", "Pz", true, true);
  mDirection->SetRange("Px!=0 || Py!=0 || Pz!=0");

  // particle energy
  mEnergy = new G4UIcmdWithADoubleAndUnit("/scout/gun/energy", this);
  mEnergy->SetGuidance("Set kinetic energy.");
  mEnergy->SetParameterName("Energy", true, true);
  mEnergy->SetDefaultUnit("MeV");

  // position
  mPosition = new G4UIcmdWith3VectorAndUnit("/scout/gun/position", this);
  mPosition->SetGuidance("Set starting position of the particle.");
  mPosition->SetParameterName("X", "Y", "Z", true, true);
  mPosition->SetDefaultUnit("cm");

  // source position type
  mPositionType = new G4UIcmdWithAString("/scout/gun/type", this);
  mPositionType->SetGuidance("Set source distribution type. Point or Volume");
  mPositionType->SetParameterName("PositionType", true, true);
  mPositionType->SetDefaultValue("Point");
  mPositionType->SetCandidates("Point Volume");

  // source shape
  mShape = new G4UIcmdWithAString("/scout/gun/shape", this);
  mShape->SetGuidance("Set the source shape (Sphere or Cylinder)");
  mShape->SetParameterName("Shape", true, true);
  mShape->SetDefaultValue("NULL");
  mShape->SetCandidates("Sphere Cylinder");

  // center coordinates
  mCenter = new G4UIcmdWith3VectorAndUnit("/scout/gun/center", this);
  mCenter->SetGuidance("Set center coordinates of source");
  mCenter->SetParameterName("X", "Y", "Z", true, true);
  mCenter->SetDefaultUnit("cm");
  mCenter->SetUnitCandidates("nm um mm cm m km");

  // half height of source
  mHalfZ = new G4UIcmdWithADoubleAndUnit("/scout/gun/halfz",this);
  mHalfZ->SetGuidance("Set z half length of source.");
  mHalfZ->SetParameterName("Halfz",true,true);
  mHalfZ->SetDefaultUnit("cm");
  mHalfZ->SetUnitCandidates("nm um mm cm m km");

  // radius of source
  mRadius = new G4UIcmdWithADoubleAndUnit("/scout/gun/radius",this);
  mRadius->SetGuidance("Set radius of source.");
  mRadius->SetParameterName("Radius",true,true);
  mRadius->SetDefaultUnit("cm");
  mRadius->SetUnitCandidates("nm um mm cm m km");

  // confine to volume
  mConfine = new G4UIcmdWithAString("/scout/gun/confine",this);
  mConfine->SetGuidance("Confine source to volume (NULL to unset).");
  mConfine->SetGuidance("usage: confine VolName");
  mConfine->SetParameterName("VolName",true,true);
  mConfine->SetDefaultValue("NULL");

  // angular distribution
  mAngType = new G4UIcmdWithAString("/scout/gun/angtype",this);
  mAngType->SetGuidance("Sets angular source distribution type");
  mAngType->SetGuidance("Possible variables are: iso direction");
  mAngType->SetParameterName("AngDis",true,true);
  mAngType->SetDefaultValue("iso");
  mAngType->SetCandidates("iso direction");

  // energy distribution
  mEnergyType = new G4UIcmdWithAString("/dmx/gun/energytype",this);
  mEnergyType->SetGuidance("Sets energy distribution type");
  mEnergyType->SetGuidance("Possible variables are: Mono");
  mEnergyType->SetParameterName("EnergyDis",true,true);
  mEnergyType->SetDefaultValue("Mono");
  mEnergyType->SetCandidates("Mono");
}

ScoutParticleSourceMessenger::~ScoutParticleSourceMessenger()
{
  delete mPositionType;
  delete mShape;
  delete mCenter;
  delete mHalfZ;
  delete mRadius;
  delete mConfine;
  delete mAngType;
  delete mEnergyType;
  delete mParticle;
  delete mPosition;
  delete mDirection;
  delete mEnergy;
  delete mList;
  delete mGunDirectory;
}

void ScoutParticleSourceMessenger::SetNewValue
(G4UIcommand* command, G4String newValues)
{
  if(command == mPositionType)
    mParticleSource->SetPosType(newValues);
  else if(command == mShape)
    mParticleSource->SetPosShape(newValues);
  else if(command == mCenter)
    mParticleSource->SetCenterCoords(mCenter->GetNew3VectorValue(newValues));
  else if(command == mHalfZ)
    mParticleSource->SetHalfZ(mHalfZ->GetNewDoubleValue(newValues));
  else if(command == mRadius)
    mParticleSource->SetRadius(mRadius->GetNewDoubleValue(newValues));
  else if(command == mConfine)
    mParticleSource->ConfineSourceToVolume(newValues);
  else if(command == mAngType)
    mParticleSource->SetAngType(newValues);
  else if(command == mEnergyType)
    mParticleSource->SetEnergyType(newValues);
  else if(command == mParticle)
    mParticleSource->SetParticleDefinition( mParticleTable->FindParticle(newValues));
  else if(command == mPosition)
  {
    mParticleSource->SetPosType("Point");
    mParticleSource->SetCenterCoords(mPosition->GetNew3VectorValue(newValues));
  }
  else if(command == mDirection)
  {
    mParticleSource->SetAngType("direction");
    mParticleSource->SetParticleMomentum(mDirection->GetNew3VectorValue(newValues));
  }
  else if(command == mEnergy)
  {
    mParticleSource->SetEnergyType("Mono");
    mParticleSource->SetMonoEnergy(mEnergy->GetNewDoubleValue(newValues));
  }
  else if(command == mList)
    mParticleTable->DumpTable();
  else
    G4cout << "Error entering command" << G4endl;
}
