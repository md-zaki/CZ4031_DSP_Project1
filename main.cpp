using namespace std;


#include "storage.h"
#include "storage.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>

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

    StorageDisk disk(100000,100);
    Record record1;
    Record record2;
    record1.averageRating = 5.5;
    record1.numVotes = 100;
    strcpy(record1.tconst,"t1000");

    record2.averageRating = 1.5;
    record2.numVotes = 200;
    strcpy(record2.tconst,"t3000");

    // === allocate block =====

    Record *recordaddress1;
    *recordaddress1 = record1;
    Record *recordaddress2;
    *recordaddress2 = record2;

    disk.currentBlockPtr = disk.diskPtr;
    





    return 0;
}