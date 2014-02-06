#include "ScoutScintHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

G4Allocator<ScoutScintHit> ScoutScintHitAllocator;

ScoutScintHit::ScoutScintHit()
{
  mEdep = 0;
  mPos = G4ThreeVector(0, 0, 0);
  mTime = 0;
  mParticleEnergy = 0;
}

ScoutScintHit::~ScoutScintHit(){}

ScoutScintHit::ScoutScintHit(const ScoutScintHit& right):G4VHit(right)
{
  mEdep = right.mEdep;
  mPos = right.mPos;
  mTime = right.mTime;
  mParticleName = right.mParticleName;
  mParticleEnergy = right.mParticleEnergy;
}

const ScoutScintHit& ScoutScintHit::operator=(const ScoutScintHit& right)
{
  mEdep = right.mEdep;
  mPos = right.mPos;
  mTime = right.mTime;
  mParticleName = right.mParticleName;
  mParticleEnergy = right.mParticleEnergy;
  return *this;
}

int ScoutScintHit::operator==(const ScoutScintHit& right) const
{
  return (this==&right) ? 1 : 0;
}

void ScoutScintHit::Draw(){}

void ScoutScintHit::Print()
{
  G4cout << " Scout hit ENERGY: " << std::setw(5) << G4BestUnit(mEdep, "Energy")
	 << ", at " << G4BestUnit(mPos, "Length") << G4endl;
}
