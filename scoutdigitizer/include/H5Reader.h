#ifndef H5READER_h
#define H5READER_h

#include <vector>
#include <string>
#include <H5Cpp.h>

namespace Scout
{
  class H5Reader
  {
    H5::H5File* file;

    int mSize;
    int mCurrentEvent;
    std::vector<int>* mPmtId;
    std::vector<double>* mTime;
    std::vector<double>* mEnergy;
    std::vector<double>* mPositionX;
    std::vector<double>* mPositionY;
    std::vector<double>* mPositionZ;
    std::vector<double>* mDirectionX;
    std::vector<double>* mDirectionY;
    std::vector<double>* mDirectionZ;
    std::vector<double>* mPolarizationX;
    std::vector<double>* mPolarizationY;
    std::vector<double>* mPolarizationZ;
  public:
    H5Reader(std::string filename);
    ~H5Reader();
    void Update();
    bool StillAlive();
    void Clear();

    std::vector<int>* GetPmtId(){return mPmtId;}
    std::vector<double>* GetTime(){return mTime;}
    std::vector<double>* GetEnergy(){return mEnergy;}
    std::vector<double>* GetPositionX(){return mPositionX;}
    std::vector<double>* GetPositionY(){return mPositionY;}
    std::vector<double>* GetPositionZ(){return mPositionZ;}
    std::vector<double>* GetDirectionX(){return mDirectionX;}
    std::vector<double>* GetDirectionY(){return mDirectionY;}
    std::vector<double>* GetDirectionZ(){return mDirectionZ;}
    std::vector<double>* GetPolarizationX(){return mPolarizationX;}
    std::vector<double>* GetPolarizationY(){return mPolarizationY;}
    std::vector<double>* GetPolarizationZ(){return mPolarizationZ;}

  };
};

#endif
