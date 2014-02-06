#ifndef SCOUTPMTHIT_h
#define SCOUTPMTHIT_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

/*!
  @class ScoutPmtHit
  @brief Describe a pmt being hit with an
  optical photon
  @author Morgan Askins
*/

class ScoutPmtHit : public G4VHit
{
  G4ThreeVector mPosition;
  G4double mTime;

public:
  ScoutPmtHit();
  ~ScoutPmtHit();

  ScoutPmtHit(const ScoutPmtHit&);
  const ScoutPmtHit& operator=(const ScoutPmtHit&);
  int operator==(const ScoutPmtHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  void Draw();
  void Print();

  inline void SetPos(G4ThreeVector vector){mPosition=vector;}
  inline G4ThreeVector GetPos() const {return mPosition;}
  inline void SetTime(G4double time){mTime = time;}
  inline G4double GetTime(){return mTime;}

};

typedef G4THitsCollection<ScoutPmtHit> ScoutPmtHitsCollection;

extern G4Allocator<ScoutPmtHit> ScoutPmtHitsAllocator;

inline void* ScoutPmtHit::operator new(size_t)
{
  void* aHit;
  aHit =(void*)ScoutPmtHitsAllocator.MallocSingle();
  return aHit;
}

inline void ScoutPmtHit::operator delete(void* aHit)
{
  ScoutPmtHitsAllocator.FreeSingle((ScoutPmtHit*) aHit);
}

#endif
  
