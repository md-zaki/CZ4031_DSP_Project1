#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>
using namespace std;


const int MAX_KEYS = 3;


class Node {
    

    public:
    bool isLeaf;  // Whether node is a leaf node
    int numKeys;  // No. of keys in a node
    //Key *keyArray;  // Pointer to array of keys in a node
    int *keyArray; //tim
    //Node **pointerArray;  // Pointer to array of pointers in a node
    void **pointerArray;  //Tim
    friend class BPlusTree;
    Node();
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
        void insertleaf(int key, Node* leafNode, void* recAddress);
        //void createDummyArrays(Key key, Node* leafNode, Key tempKeyArray[], node* tempPtrArray[], void* recAddress);
        void createDummyArrays(int key, Node* leafNode, int tempKeyArray[], vector<int*>* tempPtrArray[], void* recAddress); //tim
        //tuple<Node*,Node*> splitLeafNode(Key dummyKeyArray[],Node* dummyPtrArray[], Node* leafNode);
        tuple<Node*,Node*> splitLeafNode(int dummyKeyArray[],vector<int*>* dummyPtrArray[], Node* leafNode); //tim
        bool find_node(int key_value);
};

#endif