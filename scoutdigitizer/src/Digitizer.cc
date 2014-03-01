#include "Digitizer.h"
#include "H5Reader.h"
#include "H5Writer.h"
#include <iostream>
#include <string>
#include <vector>
#include <H5Cpp.h>

Scout::Digitizer::Digitizer(Scout::H5Reader &reader, Scout::H5Writer &writer)
{
  mReader=reader;
  mWriter=writer;
}

Scout::Digitizer::~Digitizer()
{
}

void Scout::Digitizer::ReadData()
{
  /// Okay time to do a series of tests

  // Here is the number of events!
  int events = input->getNumObjs();
  std::cout << "Processing " << events << " events\n";
  
  std::string event_name = input->getObjnameByIdx(0);

  // data to be read in
  mTime = mReader->GetTime();
  mPmtID = mReader->GetPmtID();
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
  mVoltage = mWriter->GetVoltages();
  mArrivalTime = mWriter->GetArrivalTime();
}

void Scout::Digitizer::Digitize()
{
}

void Scout::Digitizer::DigitizeAllData()
{
  while(mReader->StillAlive())
  {
    ReadData();
    mReader->Update();
    mWriter->Write();
  }
}
