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
    if (root == NULL) {
        root = new Node;
        root->keys[0].key_value = key_value;
        root->keys[0].add_vect.push_back(address);
        root->is_leaf = true;
        root->size = 1;
    }else{
        //Node *nodeCursor = root;

    }
}

void BPlusTree::remove_node(float key_value){

}

vector<void*> BPlusTree::find_node(float key_value){
    if (root == NULL) { //tree is empty
        cout << "Tree is empty\n";
    }else{
        Node *nodeCursor = root;
        while (nodeCursor->is_leaf == false) {  //While the current node is not leaf node, find the appropriate pointer to the children node
            for (int i = 0; i < nodeCursor->size; i++) {
                if (key_value < nodeCursor->keys[i].key_value) {
                    nodeCursor = nodeCursor->children[i];
                    break;
                }
                if (i == nodeCursor->size - 1) {
                    nodeCursor = nodeCursor->children[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < nodeCursor->size; i++) { //when leaf node is found, loop through the node to find the pointer to key value in "disk"
            if (nodeCursor->keys[i].key_value == key_value) {
                //cout << "Found\n";
                return nodeCursor->keys[i].add_vect; //return the vector of addresses which points to the records
            }
        }
        cout << "Not found\n";
    }
}
