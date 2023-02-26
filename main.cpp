using namespace std;


#include "storage.h"
#include "storage.cpp"
#include "rewrite.h"
#include "rewrite.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <cstring>

#include <bits/stdc++.h>

void searchTreeSingle(int key, BPlusTree tree, StorageDisk disk);
void searchStorageSingle(int key, StorageDisk disk);

int main()
{
    int blockSize, diskSize;
    blockSize = 200; // declare blockSize in bytes
    diskSize = 100000000; // declare whole disk size in bytes
    StorageDisk disk(diskSize,blockSize); // creating a disk object
    BPlusTree tree;

    
    fstream fin;
    int indexnum = 0;
    // =================== Reading file and inserting into database ========================

    Record record; //declare temporary record
    ifstream testData("data/dataSmalltim.tsv"); // reading from file
    int count = 0;

    if (testData.is_open()){
        string line,index;
        getline(testData,line);
        while(getline(testData,line,'\n')) // getting each line as a seperate value by specifying \n
        {
            stringstream s(line); // converting extracted line into a stream object
            
            while(getline(s,index,'\t')) // getting each feature as a seperate value by specifying \t
            {
                if (indexnum == 0) // first value is tconst
                {
                    strcpy(record.tconst, index.c_str()); // store current feature in current line into temporary record
                }
                else if (indexnum ==1) //second value is averageRating
                {
                    record.averageRating=stof(index); // store current feature in current line into temporary record
                }
                else if(indexnum == 2) //third value is numVotes
                {
                    record.numVotes=stoi(index); // store current feature in current line into temporary record
                }
                indexnum = indexnum + 1; // move on to next feature in line
            }
            indexnum = 0;
            void *recAddress;
            recAddress = disk.recordIntoBlock(record); // insert packed record into a block, returns record address
            int key;
            key = record.numVotes;
            tree.insertion(key,recAddress);
        }
        
    }

     // ================================== Experiment 1 =====================================
    cout << "=========== Experiment 1 ===============" << endl;
    int numofrecords;
    numofrecords = disk.assignedRecordsSize / sizeof(Record);
    cout << "Number of records in disk: " << numofrecords << endl;
    cout << "Size of a record: " << sizeof(Record) << endl;
    cout << "Number of records stored in a block: " << disk.blockSize/sizeof(Record) << endl;
    cout << "Number of blocks used for storing data: " << disk.numUsedBlocks + 1 << endl; 
    cout << endl;
    
    // ================================== Experiment 2 =====================================
    cout << "============ Experiment 2 ==============" << endl;
    cout << "Parameter n of B+ Tree: " << MAX_KEYS << endl;
    cout << "Number of nodes in B+ Tree: " << tree.numOfNodes << endl;
    cout << "Number of levels in B+ Tree: " << tree.numOfLevels << endl;
    cout << "Content of Root Node: " << endl;
    for(int i=0; i<(tree.rootNode)->numKeys;i++)
    {
        cout << (tree.rootNode)->keyArray[i] << " ";
    }
    cout << endl;
    
    cout << "Printed B+ Tree (not needed for final submission)" << endl;
    tree.printTree(tree.rootNode);
    cout << endl;

    // ================================== Experiment 3 =====================================
    cout << "============ Experiment 3 ==============" << endl;
    searchTreeSingle(13, tree, disk);
    cout << endl;
    // ================================== Experiment 4 =====================================
    cout << "============ Experiment 4 ==============" << endl;

    // ================================== Experiment 5 =====================================
    cout << "============ Experiment 5 ==============" << endl;

    int key;
    while(true){
        cout << "Enter a number to delete: "; 
        cin >> key;
        tree.deleteKey(key);
        tree.printTree(tree.rootNode);
    }

    // ================================== Listing contents of a block =====================
    // int blocknum;
    // cout << "================= Listing contents of a block ===============" << endl;
    // cout << "Key in block number: ";
    // cin >> blocknum; // key in block number
    // if (blocknum-1 <= disk.numUsedBlocks)
    // {
    //     cout << "tconst of block number " << blocknum << ": " << endl;
    //     void *currentrecord; // declare pointer for record to be listed
    //     unsigned char *selectedblockptr; // declare pointer to get block pointer to be listed
    //     selectedblockptr = (unsigned char *)disk.diskPtr + (blocknum-1)*disk.blockSize; // set block pointer to specified block number
    //     for(int i=0;i<disk.blockSize;i=i+(sizeof(Record))) // iterate through block
    //     {
    //         currentrecord = (unsigned char *)selectedblockptr+i; // set current record (block ptr + offset)
    //         cout << "tcosnt: " << (*((Record *)currentrecord)).tconst << endl;
    //     }
    // }
    // else
    // {
    //     cout << "Block does not exist" << endl;
    // }
    
    // ====================================================================================
    return 0;
}

void searchTreeSingle(int key, BPlusTree tree, StorageDisk disk)
{
    DataAddressList* addressList_cursor;
    int keyStruct;
    keyStruct = key;
    float totalAvg = 0;
    int numOfRecords = 0;
    clock_t start, end;
    clock_t startLinear, endLinear;
    start = clock();
    cout << "RETRIEVAL USING BPLUSTREE:" << endl;
    cout << "Key: numVotes = " << keyStruct << endl;
    auto [leafNode,parentNode] = tree.traverseNonLeaf(tree.rootNode, keyStruct);
    for(int i=0; i<leafNode->numKeys;i++)
    {
        if((leafNode->keyArray[i]) == key)
        {
            addressList_cursor = (DataAddressList*) leafNode->pointerArray[i];
            while(true){    // loop through the addressList for each key
                for(int j=0; j<addressList_cursor->size; j++){
                    cout << "tconst: " << addressList_cursor->addressList[j]->tconst << " ";
                    cout << "avgRating: " << addressList_cursor->addressList[j]->averageRating << " ";
                    cout << "numVotes: " << addressList_cursor->addressList[j]->numVotes << endl;
                    totalAvg = totalAvg + (float)(addressList_cursor->addressList[j]->averageRating);
                    numOfRecords++;

                }
                if(addressList_cursor->nextList == NULL){   //end of linked addressList
                    break;
                }
                addressList_cursor = addressList_cursor->nextList;
            }
        }
    }
    end = clock();
    double time_taken = double(end-start) / double(CLOCKS_PER_SEC);
    cout << endl;

    cout << "Number of index nodes processed: " << tree.numOfLevels << endl;
    cout << "Number of data blocks the processed: " << tree.numOfLevels + 1 << endl;
    float avgAll;
    avgAll = (float)(totalAvg/(float)(numOfRecords));
    cout << "Average of averageRatings of all returned records: " << avgAll<< endl;
    cout << "Running time of retrieval using BPlusTree: " << fixed << time_taken << setprecision(5) << " seconds" << endl;

    startLinear = clock();
    searchStorageSingle(key, disk);
    endLinear = clock();
    double time_taken_linear = double(endLinear-startLinear) / double(CLOCKS_PER_SEC);
    cout << "Running time of retrieval using Linear Search: " << fixed << time_taken_linear << setprecision(5) << " seconds" << endl;
    


}

void searchStorageSingle(int key, StorageDisk disk)
{
    cout << endl;
    cout << "RETRIEVAL USING LINEAR SEARCH:" << endl;
    cout << " Key: numVotes = " << key << endl;
    float totalAvg = 0;
    int numOfRecords = 0;
    int numofblocks=0;
    for(int i = 0; i <= disk.numUsedBlocks; i++)
    {
        void *currentrecord; // declare pointer for record to be listed
        unsigned char *selectedblockptr; // declare pointer to get block pointer to be listed
        selectedblockptr = (unsigned char *)disk.diskPtr + i*disk.blockSize; // set block pointer to specified block number
        for(int j=0;j<disk.blockSize;j=j+(sizeof(Record))) // iterate through block
        {
            currentrecord = (unsigned char *)selectedblockptr+j; // set current record (block ptr + offset)
            if(key == (*((Record *)currentrecord)).numVotes)
            {
                cout << "tcosnt: " << (*((Record *)currentrecord)).tconst << ", ";
                cout << "avgRating: " << (*((Record *)currentrecord)).averageRating << ", ";
                cout << "numVotes: " << (*((Record *)currentrecord)).numVotes << endl;
                totalAvg = totalAvg + (*((Record *)currentrecord)).averageRating;
                numOfRecords++;
            }
            
        }
        numofblocks++;
    }

    cout << "Number of data blocks accessed: " << numofblocks << endl;
    float avgAll;
    avgAll = (float)(totalAvg/(float)(numOfRecords));
    cout << "Average of averageRatings of all returned records: " << avgAll<< endl;

}
void searchTreeRange(int key)
{
 // to be done
}