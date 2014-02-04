#ifndef SCOUTSCINTHIT_h
#define SCOUTSCINTHIT_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/*!
  @class ScoutScintHit
  @brief Contains the information for a pmt hit
  in the scout detector
  @author Morgan Askins
*/

class ScoutScintHit : public G4VHit
{
  G4double mEdep;
  G4ThreeVector mPos;
  G4double mTime;
  G4String mParticleName;
  G4double mParticleEnergy;
public:
  ScoutScintHit();
  ScoutScintHit(const ScoutScintHit&);
  ~ScoutScintHit();
  const ScoutScintHit& operator=(const ScoutScintHit&);
  int operator==(const ScoutScintHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void Draw();
  void Print();

  void SetEdep(G4double aEdep){mEdep=aEdep;}
  void SetPos(G4ThreeVector aPos){mPos=aPos;}
  void SetParticle(G4String name){mParticleName=name;}
  void SetParticleEnergy(G4double energy){mParticleEnergy=energy;}
  void SetTime(G4double time){mTime=time;}

  G4double GetEdep(){return mEdep;}
  G4ThreeVector GetPos(){return mPos;}
  G4double GetTime(){return mTime;}
  G4String GetParticle(){return mParticleName;}
  G4double GetParticleEnergy(){return mParticleEnergy;}

};

typedef G4THitsCollection<ScoutScintHit> ScoutScintHitsCollection;
extern G4Allocator<ScoutScintHit> ScoutScintHitAllocator;

inline void* ScoutScintHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)ScoutScintHitAllocator.MallocSingle();
  return aHit;
}

inline void ScoutScintHit::operator delete(void* aHit)
{
  ScoutScintHitAllocator.FreeSingle((ScoutScintHit*)aHit);
}

#endif
