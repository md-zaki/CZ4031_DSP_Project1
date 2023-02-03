using namespace std;


#include "storage.h"
#include "storage.cpp"
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
    // fstream fin;

  
    // int linenum, indexnum = 0;
    // Record record;
    // record.averageRating=0;
    // record.numVotes=0;
    // strcpy(record.tconst,"000000000");
    // ifstream testData("data/dataSmall.tsv");
    // if (testData.is_open()){
    //     string line,index;
    //     getline(testData,line);
    //     while(getline(testData,line,'\n'))
    //     {
    //         stringstream s(line);
            
    //         while(getline(s,index,'\t'))
    //         {
    //             if (indexnum == 0)
    //             {
    //                 strcpy(record.tconst, index.c_str());
    //             }
    //             else if (indexnum ==1)
    //             {
    //                 record.averageRating=stof(index);
    //             }
    //             else if(indexnum == 2)
    //             {
    //                 record.numVotes=stoi(index);
    //             }
    //             indexnum = indexnum + 1;
    //         }
    //         linenum = linenum + 1;
    //         indexnum = 0;

    //         cout << "tconst: "<<record.tconst << endl;
    //         cout << "averagerating: "<<record.averageRating << endl;
    //         cout << "numvotes: "<<record.numVotes << endl;
    //     }

        
    // }
  
    StorageDisk disk(100000000,200);
    //================= Simulate reading from file ============
    Record record1;
    Record record2;
    record1.averageRating = 5.5;
    record1.numVotes = 100;
    strcpy(record1.tconst,"s1000");

    record2.averageRating = 1.5;
    record2.numVotes = 200;
    strcpy(record2.tconst,"s3000");
    
    // === allocate block =====\

    int offset = 0; 

    void *temp_rec_add = (unsigned char *)disk.diskPtr + (disk.numUsedBlocks*disk.blockSize) + offset; //set current record address
    for(int i=0;i<2;i++)
    {
        if(i==0)
        {
            memcpy(temp_rec_add, &record1, sizeof(record1));
        }
        else{
            memcpy(temp_rec_add, &record2, sizeof(record2));
        }

        offset = offset + sizeof(record1);
        temp_rec_add = (unsigned char *)disk.diskPtr + (disk.numUsedBlocks*disk.blockSize) + offset;


        
    }
    offset = 0;
    void *currentrecord;

    
    currentrecord = (unsigned char *)disk.diskPtr+offset;

    cout << "Extracted average rating: " << (*((Record *)currentrecord)).tconst << endl;
    return 0;
}