#include "ScoutParticleSource.hh"
#include "ScoutParticleSourceMessenger.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

#include <cmath>

ScoutParticleSource::ScoutParticleSource()
{
  // Particle Properties
  mNumberOfParticles = 1;
  mParticleDefinition = NULL;
  G4ThreeVector zero(0, 0, 0);
  mParticleMomentum = G4ParticleMomentum(1, 0, 0);
  mParticleEnergy = 1*MeV;
  mParticleCharge = 0;
  mParticlePosition = zero;
  mParticlePolarization = zero;
  mParticleTime = 0;

  // Particle Position
  mSourceType = "Volume";
  mShape = "NULL";
  mHalfZ = 0;
  mRadius = 0;
  mCenterCoords = zero;
  mConfine = false;
  mVolName = "NULL";
  mAngType = "iso";
  mMinTheta = 0;
  mMaxTheta = pi;
  mMinPhi = 0;
  mMaxPhi = twopi;

  // Energy
  mEnergyType = "Mono";
  mMonoEnergy = 1*MeV;
  
  mSourceMessenger = new ScoutParticleSourceMessenger(this);
  mNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

ScoutParticleSource::~ScoutParticleSource()
{
  delete mSourceMessenger;
}

void ScoutParticleSource::GeneratePointSource()
{
  // Generates points given the point source
  if(mSourceType == "Point")
    mParticlePosition = mCenterCoords;
  else
    G4cout << "Error mSourceType is not set to Point" << G4endl;
}

void ScoutParticleSource::GeneratePointsInVolume()
{
  G4ThreeVector RandomPosition;
  G4double x=0, y=0, z=0;

  if(mSourceType != "Volume")
    G4cout << "Error mSourceType not Volume" << G4endl;

  if(mShape == "Sphere")
  {
    x = mRadius*2;
    y = mRadius*2;
    z = mRadius*2;
    while(((x*x)+(y*y)+(z*z)) > (mRadius*mRadius))
    {
      x = (2*G4UniformRand() - 1)*mRadius;
      y = (2*G4UniformRand() - 1)*mRadius;
      z = (2*G4UniformRand() - 1)*mRadius;
    }
  }
  else if(mShape == "Cylinder")
  {
    x = 2*mRadius;
    y = 2*mRadius;
    z = (2*G4UniformRand() - 1)*mHalfZ;
    while(((x*x)+(y*y)) > (mRadius*mRadius))
    {
      x = (2*G4UniformRand() - 1)*mRadius;
      y = (2*G4UniformRand() - 1)*mRadius;
    }
  }
  else
    G4cout << "Error: Volume mShape Does not Exist" << G4endl;

  RandomPosition.setX(x);
  RandomPosition.setY(y);
  RandomPosition.setZ(z);
  mParticlePosition = mCenterCoords + RandomPosition;

}

G4bool ScoutParticleSource::IsSourceConfined()
{
  // Method to check point is within the volume specified
  if(mConfine == false)
    G4cout << "Error: Confine is false" << G4endl;
  G4ThreeVector null(0.,0.,0.);
  G4ThreeVector *ptr;
  ptr = &null;

  // Check particle_position is within VolName
  G4VPhysicalVolume *theVolume;
  theVolume=gNavigator->LocateGlobalPointAndSetup(mParticlePosition,ptr,true);
  G4String theVolName = theVolume->GetName();
  if(theVolName == mVolName) {
  //   if(verbosityLevel >= 1)
  //     G4cout << "Particle is in volume " << VolName << G4endl;
    return(true);
  }
  else
    return(false);
}

void ConfineSourceToVolume(G4String vname)
{
  mVolName = vname;
  
  G4VPhysicalVolume *tempPV = NULL;
  G4PhysicalVolumeStore *PVStore = 0;
  G4String theRequiredVolumeName = mVolName;
  PVStore = G4PhysicalVolumeStore::GetInstance();
  
  G4int counter = 0;
  G4bool found = false;
  while(!found && counter<(G4int)PVStore->size())
  {
    tempPV = (*PVStore)[counter];
    found = (tempPV->GetName() == theRequiredVolumeName);
    if(!found)
      counter++;
  }
  if(found == true)
    mConfine = true;
  else if(mVolName == "NULL")
    mConfine = false;
  else
  {
    G4cout << " *** Error: Volume does not exist *** " << G4endl;
    mVolName = "NULL";
    mConfine = false;
  }
}

void ScoutParticleSource::GeneratoeIsotropicFlux()
{
  G4double random1, random2;
  G4double px, py, pz;
  G4doubel sintheta, sinphi, costheta, cosphi;

  random1 = G4UniformRand();
  costheta = std::cos(mMinTheta) - random1*(std::cos(mMinTheta) - std::cos(mMaxTheta));
  sintheta = std::sqrt(1 - costheta*costheta);

  random2 = G4UniformRand();
  mPhi = mMinPhi + (mMaxPhi - mMinPhi)*random2;
  sinphi = std::sin(mPhi);
  cosphi = std::cos(mPhi);

  px = -sintheta*cosphi;
  py = -sintheta*sinphi;
  pz = -costheta;

  G4double ResMag = std::sqrt((px*px)+(py*py)+(pz*pz));
  px = px/ResMag;
  py = py/ResMag;
  pz = pz/ResMag;

  mParticleMomentum.setX(px);
  mParticleMomentum.setY(py);
  mParticleMomentum.setZ(pz);
}

void ScoutParticleSource::SetParticleDefinition(G4ParticleDefinition* defintion)
{
  mParticleDefinition = definition;
  mParticleCharge = definition->GetPDGCharge();
}



void ScoutParticleSource::GeneratePrimaryVertex(G4Event* event)
{
  if(mParticleDefinition == NULL)
  {
    G4cout << "No particle has been defined" << G4endl;
    return;
  }

  // Position
  G4bool sourceconf = false;
  G4int loopCount = 0;

  while(sourceconf = false && (loopCount++)<10000)
  {
    if(SourceType == "Point")
      GeneratePointSource();
    else if(SourceType == "Volume")
      GeneratePointsInVolume();
    if(mConfine == true)
      sourceconf = IsSourceConfined();
    else
      sourceconf = true;
    loopCount++;
  }
    
  // Angular components
  if(mAngType == "iso")
    GenerateIsotropicFlux();
  else if(mAngType == "direction")
    SetParticleMomentum(mParticleMomentum);
  else
    G4cout << "Error: mAngType has unusual value: " << mAngType << G4endl;

  // Energy
  if(mEnergyType == "Mono")
    GenerateMonoEnergetic();
  else
    G4cout << "Error: mEnergyType has unusual value: " << mEnergyType << G4endl;

  // TODO add verbosity and prints of particle information
  
  Grdouble mass = mParticleDefinition->GetPDGMass();
  G4double energy = mParticleEnergy + mass;
  G4double pmom = std::sqrt(energy*energy - mass*mass);
  G4double p[3] = {pmom*mParticleMomentum.x(), pmom*mParticleMomentum.y(), 
		   pmom*mParticleMomentum.z()};
  // Now actually generate the particles using a G4 particle
  G4PrimaryVertex* vertex = new G4PrimaryVertex(mParticlePosition, mParticleTime);
  for(G4int i=0; i<mNumberOfParticles; ++i)
  {
    G4PrimaryParticle* particle = new G4PrimaryParticle(mParticleDefinition, p[0], p[1], p[2]);
    particle->SetMass(mass);
    particle->SetCharge(mParticleCharge);
    particle->SetPolarization(mParticlePolarization.x(), mParticlePolarization.y(),
			      mParticlePolarization.z());
    vertex->SetPrimary(particle);
  }
  event->AddPrimaryVertex(vertex);
}
    
