#include "H5Reader.h"

Scout::H5Reader::H5Reader(std::string filename)
{
  file = new H5::H5File(filename, H5F_ACC_RDONLY);
  mSize = file->getNumObjs();
  mCurrentEvent = 0;
}

Scout::H5Reader::~H5Reader()
{
  file->close();
  delete file;
}

void Scout::H5Reader::Update()
{
  if(!StillAlive())
    return;

  std::string eventname = file->getObjnameByIdx(mCurrentEvent);
  H5::DataSet dataset = file->openDataSet(eventname);
  H5::DataSpace space = dataset.getSpace();
  int rank = space.getSimpleExtentNdims();

  // Increment to next event in the series
  mCurrentEvent++;
}

bool Scout::H5Reader::StillAlive()
{
  if( mCurrentEvent >= mSize )
    return false;
  else
    return true;
}
