#include "H5Reader.h"
#include <H5Cpp.h>
#include <iostream>
#include <cmath>

Scout::H5Reader::H5Reader(std::string filename)
{
  file = new H5::H5File(filename, H5F_ACC_RDONLY);
  mSize = file->getNumObjs();
  mCurrentEvent = 0;

  mPmtId = new std::vector<int>;
  mTime = new std::vector<double>;
  mEnergy = new std::vector<double>;
  mPositionX = new std::vector<double>;
  mPositionY = new std::vector<double>;
  mPositionZ = new std::vector<double>;
  mDirectionX = new std::vector<double>;
  mDirectionY = new std::vector<double>;
  mDirectionZ = new std::vector<double>;
  mPolarizationX = new std::vector<double>;
  mPolarizationY = new std::vector<double>;
  mPolarizationZ = new std::vector<double>;
}

Scout::H5Reader::~H5Reader()
{
  file->close();
  delete file;
  delete mPmtId;
  delete mTime;
  delete mEnergy;
  delete mPositionX;
  delete mPositionY;
  delete mPositionZ;
  delete mDirectionX;
  delete mDirectionY;
  delete mDirectionZ;
  delete mPolarizationX;
  delete mPolarizationY;
  delete mPolarizationZ;
}

void Scout::H5Reader::Update()
{
  if(!StillAlive())
    return;

  std::string eventname = file->getObjnameByIdx(mCurrentEvent);
  //std::cout << "reading from file: " << eventname << std::endl;
  std::cout << "Read event: " << mCurrentEvent << " of " << mSize;
  std::cout << " -- " << std::floor(mCurrentEvent/mSize * 100) << "%\r";
  H5::DataSet dataset = file->openDataSet(eventname);
  H5::DataSpace space = dataset.getSpace();
  int rank = space.getSimpleExtentNdims();

  // Data format in struct:
  typedef struct photon
  {
    double time;
    int pmtid;
    double energy;
    double posx;
    double posy;
    double posz;
    double dirx;
    double diry;
    double dirz;
    double polx;
    double poly;
    double polz;
  } photon;

  int num_photons = dataset.getStorageSize()/sizeof(photon);
  photon myPhoton[num_photons];

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
  
  dataset.read(myPhoton, mtype1);

  this->Clear();
  for(int i=0; i<num_photons; i++)
  {
    mPmtId->push_back(myPhoton[i].pmtid);
    mTime->push_back(myPhoton[i].time);
    mEnergy->push_back(myPhoton[i].energy);
    mPositionX->push_back(myPhoton[i].posx);
    mPositionY->push_back(myPhoton[i].posy);
    mPositionZ->push_back(myPhoton[i].posz);
    mDirectionX->push_back(myPhoton[i].dirx);
    mDirectionY->push_back(myPhoton[i].diry);
    mDirectionZ->push_back(myPhoton[i].dirz);
    mPolarizationX->push_back(myPhoton[i].polx);
    mPolarizationY->push_back(myPhoton[i].poly);
    mPolarizationZ->push_back(myPhoton[i].polz);
  }
  // Increment to next event in the series
  mCurrentEvent++;
}

bool Scout::H5Reader::StillAlive()
{
  if( mCurrentEvent >= mSize )
    return false;
  else
    return true;
}

void Scout::H5Reader::Clear()
{
  mPmtId->clear();
  mTime->clear();
  mEnergy->clear();
  mPositionX->clear();
  mPositionY->clear();
  mPositionZ->clear();
  mDirectionX->clear();
  mDirectionY->clear();
  mDirectionZ->clear();
  mPolarizationX->clear();
  mPolarizationY->clear();
  mPolarizationZ->clear();
}
