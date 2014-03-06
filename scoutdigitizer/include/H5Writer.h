#ifndef H5WRITER_h
#define H5WRITER_h

#include <H5Cpp.h>
#include <vector>
#include <array>
#include <string>

namespace Scout
{
  class H5Writer
  {678
    H5::H5File* file;
    
    std::vector<double> mCharge;       // nV*s
    std::vector<std::array<double, 18> > mVoltage;      // V
    std::vector<double> mArrivalTime;  // nS
  public:
    H5Writer(std::string filename);
    ~H5Writer();
    Write();
    
    std::vector<double>* GetCharge(){return &mCharge;}
    std::vector<std::array<double, 18> >* GetVoltage(){return &mArrivalTime;}
    std::vector<double>* GetArrivalTime(){return mArrivalTime;}
  };
};

#endif
