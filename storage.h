#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
using namespace std;

/* Structure representing one record
following the column names and datatypes found
data.tsv
*/
struct Record {
    char tconst[10]; //movie id
    float averageRating; //average rating of movie
    unsigned int numVotes; //number of ratings on movie
    
};

struct RecordLocation {
    void *blockAddress;
    unsigned int offset;
};


class StorageDisk {
    private:


    public:

        unsigned int totalDiskSize; // size in bytes of the whole disk/main memory
        unsigned int blockSize; //size in bytes of 1 block
        unsigned int usedBlocksSize; //size in bytes of all used blocks
        unsigned int assignedRecordsSize; // size in bytes of all records assigned to blocks

        int totalBlocks; // total number of blocks in database system
        int numUsedBlocks; //total number of blocks used
        int numRecordsAssigned; // number of records assigned to database system

        void *diskPtr; // pointer to start of storage
        void *currentBlockPtr; // pointer to start of current block
        /**
         * Constructor for Storage class
         * @param totalDiskSize size in bytes of the whole disk/main memory
         * @param blockSize size in bytes of 1 block
        */
        StorageDisk(unsigned int totalDiskSize,unsigned int blockSize);

        //Destructor
        ~StorageDisk(); 

        int getTotalBlocks();

        


        // to do: add functions to add, delete, retrieve records etc
};



#endif