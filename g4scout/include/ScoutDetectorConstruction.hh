// Contains the geometry information for SCOUT

#ifndef ScoutDetectorConstruction_h
#define ScoutDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

/*!
  @class ScoutDetectorConstruction
  @brief Defines the materials and geometry of the
  SCOUT detector
  @author Morgan Askins
*/

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class G4UserLimits;

// Scintillator and PMT descriptions
class ScoutScintSD;
class ScoutPmtSD;

// Messenger to pass information from macros
// about detector design
class ScoutDetectorMessenger;

class ScoutDetectorConstruction : public G4VUserDetectorConstruction
{
  ScoutScintSD* mScoutSD;
  ScoutPmtSD* mPmtSD;
  void DefineMaterials();	///< Setup the materials list
  ScoutDetectorMessenger* mDetectorMessenger;
  
  // materials
  #include "ScoutDetectorMaterial.ihh"

  // volumes
  G4LogicalVolume* mWorldLog;
  G4VPhysicalVolume* mWorldPhys;
  G4LogicalVolume* mLabLog;
  G4VPhysicalVolume* mLabPhys;
  G4LogicalVolume* mTargetLog;
  G4VPhysicalVolume* mTargetPhys;
  G4LogicalVolume* mPmtLog;
  G4VPhysicalVolume* mPmtPhys;
  G4LogicalVolume* mPhcathLog;
  G4VPhysicalVolume* mPhcathPhys;
  
public:
  ScoutDetectorConstruction();
  ~ScoutDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
};

#endif
