#include "ScoutPmtHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

#include <iomanip>

G4Allocator<ScoutPmtHit> ScoutPmtHitsAllocator;

ScoutPmtHit::ScoutPmtHit()
{
  G4double mEnergy=0;
  mPosition = G4ThreeVector(0, 0, 0);
  mDirection = G4ThreeVector(0, 0, 0);
  mPolarization = G4ThreeVector(0, 0, 0);
  mTime = 0;
}

ScoutPmtHit::~ScoutPmtHit(){}

ScoutPmtHit::ScoutPmtHit(const ScoutPmtHit& right) : G4VHit(right)
{
  mPosition = right.mPosition;
  mTime = right.mTime;
}

const ScoutPmtHit& ScoutPmtHit::operator=(const ScoutPmtHit& right)
{
  mPosition = right.mPosition;
  mTime = right.mTime;
  
  return *this;
}

int ScoutPmtHit::operator==(const ScoutPmtHit& right) const
{
  return (this==&right) ? 1 : 0;
}

void ScoutPmtHit::Draw(){;}

void ScoutPmtHit::Print(){;}
