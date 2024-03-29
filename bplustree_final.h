#ifndef BPLUSTREE_FINAL_H
#define BPLUSTREE_FINAL_H

#include <iostream>
#include "storage.h"
using namespace std;


const int MAX_KEYS = 15;
const int MAX_ADDRESSES = 23;


class Node {
    

    public:
    bool isLeaf;  // Whether node is a leaf node
    int numKeys;  // No. of keys in a node
    int *keyArray;  // Pointer to array of keys in a node
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
        void insertion(int key, void* recAddress);
        void insertIntoNonLeaf(int key, Node *parentNode, Node *childNode);
        Node *findParentNode(Node *currentNode, Node *childNode);
        void printTree(Node *currentNode);
        tuple<Node*,Node*> traverseNonLeaf(Node *rootNode, int key);
        void insertleaf(int key, Node* leafNode, void* recAddress, int position);
        void createDummyArrays(int key, Node* leafNode, int tempKeyArray[], DataAddressList* tempPtrArray[], void* recAddress);
        tuple<Node*,Node*> splitLeafNode(int dummyKeyArray[],DataAddressList* dummyPtrArray[], Node* leafNode);
        void deleteKey(int key);
        void deleteAddressList(DataAddressList* addressList);
        void removeInternal(int key, Node* parent, Node* leaf);
        tuple<int,int> getSiblingIndexLeaf(Node *parentNode, int key);
        void replaceRootNode(Node* parent, Node* leaf, Node* rootNode);
        tuple<int,int> getSiblingIndexNonLeaf(Node *parent, Node* parentOfParent);
        tuple<Node*,Node*, int> splitNonLeafNode(int tempKeyArray[],Node* tempPointerArray[], Node* parentNode, int i, int j);
};

#endif