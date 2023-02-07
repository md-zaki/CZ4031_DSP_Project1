#include <iostream>
#include "storage.h"
#include <cstring>

using namespace std;

StorageDisk::StorageDisk(unsigned int totalDiskSize,unsigned int blockSize){
    //assigning values to Storage class variables
    this->totalDiskSize = totalDiskSize;
    this->blockSize = blockSize;
    this->usedBlocksSize = 0;

    this->assignedRecordsSize = 0;
    this->totalBlocks = totalDiskSize/blockSize;
    this->numUsedBlocks = 0;
    this->numRecordsAssigned = 0;
    this->currentBlockSizeUsed = 0;

    this-> diskPtr =  new char[totalDiskSize]; // returns an address of a space in memory 
    this->currentBlockPtr = diskPtr; // initial block ptr null, no data loaded yet
}

StorageDisk::~StorageDisk() {
}

void StorageDisk::goToNextBlock()
{
  this->currentBlockPtr = this->currentBlockPtr + this->blockSize; // set pointer to next block (plus blockSize)
  this->numUsedBlocks = this->numUsedBlocks + 1; // increase number of blocks used
  this->currentBlockSizeUsed = 0; // set current used block size to 0 due to new fresh block
}


char *StorageDisk::assignBlock(int recordSize)
{
  if(this->checkCurrentBlockSize(recordSize))
  {
    return this->currentBlockPtr; // if there is enough block space, return current block
  }
  else
  {
    if(this->checkDiskSize()) // if not enough block space, check disk space
    {
      this->goToNextBlock();// if enough disk space, create a new block
      return this->currentBlockPtr;// return new block
    }
    else
    {
      return nullptr;// if not enough disk space, return nullptr
    }
  }
}


bool StorageDisk::checkDiskSize()
{
  if ((this->numUsedBlocks + 1) > (this-> totalBlocks)-1)
  {
    return false; 
  }

  else return true;
}


bool StorageDisk::checkCurrentBlockSize(int recordSize)
{
  if((this->currentBlockSizeUsed + recordSize) > this->blockSize)
  {
    return false;
  }

  else return true;
}


void StorageDisk::recordIntoBlock(Record record)
{
  char *blockToBeInserted;
  void *tempRecAddress;
  blockToBeInserted = assignBlock(sizeof(record)); // get block that the record will be inserted into
  int offset = this->currentBlockSizeUsed; // set offset for record relative to block address
  if (blockToBeInserted == nullptr)// check if disk is full
  {
    cout << "DISK FULL"<< endl; 
    return;
  }
  tempRecAddress = (unsigned char *)blockToBeInserted + offset; // create temporary address of record (address of block + offset)
  
  memcpy(tempRecAddress, &record, sizeof(record)); // copy contents of record into temporary address
  this->currentBlockSizeUsed = this->currentBlockSizeUsed + sizeof(record); // increase bytes used in current block
  this->assignedRecordsSize = this->assignedRecordsSize + sizeof(record); // increase bytes used by records in disk
  // cout << "Record with key: " << record.tconst << "successfully inserted" << endl;
}

