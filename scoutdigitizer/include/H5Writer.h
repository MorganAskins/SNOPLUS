#ifndef H5WRITER_h
#define H5WRITER_h

#include <H5Cpp.h>
#include <vector>
#include <array>
#include <string>

namespace Scout
{
  class H5Writer
  {
    H5::H5File* file;
    
    std::array<std::vector<double>, 18>* mCharge;       // nV*s
    //std::array<std::vector<double>, 18>* mArrivalTime;  // nS
  public:
    H5Writer(std::string filename);
    ~H5Writer();
    void Write(int);
    void Init();
    
    std::array<std::vector<double>, 18>* GetCharge(){return mCharge;}
    //std::array<std::vector<double>, 18>* GetArrivalTime(){return mArrivalTime;}
  };
};

#endif
