// Scout Detector Design

#include "ScoutDetectorConstruction.hh"
#include "ScoutDetectorMessenger.hh"
#include "ScoutScintSD.hh"
#include "ScoutPmtSD.hh"

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
  #include "ScoutDetectorMaterial.icc"
}

G4VPhysicalVolume* ScoutDetectorConstruction::Construct()
{
  DefineMaterials();
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

  // Target Scintillator
  G4Box* target_box = new G4Box("target_box", targetDimensions[0],
				targetDimensions[1], targetDimensions[2]);
  mTargetLog = new G4LogicalVolume(target_box, target_mat, "mTargetLog");
  mTargetPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
				  "mTargetPhys", mTargetLog, mAcrylicPhys, false, 0);
  G4VisAttributes* TargetVisAt = new G4VisAttributes(blue);
  TargetVisAt->SetVisibility(true);
  mTargetLog->SetVisAttributes(TargetVisAt);

  ConstructPmt();
  PlacePmts();



  // Sensitive Detectors
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();

  G4String name="/Scout/ScintSD";
  mScoutSD = new ScoutScintSD(name, this);
  SDMan->AddNewDetector(mScoutSD);
  mTargetLog->SetSensitiveDetector(mScoutSD);
  
  SDMan = G4SDManager::GetSDMpointer();
  name="/Scout/PmtSD";
  mPmtSD = new ScoutPmtSD(name, this);
  SDMan->AddNewDetector(mPmtSD);
  mPhcathLog->SetSensitiveDetector(mPmtSD);

  return mWorldPhys;
}


void ScoutDetectorConstruction::ConstructPmt()
{
  // Pmt Holders

  // Photomultipliers

  G4Sphere* pmt_window = new G4Sphere("pmt_sphere", 0, 2*pmtRadius, 0*deg,
  				      360*deg, 0*deg, 30*deg);
  G4Tubs* pmt_tube = new G4Tubs("pmt_tube", 0, pmtRadius, 0.5*pmtHeight,
  				0*deg, 360*deg);
  G4UnionSolid* pmt_sol = new G4UnionSolid("pmt_sol", pmt_tube, pmt_window,
  					   G4Transform3D(G4RotationMatrix(),
							 G4ThreeVector(0, 0, 0.5*pmtHeight-
								       2*pmtRadius*std::cos(30*deg))));

  mPmtLog = new G4LogicalVolume(pmt_sol, pmt_mat, "mPmtLog");
  //G4RotationMatrix* pmtdirection = new G4RotationMatrix(0*deg, 180*deg, 0);
  //mPmtPhys = new G4PVPlacement(pmtdirection, pmtPosition, "mPmtPhys", mPmtLog, mLabPhys, false, 0);
  
  //G4OpticalSurface* pmt_opsurf = new G4OpticalSurface("pmt_opsurf", unified, polished, dielectric_dielectric);
  //G4LogicalBorderSurface* pmt_surf = new G4LogicalBorderSurface("pmt_surf", mTargetPhys, mPmtPhys, pmt_opsurf);

  G4VisAttributes* PmtVisAt = new G4VisAttributes(red);
  PmtVisAt->SetForceSolid(true);
  PmtVisAt->SetVisibility(true);
  mPmtLog->SetVisAttributes(PmtVisAt);

  // Photocathode
  G4double phcathVOffset = 0.5*pmtHeight-2*pmtRadius*std::cos(30*deg);
  G4double phcathVPosition = phcathVOffset;

  G4Sphere* phcath_sol = new G4Sphere("phcath_sphere", 0,
				      2*pmtRadius+1.6*mm, 0*deg, 360*deg,
  				      0*deg, 27*deg);

  mPhcathLog = new G4LogicalVolume(phcath_sol, phcath_mat, "mPhcathLog");
  // mPhcathPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, phcathVPosition),
  // 				  "mPhcathPhys", mPhcathLog, mPmtPhys, false, 0);

  //G4OpticalSurface* phcath_opsurf = new G4OpticalSurface("phcath_opsurf", unified, polished, dielectric_dielectric);
  //G4LogicalBorderSurface* phcath_surf = new G4LogicalBorderSurface("phcath_surf", mPmtPhys, mPhcathPhys, phcath_opsurf);
  
  G4MaterialPropertiesTable* phcath_mt = new G4MaterialPropertiesTable();
  phcath_mt->AddProperty("REFLECTIVITY", phcath_PP, phcath_REFL, NUM);
  //phcath_opsurf->SetMaterialPropertiesTable(phcath_mt);

  G4VisAttributes* PhcathVisAt= new G4VisAttributes(lblue);
  PhcathVisAt->SetForceSolid(true);
  PhcathVisAt->SetVisibility(true);
  mPhcathLog->SetVisAttributes(PhcathVisAt);
}

void ScoutDetectorConstruction::PlacePmts()
{
  G4Box* pmtholder_box = new G4Box("pmtholder_box", pmtHolderDimensions[0],
				   pmtHolderDimensions[1], pmtHolderDimensions[2]);
  mPmtHolderLog = new G4LogicalVolume(pmtholder_box, pmtholder_mat, "mPmtHolderLog");
  // Place nine per side inside their pmt holders
  for(int side=0; side<2; side++)
    for(int row=0; row<3; row++)
      for(int col=0; col<3; col++)
      {
	G4RotationMatrix* pmtdirection = new G4RotationMatrix(0*deg, (1-side)*180*deg, 0);
	G4ThreeVector pmtPosition( (row-1)*(acrylicDimensions[0]*(2./3)), 
				   (col-1)*(acrylicDimensions[1]*(2./3)), 
				   pow(-1,side)*(acrylicDimensions[2]+pmtHolderDimensions[2]));

	mPmtHolderPhys = new G4PVPlacement(pmtdirection, pmtPosition, "mPmtHolderPhys",
					   mPmtHolderLog, mCopperPhys, false, 0);
      }
  G4double phcathVOffset = 0.5*pmtHeight-2*pmtRadius*std::cos(30*deg);
  G4Box* pmtholderfill_box = new G4Box("pmtholderfill_box", pmtHolderFillDimensions[0],
				       pmtHolderFillDimensions[1], pmtHolderFillDimensions[2]);
  mPmtHolderFillLog = new G4LogicalVolume(pmtholderfill_box, pmtholderfill_mat, "mPmtHolderFillLog");
  mPmtHolderFillPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "mPmtHolderFillPhys",
					 mPmtHolderFillLog, mPmtHolderPhys, false, 0);

  mPmtPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -pmtStandOff), "mPmtPhys", 
			       mPmtLog, mPmtHolderFillPhys, false, 0);
  mPhcathPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, phcathVOffset),
				  "mPhcathPhys", mPhcathLog, mPmtPhys, false, 0);
  return;
}

void ScoutDetectorConstruction::InitVariables()
{
}
