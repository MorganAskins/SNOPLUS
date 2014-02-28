#include "Digitizer.h"
#include <string>
#include <vector>
#include <H5Cpp.h>

Scout::Digitizer::Digitizer(std::string filename)
{
  input = new H5::H5File( filename, H5F_ACC_RDONLY );
}

Scout::Digitizer::~Digitizer()
{
  delete input;
}

void Scout::Digitizer::ReadData()
{
}

void Scout::Digitizer::Digitize()
{
}
