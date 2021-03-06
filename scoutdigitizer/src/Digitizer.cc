#include "Digitizer.h"
#include "H5Reader.h"
#include "H5Writer.h"
#include <iostream>
#include <string>
#include <vector>
#include <H5Cpp.h>
#include <algorithm>
#include <cmath>

Scout::Digitizer::Digitizer(Scout::H5Reader* reader, Scout::H5Writer* writer)
{
  mReader=reader;
  mWriter=writer;
}

Scout::Digitizer::~Digitizer()
{
}

void Scout::Digitizer::ReadData()
{
  // data to be read in
  mTime = mReader->GetTime();
  mPmtId = mReader->GetPmtId();
  mPmtEnergy = mReader->GetEnergy();
  mPositionX = mReader->GetPositionX();
  mPositionY = mReader->GetPositionY();
  mPositionZ = mReader->GetPositionZ();
  mDirectionX = mReader->GetDirectionX();
  mDirectionY = mReader->GetDirectionY();
  mDirectionZ = mReader->GetDirectionZ();
  mPolarizationX = mReader->GetPolarizationX();
  mPolarizationY = mReader->GetPolarizationY();
  mPolarizationZ = mReader->GetPolarizationZ();
  // data to write out
  mCharge = mWriter->GetCharge();
  //mArrivalTime = mWriter->GetArrivalTime();
}

void Scout::Digitizer::Digitize()
{
  // TODO, configuration parameters will be read in
  // here ... good way for this?
  const int daq_timing_resolution = 2; // ns
  const int timing_window = 200;       // ns
  const int time_size = timing_window / daq_timing_resolution;
  const int pmts = mCharge->size();

  // The data format being read in is a list of
  // pmt hits.
  for(int i=0; i<pmts; i++)
  {
    mCharge->at(i).clear();
    for(int j=0; j<time_size; j++)
      (mCharge->at(i)).push_back(0);
  }

  // Step one lets take those hits and sort them
  // into each pmt == 18, so arrays of vectors
  const size_t nhits = mTime->size();

  for(size_t i=0; i<nhits; ++i)
  {
    int ele = mPmtId->at(i);
    int timeslot = std::floor(mTime->at(i)/daq_timing_resolution);
    if(timeslot*daq_timing_resolution < timing_window)
      (mCharge->at(ele)).at(timeslot)++;
  }
  
  return;
}

void Scout::Digitizer::DigitizeAllData()
{
  int eventcounter = 0;
  while(mReader->StillAlive())
  {
    ReadData();
    mReader->Update();
    Digitize();
    mWriter->Write(eventcounter++);
  }
}
