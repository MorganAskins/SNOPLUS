// Contains the geometry information for SCOUT

#ifndef ScoutDetectorConstruction_h
#define ScoutDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <vector>

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
class G4OpticalSurface;

class G4UserLimits;

// Scintillator and PMT descriptions
class ScoutScintSD;
class ScoutPmtSD;

// Messenger to pass information from macros
// about detector design
class ScoutDetectorMessenger;
class ScoutPmtInfo;

class ScoutDetectorConstruction : public G4VUserDetectorConstruction
{
  ScoutScintSD* mScoutSD;
  ScoutPmtSD* mPmtSD;
  std::vector<ScoutPmtInfo*> mPmtInfoCollection;
  void DefineMaterials();	///< Setup the materials list
  void ConstructPmt(G4int);	///< Build array of Pmts
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
  G4LogicalVolume* mLeadLog;
  G4VPhysicalVolume* mLeadPhys;
  G4LogicalVolume* mBpolyLog;
  G4VPhysicalVolume* mBpolyPhys;
  G4LogicalVolume* mCopperLog;
  G4VPhysicalVolume* mCopperPhys;
  G4LogicalVolume* mAcrylicLog;
  G4VPhysicalVolume* mAcrylicPhys;
  G4LogicalVolume* mTargetLog;
  G4VPhysicalVolume* mTargetPhys;
  // These volumes are duplicated to num pmts
  G4LogicalVolume* mPmtHolderLog[18];
  G4VPhysicalVolume* mPmtHolderPhys[18];
  G4LogicalVolume* mPmtHolderFillLog[18];
  G4VPhysicalVolume* mPmtHolderFillPhys[18];
  G4LogicalVolume* mPmtLog[18];
  G4VPhysicalVolume* mPmtPhys[18];
  G4LogicalVolume* mPhcathLog[18];
  G4VPhysicalVolume* mPhcathPhys[18];
  G4LogicalVolume* mPhcathStopLog[18];
  G4VPhysicalVolume* mPhcathStopPhys[18];

  // Optical Surface
  G4OpticalSurface* mShinySurface;

  // colors
  G4Colour white = G4Colour(1.0, 1.0, 1.0);
  G4Colour grey = G4Colour(0.5, 0.5, 0.5);
  G4Colour lgrey = G4Colour(.85, .85, .85);
  G4Colour red = G4Colour(1.0, 0.0, 0.0);
  G4Colour blue = G4Colour(0.0, 0.0, 1.0);
  G4Colour cyan = G4Colour(0.0, 1.0, 1.0);
  G4Colour magenta = G4Colour(1.0, 0.0, 1.0);
  G4Colour yellow = G4Colour(1.0, 1.0, 0.0);
  G4Colour orange = G4Colour(.75, .55, 0.0);
  G4Colour lblue = G4Colour(0.0, 0.0, .75);
  G4Colour lgreen = G4Colour(0.0, .75, 0.0);
  G4Colour green = G4Colour(0.0, 1.0, 0.0);
  G4Colour brown = G4Colour(0.7, 0.4, 0.1);

  // pmt properties
  G4double pmtExposure=18*cm;
  G4double pmtHeight=15*cm;
  G4double pmtRadius=10*cm;
  G4double pmtOffset=5*cm;
  G4double pmtStandOff=10*cm;
  G4double pmtGlassThickness=5*mm;

  G4double acrylicThickness = 1*cm;
  G4double copperThickness = 1*cm;
  G4double bpolyThickness = 2*cm;
  G4double leadThickness = 10*cm;

  // dimensions -- use c++11 standard
  // volumes, in order from inner to outer:
  // target, acrylic, pmt vessel w/ pmts, copper,
  // borated poly, lead, lab, world
  G4double targetDimensions[3]={0.5*m, 0.5*m, 0.5*m};
  G4double acrylicDimensions[3]={targetDimensions[0]+acrylicThickness,
				 targetDimensions[1]+acrylicThickness,
				 targetDimensions[2]+acrylicThickness};
  G4double pmtHolderDimensions[3]={acrylicDimensions[0]/3.,
				   acrylicDimensions[1]/3.,
				   pmtHeight/2.+pmtOffset+pmtStandOff+2*cm+acrylicThickness};
  G4double pmtHolderFillDimensions[3]={pmtHolderDimensions[0]-acrylicThickness,
				       pmtHolderDimensions[1]-acrylicThickness,
				       pmtHolderDimensions[2]-acrylicThickness};
  G4double pmtHolderZoffset=acrylicDimensions[2]+pmtHolderDimensions[2];
  G4double copperDimensions[3]={acrylicDimensions[0]+copperThickness,
				acrylicDimensions[1]+copperThickness,
				acrylicDimensions[2]+2*pmtHolderDimensions[2]+copperThickness};
  G4double bpolyDimensions[3]={copperDimensions[0]+bpolyThickness,
			       copperDimensions[1]+bpolyThickness,
			       copperDimensions[2]+bpolyThickness};
  G4double leadDimensions[3]={bpolyDimensions[0]+leadThickness,
			      bpolyDimensions[1]+leadThickness,
			      bpolyDimensions[2]+leadThickness};
  G4double labDimensions[3]={leadDimensions[0]+1*m,
			     leadDimensions[0]+1*m,
			     leadDimensions[0]+1*m};
  G4double worldDimensions[3]={labDimensions[0]+50*cm,
			       labDimensions[1]+50*cm,
			       labDimensions[2]+50*cm};

  G4int NUM=2;
  G4double phcath_PP[2]={6.00*eV, 7.50*eV};
  G4double phcath_REFL[2]={0.0, 0.0};

};

#endif
