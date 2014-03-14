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
  mArrivalTime = mWriter->GetArrivalTime();
}

void Scout::Digitizer::Digitize()
{
  // TODO, configuration parameters will be read in
  // here ... good way for this?

  // The data format being read in is a list of
  // pmt hits.

  // Step one lets take those hits and sort them
  // into each pmt == 18, so arrays of vectors
  std::array<std::vector<double>, 18> pmtTime;
  //std::cout << "Digitizing now!!" << std::endl;
  const size_t nhits = mTime->size();

  for(size_t i=0; i<nhits; ++i)
  {
    int ele = mPmtId->at(i);
    (pmtTime.at(ele)).push_back(mTime->at(i));
    mCharge->at(ele) += 1;
  }
  // The goal is to find the time of first hit
  for(size_t i=0; i<pmtTime.size(); ++i)
  {
    std::sort((pmtTime.at(i)).begin(), (pmtTime.at(i)).end());
    mArrivalTime->at(i) = (pmtTime.at(i)).at(0);
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
