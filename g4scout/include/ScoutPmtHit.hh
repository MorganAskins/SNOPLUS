#ifndef SCOUTPMTHIT_h
#define SCOUTPMTHIT_h

#include "G4VPhysicalVolume.hh"
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
  G4double mEnergy;
  G4ThreeVector mPosition;
  G4ThreeVector mDirection;
  G4ThreeVector mPolarization;
  G4double mTime;
  G4int mTubeID;

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

  inline void SetEnergy(G4double energy){mEnergy=energy;}
  inline G4double GetEnergy(){return mEnergy;}

  inline void SetPos(G4ThreeVector vector){mPosition=vector;}
  inline G4ThreeVector GetPos() const {return mPosition;}

  inline void SetDir(G4ThreeVector vector){mDirection=vector;}
  inline G4ThreeVector GetDir() const {return mDirection;}

  inline void SetPol(G4ThreeVector vector){mPolarization=vector;}
  inline G4ThreeVector GetPol() const {return mPolarization;}

  inline void SetTime(G4double time){mTime = time;}
  inline G4double GetTime(){return mTime;}

  inline void SetTubeID(G4int aID){mTubeID=aID;}
  inline G4int GetTubeID(){return mTubeID;}

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
