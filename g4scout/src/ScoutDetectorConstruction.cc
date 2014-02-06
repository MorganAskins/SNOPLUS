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

#include "ScoutDetectorConstruction.h"

ScoutDetectorConstruction::ScoutDetectorConstruction()
{
  mDetectorMessenger = new ScoutDetectorMessenger(this);
}

ScoutDetectorConstruction::~ScoutDetectorConstruction()
{
  delete mDetectorMessenger;
}

ScoutDetectorConstruction::DefineMaterials()
{
  #include "ScoutDetectorMaterial.icc"
}

G4VPhysicalVolume* ScoutDetectorConstruction::Construct()
{
  DefineMaterials();

  // COLOURS ...
  G4Colour  white   (1.0, 1.0, 1.0);
  G4Colour  grey    (0.5, 0.5, 0.5);
  G4Colour  lgrey   (.85, .85, .85);
  G4Colour  red     (1.0, 0.0, 0.0);
  G4Colour  blue    (0.0, 0.0, 1.0);
  G4Colour  cyan    (0.0, 1.0, 1.0);
  G4Colour  magenta (1.0, 0.0, 1.0); 
  G4Colour  yellow  (1.0, 1.0, 0.0);
  G4Colour  orange  (.75, .55, 0.0);
  G4Colour  lblue   (0.0, 0.0, .75);
  G4Colour  lgreen  (0.0, .75, 0.0);
  G4Colour  green   (0.0, 1.0, 0.0);
  G4Colour  brown   (0.7, 0.4, 0.1);

  // List of volumes are as follows (in order):
  // World, Target
  // Note: G4 Objects use radii (even for boxes)

  const G4double targetDimensions[3] = {0.5*m, 0.5*m, 0.5*m};
  const G4double labDimensions[3] = {targetDimensions[0] + 1*m,
				     targetDimensions[1] + 1*m,
				     targetDimensions[2] + 1*m };
  const G4double worldDimensions[3] = {labDimensions[0] + 25*cm,
				       labDimensions[1] + 25*cm,
				       labDimensions[2] + 25*cm };
  
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

  // Detector Assembly:
  G4box* target_box = new G4Box("target_box", targetDimensions[0],
				targetDimensions[1], targetDimensions[2]);
  mTargetLog = new G4LogicalVolume(target_box, target_mat, "mTargetLog");
  mTargetPhys = new G4LogicalVolume(0, G4ThreeVector(0, 0, 0),
				    "mTargetPhys", mTargetLog, mLabPhys, false, 0);
  G4VisAttributes* TargetVisAt = new G4VisAttributes(cyan);
  TargetVisAt->SetVisability(true);
  mTargetLog->SetVisAttributes(TargetVisAt);


  // Attributes of the PMT's
  // Photomultipliers
  G4double pmtHeight = 15*cm;
  G4double pmtRadius = 10*cm;
  G4double pmtOffset = 1*cm;
  G4ThreeVector pmtPosition(0, 0, 0.5*pmtHeight + pmtOffset + targetDimensions[3]);
  
  G4Sphere* pmt_window = new G4Sphere("pmt_sphere", 0*cm, 2*pmtRadius, 0*deg,
				      360*deg, 0*deg, 30*deg);
  G4Tubs* pmt_tube = new G4Tubs("pmt_tube", 0*cm, pmtRadius, 0.5*pmtHeight,
				0*deg, 360*deg);
  G4UnionSolid* pmt_sol = new G4UnionSolid("pmt_sol", pmt_tube, pmt_window, 
					   G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, 0, 0.5*pmtHeight-
											   2*pmtRadius*std::cos(30*deg))));

  mPMTLog = new G4LogicalVolume(pmt_sol, pmt_mat, "mPMTLog");
  mPMTPhys = new G4PVPlacement(0, pmtPosition, "mPMTPhys", mPMTLog, mLabPhys, false, 0);

  G4OpticalSurface* pmt_opsurf = new G4OpticalSurface("pmt_opsurf", unified, polished, dielectric_dielectric);
  G4LogicalBorderSurface pmt_surf = new G4LogicalBorderSurface("pmt_surf", mTargetPhys, mPMTPhys, pmt_opsurf);

  G4VisAttributes* PMTVisAt = new G4VisAttributes(red);
  PMTVisAt->SetForceSolid(true);
  PMTVisAt->SetVisibility(true);
  mPMTLog->SetVisAttributes(PMTVisAt);

  // Photocathode				    
  G4double phcathVOffset = 0.5*pmtHeight-2*pmtRadius*std::cos(30*deg);
  G4double phcathVPosition = phcathOffset;

  G4Sphere* phcath_sol = new G4Sphere("phcath_sphere", 2*pmtRadius-1.6mm, 2*pmtRadius-1.59*mm, 0*deg, 360*deg,
				      0*deg, 27*deg);

  mPhcathLog = new G4LogicalVolume(phcath_sol, phcath_mat, "mPhcathLog");
  mPhcathPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, phcathVPosition),
				  "mPhcathPhys", mPhcathLog, mPMTPhys, false, 0);

  G4OpticalSurface* phcath_opsurf = new G4OpticalSurface("phcath_opsurf", unified, polished, dielectric_dielectric);
  G4LogicalBorderSurface* phcath_surf = new G4LogicalBorderSurface("phcath_surf", mPMTPhys, mPhcathPhys, phcath_opsurf);

  G4int NUM = 2;
  G4double phcath_PP[NUM]   = { 6.00*eV, 7.50*eV };
  G4double phcath_REFL[NUM] = { 0.0, 0.0};
  G4MaterialPropertiesTable* phcath_mt = new G4MaterialPropertiesTable();
  phcath_mt->AddProperty("REFLECTIVITY", phcath_PP, phcath_REFL, NUM);
  phcath_opsurf->SetMaterialPropertiesTable(phcath_mt);

  G4VisAttributes* PhcathVisAt= new G4VisAttributes(lblue);
  PhcathVisAt->SetForceSolid(true);
  PhcathVisAt->SetVisibility(true);
  mPhcathLog->SetVisAttributes(PhcathVisAt);

  // Sensitive Detectors
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();

  G4String name="/Scout/ScintSD";
  mScoutSD = new ScoutScintSD(name, this);
  SDMan->AddNewDetector(mScoutSD);
  mTargetLog->SetSensitiveDetector(mScoutSD);

  SDMan = G4SDManager::GetSDMpointer();
  name="/Scout/PMTSD";
  PMTSD = new ScoutPmtSD(name, this);
  SDMan->AddNewDetector(PMTSD);
  mPhcathLog->SetSensitiveDetector(PMTSD);

  return mWorldPhys;
}
