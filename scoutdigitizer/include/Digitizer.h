// Used to read in HDF5 data for scout and store it
// This is a data structure

#ifndef DIGITIZER_h
#define DIGITIZER_h

#include <vector>
#include <array>
#include <string>
#include <H5Cpp.h>
#include "H5Reader.h"
#include "H5Writer.h"

namespace Scout
{
  
  class Digitizer
  {
    H5::H5File* input;
    Scout::H5Reader* mReader;
    Scout::H5Writer* mWriter;

    // The data to be read
    std::vector<double>* mTime;
    std::vector<int>* mPmtId;
    std::vector<double>* mPmtEnergy;
    std::vector<double>* mPositionX;
    std::vector<double>* mPositionY;
    std::vector<double>* mPositionZ;
    std::vector<double>* mDirectionX;
    std::vector<double>* mDirectionY;
    std::vector<double>* mDirectionZ;
    std::vector<double>* mPolarizationX;
    std::vector<double>* mPolarizationY;
    std::vector<double>* mPolarizationZ;

    // New digitized data:
    std::array<double, 18>* mCharge;      // nV*s
    std::array<double, 18>* mArrivalTime; // ns

    void ReadData();
    void Digitize();

  public:
    Digitizer(Scout::H5Reader*, Scout::H5Writer*);
    ~Digitizer();
    void DigitizeAllData();
  };
};

#endif
