#ifndef H5READER_h
#define H5READER_h

#include <vector>
#include <string>
#include <H5Cpp.h>

namespace Scout
{
  class H5Reader
  {
    int mSize;
    int mCurrentEvent;
    std::vector<int> mPmtId;
    std::vector<double> mTime;
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
  public:
    H5Reader(std::string filename);
    ~H5Reader();
    void Update();

  };
};

#endif
