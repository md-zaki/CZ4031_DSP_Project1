#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
using namespace std;


const int MAX_KEYS = 3;
const int MAX_ADDRESSES = 5;


struct Key {
    int value;  // Value of key
};


class Node {
    

    public:
    bool isLeaf;  // Whether node is a leaf node
    int numKeys;  // No. of keys in a node
    Key *keyArray;  // Pointer to array of keys in a node
    void **pointerArray;  // Pointer to array of pointers in a node
    friend class BPlusTree;
    Node();
};

class DataAddressList{
    public:
    int size;
    Record* addressList[MAX_ADDRESSES];
    DataAddressList* nextList;
    DataAddressList();
};

class BPlusTree {
    

    public:
        Node *rootNode;  // Pointer to root node
        int numOfNodes = 0;
        int numOfLevels = 0;

        BPlusTree();
        Node *getRoot();
        void insertion(Key key, void* recAddress);
        void insertIntoNonLeaf(Key key, Node *parentNode, Node *childNode);
        Node *findParentNode(Node *currentNode, Node *childNode);
        void printTree(Node *currentNode);
        tuple<Node*,Node*> traverseNonLeaf(Node *rootNode, Key key);
        void insertleaf(Key key, Node* leafNode, void* recAddress, int position);
        void createDummyArrays(Key key, Node* leafNode, Key tempKeyArray[], DataAddressList* tempPtrArray[], void* recAddress);
        tuple<Node*,Node*> splitLeafNode(Key dummyKeyArray[],DataAddressList* dummyPtrArray[], Node* leafNode);
};

#endif