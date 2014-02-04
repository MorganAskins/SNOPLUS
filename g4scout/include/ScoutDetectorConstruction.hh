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
class ScoutPMTSD;

// Messenger to pass information from macros
// about detector design
class ScoutDetectorMessenger;

class ScoutDetectorConstruction : public G4VUserDetectorConstruction
{
  ScoutScintSD* mScoutSD;
  ScoutPMTSD* mPMTSD;
  void DefineMaterials();	///< Setup the materials list
  ScoutDetectorMessenger* pmDetectorMessenger;
  
  // materials
  #include "ScoutDetectorMaterial.ihh"

  // volumes
  G4LogicalVolume* mWorldLog;
  G4PhysicalVolume* mWorldPhys;
  G4LogicalVolume* mLabLog;
  G4PhysicalVolume* mLabPhys;
  G4LogicalVolume* mTargetLog;
  G4PhysicalVolume* mTargetPhys;
  G4LogicalVolume* mPMTLog;
  G4PhysicalVolume* mPMTPhys;
  G4LogicalVolume* mPhcathLog;
  G4PhysicalVolume* mPhcathPhys;
  
public:
  ScoutDetectorConstruction();
  ~ScoutDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
};

#endif
