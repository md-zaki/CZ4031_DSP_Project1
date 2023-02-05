#include <vector>
#include <cmath>

const int MAX_KEYS = 50;

// Structure to store key and its associated addresses
struct Keys {
    float key_value;
    std::vector<void*> add_vect;
};

class BPTree;

// Definition of a node in the B+ tree
class Node {
public:
    bool is_leaf;
    Keys *keys;
    int size;
    Node** children;

    Node() {
        // Allocate memory for keys and children
        keys = new Keys[MAX_KEYS];
        children = new Node*[MAX_KEYS + 1];
        // Set the node as a leaf node
        is_leaf = true;
        // Initialize the size of the node
        size = 0;
    }
};

class BPTree {
public:
    Node *root;

    BPTree() {
        root = nullptr;
    }

    void insert_node(float key_value, void *address) {
        // TODO: implement insert node function
        
    }

    void remove_node(float key_value) {
        // TODO: implement remove node function
    }

    void *find_node(float key_value) {
        // TODO: implement find node function
        return nullptr;
    }
};
