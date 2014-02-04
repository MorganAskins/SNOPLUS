// Scout Detector Design

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

  // I can add colors here if I want ...

  // List of volumes are as follows (in order):
  // World, Target
  // Note: G4 Objects use radii (even for boxes)

  const G4double targetDimensions[3] = {0.5*m, 0.5*m, 0.5*m};
  const G4double rockThickness = 25*cm;
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

}
