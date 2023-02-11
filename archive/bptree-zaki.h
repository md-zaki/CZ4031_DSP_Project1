#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
using namespace std;


const int MAX_KEYS = 3;


struct Key {
    int value;  // Value of key
};


class Node {
    

    public:
    bool isLeaf;  // Whether node is a leaf node
    int numKeys;  // No. of keys in a node
    Key *keyArray;  // Pointer to array of keys in a node
    Node **pointerArray;  // Pointer to array of pointers in a node
    friend class BPlusTree;
    Node();
};


class BPlusTree {
    Node *rootNode;  // Pointer to root node

    public:
        BPlusTree();
        Node *getRoot();
        void insertKey(Key key);
        void insertInternalNode(Key key, Node *parentNode, Node *childNode);
        Node *findParentNode(Node *currentNode, Node *childNode);
        void printTree(Node *currentNode);
};

#endif