// Contains the geometry information for SCOUT

#ifndef ScoutDetectorConstruction_h
#define ScoutDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


/*!
  @class ScoutDetectorConstruction
  @brief Defines the materials and geometry of the
  SCOUT detector
  @author Morgan Askins
*/

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Colour;

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
  void ConstructPmt();		///< Build array of Pmts
  void PlacePmts();
  ScoutDetectorMessenger* mDetectorMessenger;
  void InitVariables();
  
public:
  ScoutDetectorConstruction();
  ~ScoutDetectorConstruction();
  
  G4VPhysicalVolume* Construct();

private:			// Materials, volumes, variables
  
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

  // colors
  G4Colour white;
  G4Colour grey;
  G4Colour lgrey;
  G4Colour red;
  G4Colour blue;
  G4Colour cyan;
  G4Colour magenta;
  G4Colour yellow;
  G4Colour orange;
  G4Colour lblue;
  G4Colour lgreen;
  G4Colour green;
  G4Colour brown;

  // dimensions
  G4double targetDimensions[3];
  G4double labDimensions[3];
  G4double worldDimensions[3];

  // pmt properties
  G4double pmtExposure;
  G4double pmtHeight;
  G4double pmtRadius;
  G4double pmtOffset;
  G4double pmtGlassThickness;
  G4ThreeVector pmtPosition;

  G4int NUM;
  G4double phcath_PP[2];
  G4double phcath_REFL[2];

};

#endif
