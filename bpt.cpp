#include <vector>
#include <cmath>
#include "bplustree.h"

const int MAX_KEYS = 50;

Node::Node(){
    this->keys = new Keys[MAX_KEYS];
    this->children = new Node*[MAX_KEYS + 1];
    this->is_leaf = true;
    this->size = 0;
}

BPlusTree::BPlusTree(){
    this->root = NULL;
}

void BPlusTree::insert_node(float key_value, void *address){

}

void BPlusTree::remove_node(float key_value){

}

void* BPlusTree::find_node(float key_value){

};
