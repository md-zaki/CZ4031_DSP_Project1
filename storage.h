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

class StorageDisk {
    private:

        
    public:

        unsigned int totalDiskSize; // size in bytes of the whole disk/main memory
        unsigned int blockSize; //size in bytes of 1 block

        unsigned int usedBlocksSize; //size in bytes of all used blocks
        unsigned int assignedRecordsSize; // size in bytes of all records assigned to blocks
        int currentBlockSizeUsed; // size in bytes of current used space in current block

        int totalBlocks; // total number of blocks used/unused in database system
        int numUsedBlocks; //total number of blocks used
        int numRecordsAssigned; // number of records assigned to database system

        char *diskPtr; // pointer to start of storage
        char *currentBlockPtr; // pointer to start of current block
        
        /**
         * Constructor for Storage class
         * @param totalDiskSize size in bytes of the whole disk/main memory
         * @param blockSize size in bytes of 1 block
        */
        StorageDisk(unsigned int totalDiskSize,unsigned int blockSize);

        //Destructor
        ~StorageDisk(); 

        /**
         * set pointer of current block to the next free block.
         * resets currentBlockSizeUsed to 0 and increase num of used blocks
        */
        void goToNextBlock();

        /**
         * assign a block with available space for insertion of a record
         * @param recordSize size in bytes of the record to be inserted
         * @returns pointer of available block. Returns nulptr if no available block can be assigned
        */
        char *assignBlock(int recordSize);

         /**
         * check if there is enough disk size for another block
         * @returns true if there is enough space. false if there is no space
        */
        bool checkDiskSize();

        /**
         * check if there is enough block space for another record
         * @param recordsize size in bytes of the record to be inserted
         * @returns true if there is enough space. false if there is no space
        */
        bool checkCurrentBlockSize(int recordSize);

        /**
         * insert a record struct into a block
         * @param record the record struct to be inserted into block
         * @returns nothing
        */
        void * recordIntoBlock(Record record);
};



#endif