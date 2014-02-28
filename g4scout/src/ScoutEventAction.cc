#include "ScoutEventAction.hh"
#include "ScoutRunAction.hh"
#include "ScoutPrimaryGeneratorAction.hh"
#include "ScoutEventActionMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

ScoutEventAction::ScoutEventAction(ScoutRunAction* runAction,
				   ScoutPrimaryGeneratorAction* genAction)
  : mRunAction(runAction),mGeneratorAction(genAction)
{
  mActionMessenger = new ScoutEventActionMessenger(this);
  // will add scout digitizer here

  mScintillatorCollID = -1;
  mPmtCollID = -1;
}

ScoutEventAction::~ScoutEventAction()
{
  delete mActionMessenger;
}

void ScoutEventAction::BeginOfEventAction(const G4Event*)
{
  if(mScintillatorCollID == -1)
  {
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    mScintillatorCollID = SDman->GetCollectionID("mScintillatorCollection");
  }
  if(mPmtCollID == -1)
  {
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    mPmtCollID = SDman->GetCollectionID("mPmtCollection");
  }
}

void ScoutEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "Processing event: " << event->GetEventID() << G4endl;
  FillRootEvent(1);
  G4int evid = event->GetEventID();
  
  ScoutScintHitsCollection* SHC = NULL;
  ScoutPmtHitsCollection* PHC = NULL;
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(HCE)
  {
    //SHC = (ScoutScintHitsCollection*)(HCE->GetHC(mScintillatorCollID));
    PHC = (ScoutPmtHitsCollection*)(HCE->GetHC(mPmtCollID));
  }

  // scintillator -- TODO
  std::vector<ScoutPmtHit*> *collection = PHC->GetVector();
  G4cout << "Pmt hits: " << PHC->entries() << G4endl;
  for( auto& it : *collection )
  {
    mRunAction->AddPmtID(it->GetTubeID());
    mRunAction->AddEnergy(it->GetEnergy());
    mRunAction->AddPosition(it->GetPos());
    mRunAction->AddDirection(it->GetDir());
    mRunAction->AddPolarization(it->GetPol());
    mRunAction->AddTime(it->GetTime());
  }
  
  FillHDF5Event(evid);
}

void ScoutEventAction::FillHDF5Event(G4int event_id)
{
  H5::H5File* file = mRunAction->GetFile();

  // Grab all of the variables to write to this event from the run action
  std::vector<G4double>*  energyv = mRunAction->GetEnergy();
  std::vector<G4int>* pmtidv = mRunAction->GetPmtID();
  std::vector<G4ThreeVector>* positionv = mRunAction->GetPosition();
  std::vector<G4ThreeVector>* directionv = mRunAction->GetDirection();
  std::vector<G4ThreeVector>* polarizationv = mRunAction->GetPolarization();
  std::vector<G4double>*  timev = mRunAction->GetTime();
  const hsize_t nhits = pmtidv->size();
  // Will replace this struct later!! Holds photon information
  typedef struct photon{
    G4double time;
    G4int pmtid;
    G4double energy;
    G4double posx;
    G4double posy;
    G4double posz;
    G4double dirx;
    G4double diry;
    G4double dirz;
    G4double polx;
    G4double poly;
    G4double polz;
  } photon;
  // Now define a dataspace and data set rank 1 dimension nhits
  const int rank=1;
  H5std_string dataname = "event_"+std::to_string(event_id);
  photon gamma[nhits];
  for(hsize_t i=0; i<nhits; i++)	// Fill all photons
  {
    gamma[i].time=timev->at(i);
    gamma[i].pmtid=pmtidv->at(i);
    gamma[i].energy=energyv->at(i);
    gamma[i].posx=(positionv->at(i))[0];
    gamma[i].posy=(positionv->at(i))[1];
    gamma[i].posz=(positionv->at(i))[2];
    gamma[i].dirx=(directionv->at(i))[0];
    gamma[i].diry=(directionv->at(i))[1];
    gamma[i].dirz=(directionv->at(i))[2];
    gamma[i].polx=(polarizationv->at(i))[0];
    gamma[i].poly=(polarizationv->at(i))[1];
    gamma[i].polz=(polarizationv->at(i))[2];
  }
  // Now define a dataspace and data set rank 1 dimension nhits
  hsize_t dims[] = {nhits};
  H5::DataSpace space(rank, dims);
  // Create the memory datatype
  H5::CompType mtype1( sizeof(photon) );
  H5std_string pmtidname = "pmtid";
  mtype1.insertMember("time", HOFFSET(photon, time), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("pmtid", HOFFSET(photon, pmtid), H5::PredType::NATIVE_INT);
  mtype1.insertMember("energy", HOFFSET(photon, energy), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("posx", HOFFSET(photon, posx), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("posy", HOFFSET(photon, posy), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("posz", HOFFSET(photon, posz), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("dirx", HOFFSET(photon, dirx), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("diry", HOFFSET(photon, diry), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("dirz", HOFFSET(photon, dirz), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("polx", HOFFSET(photon, polx), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("poly", HOFFSET(photon, poly), H5::PredType::NATIVE_DOUBLE);
  mtype1.insertMember("polz", HOFFSET(photon, polz), H5::PredType::NATIVE_DOUBLE);
  // Create the dataset
  H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataname, mtype1, space));
  dataset->write(gamma, mtype1);
  delete dataset;
}



void ScoutEventAction::FillRootEvent(G4int)
{
  return;
}
