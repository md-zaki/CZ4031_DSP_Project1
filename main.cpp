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
    ifstream testData("data/data10.tsv"); // reading from file

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
            Key key;
            key.value = record.numVotes;
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
    for(int i=0; i<(tree.rootNode)->numKeys;i++)
    {
        cout << "Content of Root Node: " << (tree.rootNode)->keyArray[i].value;
    }
    cout << endl;
    
    cout << "Printed B+ Tree" << endl;
    tree.printTree(tree.rootNode);
    cout << endl;
    // ================================== Experiment 3 =====================================
    cout << "============ Experiment 3 ==============" << endl;
    Key key;
    key.value = 11;
    auto [leafNode,parentNode] = tree.traverseNonLeaf(tree.rootNode, key);
    for(int i=0; i<leafNode->numKeys;i++)
    {
        if((leafNode->keyArray[i].value) == key.value)
        {
            cout << ((Record*)(leafNode->pointerArray[i]))->tconst << endl;
        }
    }
    cout << endl;
    // ================================== Experiment 4 =====================================

    // ================================== Experiment 5 =====================================

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