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
    fstream fin;

  
    int linenum, indexnum = 0;
    Record record;
    record.averageRating=0;
    record.numVotes=0;
    strcpy(record.tconst,"000000000");
    ifstream testData("data/dataSmall.tsv");
    if (testData.is_open()){
        string line,index;
        getline(testData,line);
        while(getline(testData,line,'\n'))
        {
            stringstream s(line);
            
            while(getline(s,index,'\t'))
            {
                if (indexnum == 0)
                {
                    strcpy(record.tconst, index.c_str());
                }
                else if (indexnum ==1)
                {
                    record.averageRating=stof(index);
                }
                else if(indexnum == 2)
                {
                    record.numVotes=stoi(index);
                }
                indexnum = indexnum + 1;
            }
            linenum = linenum + 1;
            indexnum = 0;

            cout << "tconst: "<<record.tconst << endl;
            cout << "averagerating: "<<record.averageRating << endl;
            cout << "numvotes: "<<record.numVotes << endl;
        }


        
    }



    return 0;
}