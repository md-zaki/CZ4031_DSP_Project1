#include "bplustree.h"
#include <queue>
using namespace std;


Node::Node() {
    isLeaf = true;
    numKeys = 0;
    keyArray = new Key[MAX_KEYS];
    pointerArray = new Node *[MAX_KEYS+1];
}


BPlusTree::BPlusTree() {
    rootNode = nullptr;
}

Node *BPlusTree::getRoot() { 
    return rootNode; 
}

void BPlusTree::insertKey(Key key) {
    if (rootNode == nullptr) {
        rootNode = new Node;
        rootNode->isLeaf = true;
        rootNode->numKeys = 1;
        rootNode->keyArray[0] = key;
    }
    else {
        Node *currentNode = rootNode;
        Node *parentNode;

        while (!currentNode->isLeaf) {
            parentNode = currentNode;
            for (int i=0; i<currentNode->numKeys; i++) {
                if (key.value < currentNode->keyArray[i].value) {
                    currentNode = currentNode->pointerArray[i];
                    break;
                }
                if (i == currentNode->numKeys-1) {
                    currentNode = currentNode->pointerArray[i+1];
                    break;
                }
            }
        }

        // Reached leaf node
        if (currentNode->numKeys < MAX_KEYS) {  // Current node is not full
            // Find correct position to insert new key
            int i = 0;
            while (key.value > currentNode->keyArray[i].value && i < currentNode->numKeys) {
                i++;
            }
            // Make space for new key
            for (int j=currentNode->numKeys; j>i; j--) {
                currentNode->keyArray[j] = currentNode->keyArray[j-1];
            }
            // Insert key
            currentNode->keyArray[i] = key;
            currentNode->pointerArray[currentNode->numKeys] = currentNode->pointerArray[currentNode->numKeys-1];
            currentNode->pointerArray[currentNode->numKeys-1] = nullptr;
        }
        else {  // Current node is full
            // Create temporary array of keys and insert new key into it
            Key tempKeyArray[MAX_KEYS+1];
            for (int i=0; i<MAX_KEYS; i++) {
                tempKeyArray[i] = currentNode->keyArray[i];
            }
            int i = 0;
            while (key.value > tempKeyArray[i].value) {
                i++;
            }
            for (int j=MAX_KEYS; j>i; j--) {
                tempKeyArray[j] = tempKeyArray[j-1];
            }
            tempKeyArray[i] = key;

            // Split current node into two
            Node *newLeafNode = new Node;
            newLeafNode->isLeaf = true;
            currentNode->numKeys = (MAX_KEYS+1) / 2;
            newLeafNode->numKeys = MAX_KEYS+1 - (MAX_KEYS+1)/2;
            // Rearrangement between two nodes
            currentNode->pointerArray[currentNode->numKeys] = newLeafNode;
            newLeafNode->pointerArray[newLeafNode->numKeys] = currentNode->pointerArray[MAX_KEYS];
            currentNode->pointerArray[MAX_KEYS] = nullptr;
            for (int i=0; i<currentNode->numKeys; i++) {
                currentNode->keyArray[i] = tempKeyArray[i];
            }
            for (int i=0, j=currentNode->numKeys; i<newLeafNode->numKeys; i++, j++) {
                newLeafNode->keyArray[i] = tempKeyArray[j];
            }

            if (currentNode == rootNode) {
                // If current node is root node, create new root node
                Node *newRootNode = new Node;
                newRootNode->keyArray[0] = newLeafNode->keyArray[0];
                newRootNode->pointerArray[0] = currentNode;
                newRootNode->pointerArray[1] = newLeafNode;
                newRootNode->isLeaf = false;
                newRootNode->numKeys = 1;
                rootNode = newRootNode;
            }
            else {
                // Insert first key of new leaf node into parent node
                insertInternalNode(newLeafNode->keyArray[0], parentNode, newLeafNode);
            }
        }
    }
}

void BPlusTree::insertInternalNode(Key key, Node *parentNode, Node *childNode) {
    if (parentNode->numKeys < MAX_KEYS) {
        // If parent node is not full, insert new key into it
        int i = 0;
        while (key.value > parentNode->keyArray[i].value && i < parentNode->numKeys) {
            i++;
        }
        for (int j=parentNode->numKeys; j>i; j--) {
            parentNode->keyArray[j] = parentNode->keyArray[j-1];
            parentNode->pointerArray[j+1] = parentNode->pointerArray[j];
        }
        parentNode->keyArray[i] = key;
        parentNode->numKeys++;
        parentNode->pointerArray[i+1] = childNode;
    }
    else {  // Parent node is full
        // Create temporary arrays of keys and pointers with space for extra key and pointer
        Key tempKeyArray[MAX_KEYS+1];
        Node *tempPointerArray[MAX_KEYS+2];

        // Copy keys and pointers of parent node into temporary arrays
        for (int i=0; i<MAX_KEYS; i++) {
            tempKeyArray[i] = parentNode->keyArray[i];
            tempPointerArray[i] = parentNode->pointerArray[i];
        }
        tempPointerArray[MAX_KEYS] = parentNode->pointerArray[MAX_KEYS];

        // Insert new key and pointer into temporary arrays
        int i = 0;
        int j;
        while (key.value > tempKeyArray[i].value && i < MAX_KEYS) {
            i++;
        }
        for (j=MAX_KEYS+1; j>i; j--) {
            tempKeyArray[j] = tempKeyArray[j-1];
        }
        tempKeyArray[i] = key;
        for (j=MAX_KEYS+2; j>i+1; j--) {
            tempPointerArray[j] = tempPointerArray[j-1];
        }
        tempPointerArray[i+1] = childNode;

        // Split parent node into two
        Node *newInternalNode = new Node;
        newInternalNode->isLeaf = false;
        parentNode->numKeys = (MAX_KEYS+1) / 2;
        newInternalNode->numKeys = MAX_KEYS - (MAX_KEYS+1)/2;
        for (i=0, j=parentNode->numKeys+1; i<newInternalNode->numKeys; i++, j++) {
            newInternalNode->keyArray[i] = tempKeyArray[j];
            newInternalNode->pointerArray[i] = tempPointerArray[j];
        }
        newInternalNode->pointerArray[newInternalNode->numKeys] = tempPointerArray[j];

        if (parentNode == rootNode) {
            // If parent node is root node, create new root node
            Node *newRootNode = new Node;
            newRootNode->keyArray[0] = parentNode->keyArray[parentNode->numKeys];
            newRootNode->pointerArray[0] = parentNode;
            newRootNode->pointerArray[1] = newInternalNode;
            newRootNode->isLeaf = false;
            newRootNode->numKeys = 1;
            rootNode = newRootNode;
        }
        else {
            insertInternalNode(parentNode->keyArray[parentNode->numKeys], findParentNode(rootNode, parentNode), newInternalNode);
        }
    }
}

Node *BPlusTree::findParentNode(Node *currentNode, Node *childNode) {
    // Use DFS to find parent node
    Node *parentNode;
    if (currentNode->isLeaf || (currentNode->pointerArray[0])->isLeaf) {
        return nullptr;
    }
    for (int i=0; i<currentNode->numKeys+1; i++) {
        if (currentNode->pointerArray[i] == childNode) {
            parentNode = currentNode;
            return parentNode;
        }
        else {
            parentNode = findParentNode(currentNode->pointerArray[i], childNode);
            if (parentNode != nullptr) {
                return parentNode;
            }
        }
    }
    return parentNode;
}

void BPlusTree::printTree(Node *currentNode) {
    if (currentNode == nullptr) {
        return;
    }

    queue<Node *> q;
    q.push(currentNode);
    while (!q.empty()) {
        int queueSize = q.size();
        for (int i=0; i<queueSize; i++) {
            Node *node = q.front();
            q.pop();
            for (int j=0; j<node->numKeys; j++) {
                cout << node->keyArray[j].value << " ";
            }
            for (int j=0; j<node->numKeys+1; j++) {
                if (node->pointerArray[j] != nullptr) {
                    q.push(node->pointerArray[j]);
                }
            }
            cout << "\t";
        }
        cout << endl;
    }
}