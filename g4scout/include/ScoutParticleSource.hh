#ifndef ScoutParticleSource_h
#define ScoutParticleSource_h

#include "G4VPrimaryGenerator.hh"
#include "G4Navigator.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"

#include "ScoutParticleSourceMessenger.hh"

/*!
  @class ScoutParticleSource
  @brief Define particle guns and vector files
  to read from for physics generation
  @author Morgan Askins
*/

class ScoutParticleSource : public G4VPrimaryGenerator
{
  // Particle Properties
  G4int mNumberOfParticles;
  G4ParticleDefinition* mParticleDefinition;
  G4ParticleMomentum mParticleMomentum;
  G4double mParticleEnergy;
  G4double mParticleCharge;
  G4ThreeVector mParticlePosition;
  G4ThreeVector mParticlePolarization;
  G4double mParticleTime;

  // position distribution
  G4String mSourceType;
  G4String mShape;
  G4double mHalfZ;
  G4double mRadius;
  G4ThreeVector mCenterCoords;
  G4bool mConfine;
  G4String mVolName;
  G4String mAngType;
  G4double mMinTheta, mMaxTheta, mMinPhi, mMaxPhi;
  G4double mTheta, mPhi;
  G4String mEnergyType;
  G4double mMonoEnergy;

  ScoutParticleSourceMessenger *mSourceMessenger;
  G4Navigator *mNavigator;

public:
  ScoutParticleSource();
  ~ScoutParticleSource();
  void GeneratePrimaryVertex(G4Event *event);

  // position distribution
  void SetSourceType(G4String SourceType){mSourceType = SourceType;}
  void SetShape(G4String aShape){mShape = aShape;}
  void SetCenterCoords(G4ThreeVector coords){mCenterCoords = coords;}
  void SetHalfZ(G4double halfz){mHalfZ = halfz;}
  void SetRadius(G4double radius){mRadius = radius;}
  void GeneratePointSource();
  void GeneratePointsInVolume();
  G4bool IsSourceConfined();
  void ConfineSourceToVolume(G4String);

  // angular distribution
  void SetAngType(G4String type){mAngType = type;}
  void SetParticleMomentum(G4ParticleMomentum mom){mParticleMomentum = mom.unit();}
  void GenerateIsotropicFlux();

  // energy distribution
  void SetEnergyType(G4String type){mEnergyType = type;}
  void SetMonoEnergy(G4double energy){mMonoEnergy = energy;}
  void GenerateMonoEnergetic();
  inline G4double GetParticleEnergy(){return mParticleEnergy;}

  // particle properties
  void SetParticleDefinition(G4ParticleDefinition* aParticleDefinition);
  inline void SetParticleCharge(G4double aCharge){mParticleCharge = aCharge;}
};

#endif
