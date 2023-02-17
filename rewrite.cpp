#include "rewrite.h"
#include <queue>
#include <tuple>
#include <vector>
using namespace std;


Node::Node() {
    isLeaf = true;
    numKeys = 0;
    keyArray = new int[MAX_KEYS];
    pointerArray = new void* [MAX_KEYS+1]();
}

DataAddressList::DataAddressList(){
    size = 0;
    nextList = NULL;
}

BPlusTree::BPlusTree() {
    rootNode = nullptr;
}

//==========================================================================

void BPlusTree::insertion(int key, void* recAddress)
{
    if (rootNode == nullptr) // first insertion
    {
        rootNode = new Node;
        rootNode->isLeaf = true;
        rootNode->numKeys = rootNode->numKeys + 1;
        rootNode->keyArray[0] = key;
        rootNode->pointerArray[0] = new DataAddressList;
        ((DataAddressList*)rootNode->pointerArray[0])->addressList[0] = (Record*)recAddress;
        ((DataAddressList*)rootNode->pointerArray[0])->size++;
        numOfNodes++;
        numOfLevels++;
    }
    else
    {
        // traverse tree until find a leaf node for key
        auto [leafNode, parentNode] = traverseNonLeaf(rootNode,key); //returns leafNode for key to be inserted into and its parent node
        
        //check if key exists and retrieve the position to insert to leaf node
        //ADDED BY TIM
        bool exist = false;
        int position=0;
        while(key > leafNode->keyArray[position] && position < leafNode->numKeys)
        {
            position++;
        }
        if(leafNode->keyArray[position] == key){
            exist = true;
        }

        // Reached leaf
        if(leafNode->numKeys < MAX_KEYS || exist) // if current node has space
        {
            insertleaf(key, leafNode, recAddress, position); // insert key into specified leafNode
        }
        else // if current node is full
        {   
            //create tempkeyarray
            int dummyKeyArray[MAX_KEYS+1];
            DataAddressList* dummyPtrArray[MAX_KEYS+2];
            createDummyArrays(key, leafNode, dummyKeyArray, dummyPtrArray, recAddress); // create dummy key array with new key inserted

            // Split current node into two
            auto[firstNode, secondNode] = splitLeafNode(dummyKeyArray,dummyPtrArray,leafNode);
            numOfNodes++;

            if (firstNode == rootNode) {
                // If current node is root node, create new root node
                Node *newRootNode = new Node;
                newRootNode->keyArray[0] = firstNode->keyArray[0];
                newRootNode->pointerArray[0] = firstNode;
                newRootNode->pointerArray[1] = secondNode;
                newRootNode->isLeaf = false;
                newRootNode->numKeys = 1;
                rootNode = newRootNode;
                numOfNodes++;
                numOfLevels++;
            }
            else {
                // Insert first key of new leaf node into parent node
                insertIntoNonLeaf(secondNode->keyArray[0], parentNode, secondNode);
            }
        }
    }
}


tuple<Node*,Node*> BPlusTree::traverseNonLeaf(Node *rootNode, int key)
{
    Node *currentNode = rootNode;
    Node *parent;

    while(currentNode->isLeaf == false)
    {
        parent = currentNode;
        for(int index=0; index<(currentNode->numKeys);index++) // check root node first
        {
            if (key < currentNode->keyArray[index]) // if key to be inserted is less than key in index
            {
                currentNode = (Node*)currentNode->pointerArray[index]; // set current node to pointer in root node corresponding to key i
                break;
            }
            if(index == (currentNode->numKeys)-1) // if iterate until last key
            {
                currentNode = (Node*)currentNode->pointerArray[index+1]; // set current node to node pointed by last pointer
                break;
            }
        }
    }

    // return leaf node and parent
    return {currentNode,parent};
}



void BPlusTree::insertleaf(int key, Node* leafNode, void* recAddress, int position)
{   //TIM DELETED THIS PART ABOUT FINDING THE POSITION OF LEAFNODE TO INSERT AND SHIFTED IT TO THE OUTER FUNCTION
    if(leafNode->keyArray[position] == key){    //if key already exist, then add the record address to the addressList
        DataAddressList* cursor = (DataAddressList*)leafNode->pointerArray[position];
        while(cursor->nextList != NULL){    //perform while loop to point to the last addressList
            cursor = cursor->nextList;
        }
        if(cursor->size == MAX_ADDRESSES){  //if the addressList is full, create new addressList and insert new record
            cursor->nextList = new DataAddressList;
            DataAddressList* newList = cursor->nextList;
            newList->addressList[0] = (Record*)recAddress;
            newList->size++;
        }else{  //else insert record directly into addressList
            cursor->addressList[cursor->size] = (Record*)recAddress;
            cursor->size++;
        }
        
        return;
    }
    // Make space for new key
    for (int nodeindex=leafNode->numKeys; nodeindex>position; nodeindex--) 
    {
        leafNode->keyArray[nodeindex] = leafNode->keyArray[nodeindex-1]; // move all keys to make space
        leafNode->pointerArray[nodeindex] = leafNode->pointerArray[nodeindex-1];
    }
    //insert key
    leafNode->keyArray[position] = key;
    leafNode->pointerArray[leafNode->numKeys] = leafNode->pointerArray[leafNode->numKeys-1];
    leafNode->numKeys = leafNode->numKeys + 1; 
    leafNode->pointerArray[position] = new DataAddressList;
    ((DataAddressList*)leafNode->pointerArray[position])->addressList[0] = (Record*)recAddress;
    ((DataAddressList*)leafNode->pointerArray[position])->size++;
}



void BPlusTree::createDummyArrays(int key, Node* leafNode, int tempKeyArray[], DataAddressList* tempPtrArray[], void* recAddress)
{
    // Create temporary array of keys and insert new key into it
    for (int index=0; index<MAX_KEYS; index++) {
        tempKeyArray[index] = leafNode->keyArray[index]; // transfer keys to temp array
        tempPtrArray[index] = (DataAddressList*)leafNode->pointerArray[index];
    }
    int index = 0;
    while (key > tempKeyArray[index]) {
        index++; // find postion to insert key in temp array
    }
    for (int j=MAX_KEYS; j>index; j--) {
        tempKeyArray[j] = tempKeyArray[j-1]; // make space in temp array for key
        tempPtrArray[j] = tempPtrArray[j-1]; // make space in temp array for ptr
    }
    tempKeyArray[index] = key; // insert key in temp array
    tempPtrArray[index] = new DataAddressList;
    tempPtrArray[index]->addressList[0] = (Record*)recAddress;
    tempPtrArray[index]->size++;
}



tuple<Node*,Node*> BPlusTree::splitLeafNode(int dummyKeyArray[], DataAddressList* dummyPtrArray[], Node* leafNode)
{
    // create second node after splitting
    Node *secondNode = new Node;
    secondNode->isLeaf = true;
    // split current leaf node in half
    leafNode->numKeys = (MAX_KEYS+1) / 2;
    // set second node numKeys to max keys minus current node numkeys
    secondNode->numKeys = (MAX_KEYS+1) - (leafNode->numKeys);

    // Assign new pointers in pointer array to both nodes from dummy ptr array
    
    for(int i=0; i<leafNode->numKeys; i++)
    {
        leafNode->keyArray[i] = dummyKeyArray[i];
        leafNode->pointerArray[i] = dummyPtrArray[i];
    }
    for (int i=0, j=leafNode->numKeys; i<secondNode->numKeys; i++, j++) {
        secondNode->keyArray[i] = dummyKeyArray[j];
        secondNode->pointerArray[i] = dummyPtrArray[j];
    }

    leafNode->pointerArray[leafNode->numKeys] = secondNode; // last pointer of leaf node point to next leaf node
    secondNode->pointerArray[secondNode->numKeys] = leafNode->pointerArray[MAX_KEYS]; // second node point to next node

    return {leafNode, secondNode};
}




void BPlusTree::insertIntoNonLeaf(int key, Node *parentNode, Node *childNode) {
    if (parentNode->numKeys < MAX_KEYS) {
        // If parent node is not full, insert new key into it
        int i = 0;
        while (key > parentNode->keyArray[i] && i < parentNode->numKeys) {
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
        int tempKeyArray[MAX_KEYS+1];
        Node *tempPointerArray[MAX_KEYS+2];

        // Copy keys and pointers of parent node into temporary arrays
        for (int i=0; i<MAX_KEYS; i++) {
            tempKeyArray[i] = parentNode->keyArray[i];
            tempPointerArray[i] = (Node*)parentNode->pointerArray[i];
        }
        tempPointerArray[MAX_KEYS] = (Node*)parentNode->pointerArray[MAX_KEYS];

        // Insert new key and pointer into temporary arrays
        int i = 0;
        int j;
        while (key > tempKeyArray[i] && i < MAX_KEYS) { //could be merged with loop above
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
        numOfNodes++;

        if (parentNode == rootNode) {
            // If parent node is root node, create new root node
            Node *newRootNode = new Node;
            newRootNode->keyArray[0] = parentNode->keyArray[parentNode->numKeys];
            newRootNode->pointerArray[0] = parentNode;
            newRootNode->pointerArray[1] = newInternalNode;
            newRootNode->isLeaf = false;
            newRootNode->numKeys = 1;
            rootNode = newRootNode;
            numOfNodes++;
            numOfLevels++;
        }
        else {
            insertIntoNonLeaf(parentNode->keyArray[parentNode->numKeys], findParentNode(rootNode, parentNode), newInternalNode);
        }
    }
}




Node *BPlusTree::findParentNode(Node *currentNode, Node *childNode) {   //could utilise key to find parent node
    // Use DFS to find parent node
    Node *parentNode;
    if (currentNode->isLeaf || ((Node*)currentNode->pointerArray[0])->isLeaf) {
        return nullptr;
    }
    for (int i=0; i<currentNode->numKeys+1; i++) {
        if (currentNode->pointerArray[i] == childNode) {
            parentNode = currentNode;
            return parentNode;
        }
        else {
            parentNode = findParentNode((Node*)currentNode->pointerArray[i], childNode);
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
    bool found_first_leaf = false;
    Node* first_leaf_node; 
    q.push(currentNode); // push current node into queue
    while (!q.empty()) // while queue not empty
    {
        int queueSize = q.size();
        for (int i=0; i<queueSize; i++) // iterate through queue
        {
            Node *node = q.front(); // get the first node in queue
            q.pop(); // remove first node in queue
            for (int j=0; j<node->numKeys; j++) 
            {
                cout << node->keyArray[j] << " "; // print all keys in current node
            }
            for (int j=0; j<node->numKeys+1; j++) 
            {
                if (node->isLeaf) // if node is leaf, break
                {   
                    if(!found_first_leaf){ 
                        found_first_leaf = true;
                        first_leaf_node = node; //Assign first leaf node, TIM ADDED THIS TO PRINT LINKED ADDRESSLIST
                    }
                    break;
                }
                else if (node->pointerArray[j] != nullptr) {
                    q.push((Node*)node->pointerArray[j]);
                }
            }
            cout << "\t";
        }
        cout << endl;
    }

    //TIM ADDED THIS CHUNK BELOW FOR PRINTING THE LINKED ADDRESSLIST
    DataAddressList* addressList_cursor; 
    Node* next_node;
    while(true){    //keep looping through leaf nodes
        next_node = (Node*)first_leaf_node->pointerArray[first_leaf_node->numKeys];

        for(int i=0; i<first_leaf_node->numKeys; i++){  //loop through the each keys in the leaf node
            addressList_cursor = (DataAddressList*) first_leaf_node->pointerArray[i];
            
            while(true){    // loop through the addressList for each key
                for(int j=0; j<addressList_cursor->size; j++){
                    cout << addressList_cursor->addressList[j]->tconst << " ";
                    cout << addressList_cursor->addressList[j]->averageRating << " ";
                    cout << addressList_cursor->addressList[j]->numVotes << endl;
                }
                if(addressList_cursor->nextList == NULL){   //end of linked addressList
                    break;
                }
                addressList_cursor = addressList_cursor->nextList;
            }   
        }

        if(next_node == NULL){
            break;
        }
        first_leaf_node = next_node;
        cout << endl;
    }
}