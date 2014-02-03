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
class ScoutScint;
class ScoutPMT;

// Messenger to pass information from macros
// about detector design
class ScoutDetectorMessenger;

class ScoutDetectorConstruction : public G4VUserDetectorConstruction
{
private:  
  void DefineMaterials();	///< Setup the materials list
  ScoutDetectorMessenger* pmDetectorMessenger;

  // sizes
  G4double mWorldDimensions[3];
  G4double mDetectorDimensions[3];
  
  // materials

public:
  ScoutDetectorConstruction();
  ~ScoutDetectorConstruction();

  G4VPhysicalVolume* Construct();
