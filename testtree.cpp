using namespace std;


#include "storage.h"
#include "storage.cpp"
#include "bptree-zaki.h"
#include "bptree-zaki.cpp"
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
    BPlusTree tree;
    Key key1;
    key1.value = 1;
    Key key2;
    key2.value = 2;
    Key key3;
    key3.value = 3;
    Key key4;
    key4.value = 4;
    Key key5;
    key5.value = 5;
    tree.insertKey(key2);
    tree.insertKey(key1);
    tree.insertKey(key3);
    tree.insertKey(key4);
    tree.insertKey(key5);
    tree.insertKey(key1);
    tree.printTree(tree.getRoot());
    // Node *node = tree.getRoot();
    // cout << node->numKeys << endl;
    return 0;
}