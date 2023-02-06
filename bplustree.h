#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
using namespace std;

struct Key {
    float value;  // Value of key
};

class Node {
    bool isLeafNode;    // Whether node is a leaf node
    int numKeys;        // No. of keys in a node
    Key *keyArrPtr;     // Pointer to array of keys in a node
    Node **ptrArrPtr;   // Pointer to array of pointers in a node

    public:
        Node();
};

class BPlusTree {
    Node *root;  // Pointer to root node

    public:
        BPlusTree();
};

#endif