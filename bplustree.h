#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
using namespace std;

// Structure to store key and its associated addresses
struct Keys {
    float key_value;
    vector<void*> add_vect; //for duplicate keys
};

class Node {
public:
    bool is_leaf; // Whether node is a leaf node
    Keys *keys; //Pointer to keys struct
    int size; //size of keys in the node
    Node** children; //Pointer to array of pointers in a node

    Node();
};

class BPlusTree {
    Node *root;  // Pointer to root node

    public:

        BPlusTree(); //constructor for B+ tree

        void insert_node(float key_value, void *address); 

        void remove_node(float key_value); 

        vector<void*>find_node(float key_value);
};

#endif