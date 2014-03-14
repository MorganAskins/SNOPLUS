#include "H5Writer.h"
#include <iostream>
#include <vector>
#include <array>
#include <string>

Scout::H5Writer::H5Writer(std::string filename)
{
  file = new H5::H5File(filename, H5F_ACC_TRUNC);
  mCharge = new std::array<std::vector<double>, 18>;
  //mArrivalTime = new std::array<std::vector<double>, 18>;
  this->Init();
}

Scout::H5Writer::~H5Writer()
{
  delete mCharge;
  //delete mArrivalTime;
  file->close();
  delete file;
}

void Scout::H5Writer::Write(int eventid)
{
  const int num_pmts=18;
  H5std_string groupname = "/event_"+std::to_string(eventid);
  H5::Group* pmtgroup = new H5::Group(file->createGroup(groupname));

  typedef struct PmtData
  {
    double charge;
  } PmtData;

  for(int aPmt=0; aPmt<num_pmts; aPmt++)
  {
    const size_t elements=(mCharge->at(aPmt)).size();
    PmtData current_pmt[elements];
    
    H5std_string dataname = groupname + "/pmt" + std::to_string(aPmt);
    for(size_t i=0; i<elements; i++)
    {
      //current_pmt[i].time=(mArrivalTime->at(aPmt)).at(0);
      current_pmt[i].charge=(mCharge->at(aPmt)).at(i);
    }
    
    hsize_t dims[] = {elements};
    H5::DataSpace space(1, dims);
    H5::CompType mtype(sizeof(PmtData));
    //mtype.insertMember("time", HOFFSET(PmtData, time), H5::PredType::NATIVE_DOUBLE);
    mtype.insertMember("charge", HOFFSET(PmtData, charge), H5::PredType::NATIVE_DOUBLE);
    //H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataname, mtype, space));
    //H5::DataSet* dataset = new H5::DataSet(pmtgroup, space);
    H5::DataSet* dataset = new H5::DataSet(pmtgroup->createDataSet(dataname, mtype, space));
    dataset->write(current_pmt, mtype);
    delete dataset;
  }
  //delete pmtgroup;

  this->Init();
}
/*
void Scout::H5Writer::Write(int eventid)
{
  typedef struct digiphoton
  {
    double time;
    double charge;
  } digiphoton;

  const int num_pmts=18;
  digiphoton myDPhoton[num_pmts];
  H5std_string groupname = "/event_"+std::to_string(eventid);
  H5::Group* pmtgroup = new H5::Group(file->createGroup(groupname));
  //H5::Group* pmtgroup = new H5::Group(file->createGroup("/hi"));
  H5std_string dataname = groupname + "/data";
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
  //H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataname, mtype, space));
  H5::DataSet* dataset = new H5::DataSet(pmtgroup, space);
  //H5::DataSet* dataset = new H5::DataSet(pmtgroup->createDataSet(dataname, mtype, space));
  dataset->write(myDPhoton, mtype);
  delete dataset;
  //delete pmtgroup;

  this->Init();
}
*/

void Scout::H5Writer::Init()
{
  // At the beginning and after a write this should happen
  for(auto &it : *mCharge)	// it is a vector<double>
    it.clear();
  // for(auto &it : *mArrivalTime)
  //   it.clear();
}
