#include <iostream>
#include "storage.h"

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

    this-> diskPtr =  new char[totalDiskSize]; // returns an address of a space in memory 
    this->currentBlockPtr = nullptr; // initial block ptr null, no data loaded yet
}

StorageDisk::~StorageDisk() {
}

int StorageDisk::getTotalBlocks() {
  return this->totalBlocks;
}

// to do: add functions to add, delete, retrieve records etc