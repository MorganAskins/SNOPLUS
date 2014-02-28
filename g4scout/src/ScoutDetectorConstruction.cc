// Scout Detector Design
#include <string>

#include "ScoutDetectorConstruction.hh"
#include "ScoutDetectorMessenger.hh"
#include "ScoutScintSD.hh"
#include "ScoutPmtSD.hh"
#include "ScoutPmtInfo.hh"

#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Polycone.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4FieldManager.hh"
#include "G4UniformElectricField.hh"
#include "G4TransportationManager.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"

#include "G4RunManager.hh"

#include "ScoutDetectorConstruction.hh"
//#include "ScoutDetectorPmt.icc"

ScoutDetectorConstruction::ScoutDetectorConstruction()
{
  mDetectorMessenger = new ScoutDetectorMessenger(this);

  InitVariables();
}

ScoutDetectorConstruction::~ScoutDetectorConstruction()
{
  delete mDetectorMessenger;
}

void ScoutDetectorConstruction::DefineMaterials()
{
//#include "ScoutDetectorMaterial.icc"
#include "ScoutDetectorMaterialSimple.icc"
}

G4VPhysicalVolume* ScoutDetectorConstruction::Construct()
{
  DefineMaterials();
  G4cout << "CONSTRUCTING THE GEOMETRY" << G4endl;
  // List of volumes are as follows (in order):
  // World, Target
  // Note: G4 Objects use radii (even for boxes)

  // This is the world the detector lives in
  G4Box* world_box = new G4Box("world_box", worldDimensions[0],
			       worldDimensions[1], worldDimensions[2] );
  mWorldLog = new G4LogicalVolume(world_box, world_mat, "mWorldLog");
  mWorldPhys = new G4PVPlacement(0 /*rotation*/, G4ThreeVector(0, 0, 0),
				 "mWorldPhys", mWorldLog, NULL/*mother physical*/,
				 false /*pMany*/, 0 /*pCopy*/);
  // Lab Space -- AIR
  G4Box* lab_box = new G4Box("lab_box", labDimensions[0],
			     labDimensions[1], labDimensions[2] );
  mLabLog = new G4LogicalVolume(lab_box, lab_mat, "mLabLog");
  mLabPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
			       "mLabPhys", mLabLog, mWorldPhys, false, 0);

  ///////Detector Assembly////////////
  // Lead Shield
  G4Box* lead_box = new G4Box("lead_box", leadDimensions[0], 
			      leadDimensions[1], leadDimensions[2]);
  mLeadLog = new G4LogicalVolume(lead_box, lead_mat, "mLeadLog");
  mLeadPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				"mLeadPhys", mLeadLog, mLabPhys, false, 0);
  G4VisAttributes* LeadVisAt = new G4VisAttributes(grey);
  LeadVisAt->SetVisibility(true);
  mLeadLog->SetVisAttributes(LeadVisAt);
  
  // Borated Polyethylene Shield
  G4Box* bpoly_box = new G4Box("bpoly_box", bpolyDimensions[0], 
			       bpolyDimensions[1], bpolyDimensions[2]);
  mBpolyLog = new G4LogicalVolume(bpoly_box, boratedpoly_mat, "mBpolyLog");
  mBpolyPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				 "mBpolyPhys", mBpolyLog, mLeadPhys, false, 0);
  G4VisAttributes* BpolyVisAt = new G4VisAttributes(magenta);
  BpolyVisAt->SetVisibility(true);
  mBpolyLog->SetVisAttributes(BpolyVisAt);

  // Copper Shield
  G4Box* copper_box = new G4Box("copper_box", copperDimensions[0], 
				copperDimensions[1], copperDimensions[2]);
  mCopperLog = new G4LogicalVolume(copper_box, copper_mat, "mCopperLog");
  mCopperPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				  "mCopperPhys", mCopperLog, mBpolyPhys, false, 0);
  G4VisAttributes* CopperVisAt = new G4VisAttributes(orange);
  CopperVisAt->SetVisibility(true);
  mCopperLog->SetVisAttributes(CopperVisAt);

  // Acrylic Vessel
  G4Box* acrylic_box = new G4Box("acrylic_box", acrylicDimensions[0], 
				 acrylicDimensions[1], acrylicDimensions[2]);
  mAcrylicLog = new G4LogicalVolume(acrylic_box, acrylic_mat, "mAcrylicLog");
  mAcrylicPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				   "mAcrylicPhys", mAcrylicLog, mCopperPhys, false, 0);
  G4VisAttributes* AcrylicVisAt = new G4VisAttributes(white);
  AcrylicVisAt->SetVisibility(true);
  mAcrylicLog->SetVisAttributes(AcrylicVisAt);

  // // Acrylic -- Scintillator Boundary
  // G4OpticalSurface* AcrylicScintSurface = new G4OpticalSurface
  //   ("AcrylicScintSurface", unified, polished, dielectric_dielectric);
  // G4LogicalBorderSurface* AcrylicScintBorder = new G4LogicalBorderSurface
  //   ("AcrylicScintBorder", mAcrylicPhys, mTargetPhys, AcrylicScintSurface);

  // Target Scintillator
  G4Box* target_box = new G4Box("target_box", targetDimensions[0],
				targetDimensions[1], targetDimensions[2]);
  mTargetLog = new G4LogicalVolume(target_box, target_mat, "mTargetLog");
  mTargetPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				  "mTargetPhys", mTargetLog, mAcrylicPhys, false, 0);
  G4VisAttributes* TargetVisAt = new G4VisAttributes(blue);
  TargetVisAt->SetVisibility(true);
  mTargetLog->SetVisAttributes(TargetVisAt);

  //ConstructPmt();
  G4cout << "PLACING THE PMTs" << G4endl;
  PlacePmts();

  // Sensitive Detectors
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();

  // G4String name="/Scout/ScintSD";
  // mScoutSD = new ScoutScintSD(name, this);
  // SDMan->AddNewDetector(mScoutSD);
  // mTargetLog->SetSensitiveDetector(mScoutSD);
  
  SDMan = G4SDManager::GetSDMpointer();
  G4String name="/Scout/PmtSD";
  mPmtSD = new ScoutPmtSD(name, this);
  SDMan->AddNewDetector(mPmtSD);
  for(int i=0; i<18; i++)
    mPhcathLog[i]->SetSensitiveDetector(mPmtSD);
  //mPhcathLog->SetSensitiveDetector(mPmtD);

  return mWorldPhys;
}


void ScoutDetectorConstruction::ConstructPmt(G4int pmtTubeID)
{
  G4Sphere* pmt_window = new G4Sphere("pmt_sphere", 0, 2*pmtRadius, 0*deg,
  				      360*deg, 0*deg, 30*deg);
  G4Tubs* pmt_tube = new G4Tubs("pmt_tube", 0, pmtRadius, 0.5*pmtHeight,
  				0*deg, 360*deg);
  G4UnionSolid* pmt_sol = new G4UnionSolid("pmt_sol", pmt_tube, pmt_window,
  					   G4Transform3D(G4RotationMatrix(),
							 G4ThreeVector(0, 0, 0.5*pmtHeight-
								       2*pmtRadius*std::cos(30*deg))));

  std::string pmtlogname="mPmtLog"+std::to_string(pmtTubeID);
  mPmtLog[pmtTubeID] = new G4LogicalVolume(pmt_sol, pmt_mat, pmtlogname.c_str());

  G4VisAttributes* PmtVisAt = new G4VisAttributes(red);
  PmtVisAt->SetForceSolid(true);
  PmtVisAt->SetVisibility(true);
  mPmtLog[pmtTubeID]->SetVisAttributes(PmtVisAt);

  // Photocathode
  G4double phcathVOffset = 0.5*pmtHeight-2*pmtRadius*std::cos(30*deg);
  G4double phcathVPosition = phcathVOffset;
  G4Sphere* phcath_shell = new G4Sphere("phcath_sphere_shell", 2*pmtRadius-1.6*mm,
					2*pmtRadius+1.6*mm, 0*deg, 360*deg,
					0*deg, 27*deg);
  G4Sphere* phcath_sol = new G4Sphere("phcath_sphere_sol", 2*pmtRadius-0.1*mm,
				      2*pmtRadius+0.1*mm, 0*deg, 360*deg,
  				      0*deg, 27*deg);
  std::string phcathstoplogname="mPhcathStopLog"+std::to_string(pmtTubeID);
  mPhcathStopLog[pmtTubeID] = new G4LogicalVolume(phcath_shell, phcathstop_mat, phcathstoplogname.c_str());
  std::string phcathlogname="mPhcathLog"+std::to_string(pmtTubeID);
  mPhcathLog[pmtTubeID] = new G4LogicalVolume(phcath_sol, phcath_mat, phcathlogname.c_str());

  G4VisAttributes* PhcathVisAt= new G4VisAttributes(lblue);
  PhcathVisAt->SetForceSolid(true);
  PhcathVisAt->SetVisibility(true);
  mPhcathLog[pmtTubeID]->SetVisAttributes(PhcathVisAt);
}

void ScoutDetectorConstruction::PlacePmts()
{
  // Place nine per side inside their pmt holders
  G4int pmtTubeID=0;
  for(int side=0; side<2; side++)
    for(int row=0; row<3; row++)
      for(int col=0; col<3; col++)
      {
	G4Box* pmtholder_box = new G4Box("pmtholder_box", pmtHolderDimensions[0],
					 pmtHolderDimensions[1], pmtHolderDimensions[2]);
	std::string holderlogname="mPmtHolderLog"+std::to_string(pmtTubeID);
	mPmtHolderLog[pmtTubeID] = new G4LogicalVolume(pmtholder_box, pmtholder_mat, holderlogname.c_str());
	
	// Place the pmt in the collection object
	mPmtInfoCollection.push_back( new ScoutPmtInfo(side, row, col, pmtTubeID) );
	G4RotationMatrix* pmtdirection = new G4RotationMatrix(0*deg, (1-side)*180*deg, 0);
	G4ThreeVector pmtPosition( (row-1)*(acrylicDimensions[0]*(2./3)), 
				   (col-1)*(acrylicDimensions[1]*(2./3)), 
				   pow(-1,side)*(acrylicDimensions[2]+pmtHolderDimensions[2]));
	std::string holdername ="mPmtHolderPhys"+std::to_string(pmtTubeID);
	mPmtHolderPhys[pmtTubeID] = new G4PVPlacement(pmtdirection, pmtPosition, holdername.c_str(),
					   mPmtHolderLog[pmtTubeID], mCopperPhys, false, pmtTubeID);
	G4double phcathVOffset = 0.5*pmtHeight-2*pmtRadius*std::cos(30*deg);
	G4Box* pmtholderfill_box = new G4Box("pmtholderfill_box", pmtHolderFillDimensions[0],
					     pmtHolderFillDimensions[1], pmtHolderFillDimensions[2]);
	std::string holderfillname="mPmtHolderFillLog"+std::to_string(pmtTubeID);
	mPmtHolderFillLog[pmtTubeID] = new G4LogicalVolume(pmtholderfill_box, pmtholderfill_mat, 
							   holderfillname.c_str());
	std::string holderfillphysname="mPmtHolderFillPhys"+std::to_string(pmtTubeID);
	mPmtHolderFillPhys[pmtTubeID] = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), holderfillphysname.c_str(),
					       mPmtHolderFillLog[pmtTubeID], mPmtHolderPhys[pmtTubeID], 
					       false, pmtTubeID);
	ConstructPmt(pmtTubeID);
	std::string pmtphysname="mPmtPhys"+std::to_string(pmtTubeID);
	mPmtPhys[pmtTubeID] = new G4PVPlacement(0, G4ThreeVector(0, 0, -pmtStandOff), pmtphysname.c_str(), 
						mPmtLog[pmtTubeID], mPmtHolderFillPhys[pmtTubeID], 
						false, pmtTubeID);
	std::string phcathphysname="mPhcathPhys"+std::to_string(pmtTubeID);
	mPhcathPhys[pmtTubeID] = new G4PVPlacement(0, G4ThreeVector(0, 0, phcathVOffset),
					phcathphysname.c_str(), mPhcathLog[pmtTubeID], 
					mPmtPhys[pmtTubeID], false, pmtTubeID);
	std::string phcathstopphysname="mPhcathStopPhys"+std::to_string(pmtTubeID);
	mPhcathStopPhys[pmtTubeID] = new G4PVPlacement(0, G4ThreeVector(0, 0, phcathVOffset),
						       phcathstopphysname.c_str(), mPhcathStopLog[pmtTubeID], 
						       mPhcathPhys[pmtTubeID], false, pmtTubeID);
	pmtTubeID++;
      }

  return;
}

void ScoutDetectorConstruction::InitVariables()
{
}
