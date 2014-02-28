// Used to read in HDF5 data for scout and store it
// This is a data structure

#ifndef DIGITIZER_h
#define DIGITIZER_h

#include <vector>
#include <string>
#include <H5Cpp.h>

namespace Scout
{
  
  class Digitizer
  {
    H5::H5File* input;

    // The data to be read
    std::vector<double> mTime;
    std::vector<int> mPmtID;
    std::vector<double> mEnergy;
    std::vector<double> mPositionX;
    std::vector<double> mPositionY;
    std::vector<double> mPositionZ;
    std::vector<double> mDirectionX;
    std::vector<double> mDirectionY;
    std::vector<double> mDirectionZ;
    std::vector<double> mPolarizationX;
    std::vector<double> mPolarizationY;
    std::vector<double> mPolarizationZ;

    // New digitized data:
    std::vector<double> mCharge;      // ns
    std::vector<double> mVoltage[18]; // V
    std::vector<double> mArrivalTime; // nV*s

  public:
    Digitizer(std::string filename);
    ~Digitizer();

    void ReadData();
    void Digitize();
    // This is meant ONLY to read from file,
    // therefore no Setters
  };
};

#endif
