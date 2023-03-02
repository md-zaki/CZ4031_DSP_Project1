#include <chrono>
#include "storage.h"
#include "storage.cpp"
#include "bplustree_final.h"
#include "bplustree_final.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <cstring>
using namespace std::chrono;




#include <bits/stdc++.h>

void searchTreeSingle(int key, BPlusTree tree, StorageDisk disk, bool print);
void searchStorageSingle(int key, StorageDisk disk, bool print);
void searchTreeRange(int lower, int upper, BPlusTree tree, StorageDisk disk, bool print);
void searchStorageRange(int lower, int upper, StorageDisk disk, bool print);
void deletelinearscan(StorageDisk disk, bool print, int key);
int getBlockRecordisIn(Record * record, StorageDisk disk, int j);

int main()
{
    bool print = false; // true to print out everything, false to omit printing in output
    int blockSize, diskSize;
    blockSize = 200; // declare blockSize in bytes
    diskSize = 100000000; // declare whole disk size in bytes
    StorageDisk disk(diskSize,blockSize); // creating a disk object
    BPlusTree tree;

    
    fstream fin;
    int indexnum = 0;
    // =================== Reading file and inserting into database ========================

    Record record; //declare temporary record
    ifstream testData("data/data.tsv"); // reading from file
    // std::ofstream out("output.txt"); // writing to file

    // std::cout.rdbuf(out.rdbuf()); // redirect std::cout to out.txt!

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
    cout << "================== Welcome to CZ4031 Project 1, Database Systems Principles ======================" << endl;
    // cout << "Key in Experiment to conduct: 1, 2, 3, 4 or 5" << endl;
    // cout << "Key in 0 to exit" << endl;
    // cout << "Experiment: " ;
    //int choice;
    int choice=1; //if output to file, and cycle thru all experiments
    //cin >> choice; //comment if cycling thru all experiments
    while(choice != 6)
    {
        if(choice == 1)
        {
            // ================================== Experiment 1 =====================================
            cout << "=========== Experiment 1 ===============" << endl;
            int numofrecords;
            numofrecords = disk.assignedRecordsSize / sizeof(Record);
            cout << "Number of records in disk: " << numofrecords << endl;
            cout << "Size of a record: " << sizeof(Record) << endl;
            cout << "Number of records stored in a block: " << disk.blockSize/sizeof(Record) << endl;
            cout << "Number of blocks used for storing data: " << disk.numUsedBlocks + 1 << endl; 
            cout << endl;
        }
        else if(choice == 2)
        {
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
            
            if(print)
            {
                cout << "Printed B+ Tree (not needed for final submission)" << endl;
                tree.printTree(tree.rootNode);
                cout << endl;
            }
            // size = sizeof(*(tree.rootNode->keyArray)) + sizeof(*(tree.rootNode->pointerArray)) + sizeof(tree.rootNode->isLeaf) + sizeof(tree.rootNode->numKeys);
            // size_t size = sizeof(tree.rootNode->isLeaf) + sizeof(tree.rootNode->numKeys) + sizeof(int*) * MAX_KEYS + sizeof(void*) * (MAX_KEYS+1);
            // cout << "Size of node: " << size << endl;
            
        }
        else if(choice == 3)
        {
            // ================================== Experiment 3 =====================================
            cout << "============ Experiment 3 ==============" << endl;
            searchTreeSingle(500, tree, disk, print);
            cout << endl;
        }
        else if(choice == 4)
        {
            // ================================== Experiment 4 =====================================
            cout << "============ Experiment 4 ==============" << endl;
            int lower = 30000;
            int upper = 40000;
            searchTreeRange(lower,upper,tree,disk, print);
        }   
        else if(choice == 5)
        {
            // ================================== Experiment 5 =====================================
            cout << "============ Experiment 5 ==============" << endl;
            // clock_t starttree, endtree;
            
            int key=1000;
            // while(true){
                // cout << "Enter a number to delete: "; 
                // cin >> key;
                key = 1000;
                cout << "DELETE numVotes with value: "<< key <<  endl;
                auto start = high_resolution_clock::now();
                tree.deleteKey(key);
                auto stop = high_resolution_clock::now();
                if(print)
                {
                    tree.printTree(tree.rootNode);
                }
            cout << "Num of nodes in tree: " << tree.numOfNodes << endl;
            cout << "Num of levels in tree: " << tree.numOfLevels << endl;
            cout << "Content of Root Node: ";
            for(int i=0; i<(tree.rootNode)->numKeys;i++)
            {
                cout << (tree.rootNode)->keyArray[i] << " ";
            }
            cout << endl;
            // double time_taken_tree = double(endtree-starttree) / double(CLOCKS_PER_SEC);
            std::chrono::duration<double, std::milli> time_taken_tree = stop-start;
            cout << "Running time of deletion using BPlusTree: " << fixed << time_taken_tree.count() << setprecision(5) << " ms" << endl;
            // }
            // clock_t startscan, endscan;
            // startscan = clock();
            auto startscan = high_resolution_clock::now();
            deletelinearscan(disk, print, key);
            auto endscan = high_resolution_clock::now();
            // double time_taken_scan = double(endscan-startscan) / double(CLOCKS_PER_SEC);
            std::chrono::duration<double, std::milli> time_taken_scan = endscan-startscan;
            cout << "Running time of deletion using Linear Scan: " << fixed << time_taken_scan.count() << setprecision(5) << " ms" << endl;

            
        }

        cout << "============================================" << endl;
        // cout << "Key in Experiment to conduct: 1, 2, 3, 4 or 5" << endl;
        // cout << "Key in 0 to exit" << endl;
        // cout << "Choice: " ;
        //cin >> choice; //comment if cycling thru all experiments
        choice++; //if cycling thru all experiments
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

    // out.close(); //closing output file
 
    return 0;
}

void deletelinearscan(StorageDisk disk, bool print, int key)
{
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
                //  delete (Record*)currentrecord;
            }
            
        }
        numofblocks++;
    }
}

void searchTreeSingle(int key, BPlusTree tree, StorageDisk disk, bool print)
{
    DataAddressList* addressList_cursor;
    int keyStruct;
    keyStruct = key;
    float totalAvg = 0;
    int numOfRecords = 0;
    int numOfIndexAccessed = 0;
    int numOfBlocksAccessed = 0;
    vector<int> blocksAlreadyAccessed; 
    // clock_t start, end;
    // clock_t startLinear, endLinear;
    
    cout << "RETRIEVAL USING BPLUSTREE:" << endl;
    cout << "Key: numVotes = " << keyStruct << endl;
    auto start = high_resolution_clock::now();
    auto [leafNode,parentNode] = tree.traverseNonLeaf(tree.rootNode, keyStruct);
    // start = clock();
    
    for(int i=0; i<leafNode->numKeys;i++)
    {
        if((leafNode->keyArray[i]) == key)
        {
            numOfIndexAccessed++;
            addressList_cursor = (DataAddressList*) leafNode->pointerArray[i];
            while(true){    // loop through the addressList for each key
                for(int j=0; j<addressList_cursor->size; j++){
                    if(print)
                    {
                        cout << "tconst: " << addressList_cursor->addressList[j]->tconst << " ";
                        cout << "avgRating: " << addressList_cursor->addressList[j]->averageRating << " ";
                        cout << "numVotes: " << addressList_cursor->addressList[j]->numVotes << endl;
                    }
                    int block;
                    block = getBlockRecordisIn(addressList_cursor->addressList[j], disk, j);
                //    cout << "BLOCK: " <<  block <<endl;
                   if (std::find(blocksAlreadyAccessed.begin(), blocksAlreadyAccessed.end(), block) != blocksAlreadyAccessed.end()) {

                    }
                    else {
                            blocksAlreadyAccessed.push_back(block);
                            numOfBlocksAccessed++;
                            // cout << "block added" << endl;
                    }

                    
                    totalAvg = totalAvg + (float)(addressList_cursor->addressList[j]->averageRating);
                    numOfRecords++;

                }
                if(addressList_cursor->nextList == NULL){   //end of linked addressList
                    break;
                }
                addressList_cursor = addressList_cursor->nextList;
                numOfIndexAccessed++;
            }
        }
    }
    // end = clock();
    auto stop = high_resolution_clock::now();
    // double time_taken = double(end-start) / double(CLOCKS_PER_SEC);
    std::chrono::duration<double, std::milli> fp_ms = stop-start;
    cout << endl;

    cout << "Number of index nodes processed: " << numOfIndexAccessed + tree.numOfLevels << endl;
    cout << "Number of data blocks processed: " << numOfBlocksAccessed << endl;
    float avgAll;
    avgAll = (float)(totalAvg/(float)(numOfRecords));
    cout << "Average of averageRatings of all returned records: " << avgAll<< endl;
    cout << "Running time of retrieval using BPlusTree: " << fixed << fp_ms.count() << setprecision(5) << " ms" << endl;

    // startLinear = clock();
    auto startLinear = high_resolution_clock::now();
    searchStorageSingle(key, disk, print);
    // endLinear = clock();
    auto endLinear = high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_taken_linear = endLinear-startLinear;
    // double time_taken_linear = double(endLinear-startLinear) / double(CLOCKS_PER_SEC);
    cout << "Running time of retrieval using Linear Search: " << fixed << time_taken_linear.count() << setprecision(5) << " ms" << endl;
    


}

void searchStorageSingle(int key, StorageDisk disk, bool print)
{
    cout << endl;
    cout << "RETRIEVAL USING LINEAR SEARCH:" << endl;
    cout << "Key: numVotes = " << key << endl;
    float totalAvg = 0;
    int numOfRecords = 0;
    int numofblocks=0;
    vector<int> blocksAlreadyAccessed;
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
                int block = getBlockRecordisIn((Record *)currentrecord, disk, j);
                //    cout << "BLOCK: " <<  block <<endl;
                   if (std::find(blocksAlreadyAccessed.begin(), blocksAlreadyAccessed.end(), block) != blocksAlreadyAccessed.end()) {

                    }
                    else {
                            blocksAlreadyAccessed.push_back(block);
                    }
                if(print)
                {
                    cout << "tcosnt: " << (*((Record *)currentrecord)).tconst << ", ";
                    cout << "avgRating: " << (*((Record *)currentrecord)).averageRating << ", ";
                    cout << "numVotes: " << (*((Record *)currentrecord)).numVotes << endl;
                }
                
                
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
void searchTreeRange(int lower, int upper, BPlusTree tree, StorageDisk disk, bool print)
{
    DataAddressList* addressList_cursor;
    bool check = true;
    float totalAvg = 0;
    int numOfRecords = 0;
    int numOfIndexAccessed = 0;
    int numOfIndex = 0;
    int numOfBlocksAccessed = 0;
    vector<int> blocksAlreadyAccessed; 
    // clock_t start, end;
    // clock_t startLinear, endLinear;
    // start = clock();
    
    cout << "RETRIEVAL USING BPLUSTREE:" << endl;
    cout << "Key: numVotes between " << lower << " and " << upper << endl;
    auto start = high_resolution_clock::now();
    auto [leafNode,parentNode] = tree.traverseNonLeaf(tree.rootNode, lower);
    while(check)
    {
        for(int i=0; i<leafNode->numKeys;i++)
        {
            if((leafNode->keyArray[i]) >= lower && (leafNode->keyArray[i]) <= upper)
            {
                numOfIndexAccessed++;
                check = true;
                addressList_cursor = (DataAddressList*) leafNode->pointerArray[i];
                while(true){    // loop through the addressList for each key
                    for(int j=0; j<addressList_cursor->size; j++){
                        if(print)
                        {
                            cout << "tconst: " << addressList_cursor->addressList[j]->tconst << " ";
                            cout << "avgRating: " << addressList_cursor->addressList[j]->averageRating << " ";
                            cout << "numVotes: " << addressList_cursor->addressList[j]->numVotes << endl;
                        }
                        int block;
                        block = getBlockRecordisIn(addressList_cursor->addressList[j], disk, j);
                        if (std::find(blocksAlreadyAccessed.begin(), blocksAlreadyAccessed.end(), block) != blocksAlreadyAccessed.end()) {

                        }
                        else {
                                blocksAlreadyAccessed.push_back(block);
                                numOfBlocksAccessed++;
                                // cout << "block added" << endl;
                        }
                        
                        totalAvg = (float)totalAvg + (float)(addressList_cursor->addressList[j]->averageRating);
                        numOfRecords++;

                    }
                    if(addressList_cursor->nextList == NULL){   //end of linked addressList
                        break;
                    }
                    addressList_cursor = addressList_cursor->nextList;
                    numOfIndexAccessed++;
                }
            }
            else
            {
                check = false;
            }
        }
        leafNode = (Node*)leafNode->pointerArray[leafNode->numKeys];
        // numOfBlocksAccessed++;
        if(check == true)
        {
            numOfIndexAccessed++;
        }
        
        
    }

    // end = clock();
    auto stop = high_resolution_clock::now();
    // double time_taken = double(end-start) / double(CLOCKS_PER_SEC);
    std::chrono::duration<double, std::milli> time_taken = stop - start;

    cout << "Number of index nodes processed: " << tree.numOfLevels + numOfIndexAccessed << endl;
    cout << "Number of data blocks processed: " << numOfBlocksAccessed << endl;
    float avgAll;
    avgAll = (float)(totalAvg/(float)(numOfRecords));
    cout << "Average of averageRatings of all returned records: " << avgAll<< endl;
    cout << "Running time of retrieval using BPlusTree: " << fixed << time_taken.count() << setprecision(5) << " ms" << endl;

    // startLinear = clock();
    auto startLinear = high_resolution_clock::now();
    searchStorageRange(lower, upper,  disk, print);
    // endLinear = clock();
    auto endLinear = high_resolution_clock::now();
    // double time_taken_linear = double(endLinear-startLinear) / double(CLOCKS_PER_SEC);
    std::chrono::duration<double, std::milli> time_taken_linear = endLinear-startLinear;
    cout << "Running time of retrieval using Linear Search: " << fixed << time_taken_linear.count() << setprecision(5) << " ms" << endl;
}

void searchStorageRange(int lower, int upper, StorageDisk disk, bool print)
{
    cout << endl;
    cout << "RETRIEVAL USING LINEAR SEARCH:" << endl;
    cout << "Key: numVotes between " << lower << " and " << upper << endl;
    float totalAvg = 0;
    int numOfRecords = 0;
    int numofblocks=0;
    vector<int> blocksAlreadyAccessed; 
    for(int i = 0; i <= disk.numUsedBlocks; i++)
    {
        void *currentrecord; // declare pointer for record to be listed
        unsigned char *selectedblockptr; // declare pointer to get block pointer to be listed
        selectedblockptr = (unsigned char *)disk.diskPtr + i*disk.blockSize; // set block pointer to specified block number
        for(int j=0;j<disk.blockSize;j=j+(sizeof(Record))) // iterate through block
        {
            currentrecord = (unsigned char *)selectedblockptr+j; // set current record (block ptr + offset)
            if((*((Record *)currentrecord)).numVotes >= lower && (*((Record *)currentrecord)).numVotes <= upper)
            {
                
                if(print)
                {
                    cout << "tcosnt: " << (*((Record *)currentrecord)).tconst << ", ";
                    cout << "avgRating: " << (*((Record *)currentrecord)).averageRating << ", ";
                    cout << "numVotes: " << (*((Record *)currentrecord)).numVotes << endl;
                }
                int block = getBlockRecordisIn((Record *)currentrecord, disk, j);
                if (std::find(blocksAlreadyAccessed.begin(), blocksAlreadyAccessed.end(), block) != blocksAlreadyAccessed.end()) {
                    
                }
                else {
                        blocksAlreadyAccessed.push_back(block);
                        // cout << "block added" << endl;
                }
                totalAvg = totalAvg + (float)((Record *)currentrecord)->averageRating;
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

int getBlockRecordisIn(Record * record, StorageDisk disk, int j)
{
    int block=0;

    while(true)
    {
        unsigned char * currentBlock;
        currentBlock = (unsigned char *)disk.diskPtr + disk.blockSize * block;
        if((unsigned int*) record < (unsigned int *)currentBlock)
        {
            break;
        }
        else if(block > disk.numUsedBlocks)
        {
            break;
        }
        else block++;
    }

    return block;
}