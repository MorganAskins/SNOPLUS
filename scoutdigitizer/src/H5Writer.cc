#include "H5Writer.h"
#include <iostream>
#include <vector>
#include <array>
#include <string>

Scout::H5Writer::H5Writer(std::string filename)
{
  file = new H5::H5File(filename, H5F_ACC_TRUNC);
  mCharge = new std::array<double, 18>;
  mArrivalTime = new std::array<double, 18>;
  this->Init();
}

Scout::H5Writer::~H5Writer()
{
  delete mCharge;
  delete mArrivalTime;
  file->close();
  delete file;
}

void Scout::H5Writer::Write(int eventid)
{
  typedef struct digiphoton
  {
    double time;
    double charge;
  } digiphoton;

  const int num_pmts=18;
  digiphoton myDPhoton[num_pmts];

  H5std_string dataname = "event_"+std::to_string(eventid);
  for(hsize_t i=0; i<num_pmts; i++)
  {
    myDPhoton[i].charge=mCharge->at(i);
    myDPhoton[i].time=mArrivalTime->at(i);
  }
  hsize_t dims[] = {num_pmts};
  H5::DataSpace space(1, dims);
  H5::CompType mtype(sizeof(digiphoton));
  mtype.insertMember("time", HOFFSET(digiphoton, time), H5::PredType::NATIVE_DOUBLE);
  mtype.insertMember("charge", HOFFSET(digiphoton, charge), H5::PredType::NATIVE_DOUBLE);
  H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataname, mtype, space));
  dataset->write(myDPhoton, mtype);
  delete dataset;

  this->Init();
}

void Scout::H5Writer::Init()
{
  // At the beginning and after a write this should happen
  for(auto &it : *mCharge)
    it=0;
  for(auto &it : *mArrivalTime)
    it=0;
}
