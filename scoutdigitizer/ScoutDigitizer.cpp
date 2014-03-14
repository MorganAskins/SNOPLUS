// ScoutDigitizer will take data from Scout in hdf5 format
// and digitize the information into pmt pulses. The input
// format will be std::vectors of pmthits for each event
// the output will be a waveform (200ns long, at 1ns/bin)
// for each PMT (totalling 18). An integrated pulse (charge),
// and a time of first hit.

// C++(11) classes
#include <vector>
#include <iostream>
#include <numeric>
#include <string>
//using namespace std;

// hdf5 format
#include <H5Cpp.h>
//using namespace H5;

// user defined classes
#include "Digitizer.h"
#include "H5Writer.h"

int main(int argc, char* argv[])
{
  // Must give a file to operate on
  if( argc != 2 )
  {
    std::cout << "Input requires one filename with extension h5" << std::endl;
    return -1;
  }
  // input is an hdf5 file ending with h5
  std::string fname(argv[1]);
  std::string ending=".h5";
  if( fname.compare(fname.size()-ending.size(), ending.size(), ending) )
  {
    std::cout << "Only accepting .h5 files currently" << std::endl;
    return -1;
  }

  // Write the data back into hdf5
  std::string outputname=fname.substr(0, fname.size()-3)+"digi.h5";
  std::cout << "Writing to: " << outputname << std::endl;;

  Scout::H5Reader reader(fname);
  std::cout << "Created the reader" << std::endl;
  Scout::H5Writer writer(outputname);
  std::cout << "Created the writer" << std::endl;

  Scout::Digitizer digi(&reader, &writer);
  std::cout << "Digitizing data" << std::endl;
  digi.DigitizeAllData();

  std::cout << "c'est fini!" << std::endl;

  return 0;
}
