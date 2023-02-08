#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class Node {
    bool is_leaf; // check if node is the lowest level
    int *keyArray, size; // pointer to array of keys
    Node **ptrArray; // pointer to array of pointers
    friend class BPTree;

    public:
    Node(); // constructor of class node
};

class BPTree {
    Node *root;

    public:
        BPTree();
        void search(int);
        void insert(int);
        Node *getRoot();

};