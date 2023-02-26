#include "rewrite.h"
#include "storage.h"
#include <queue>
#include <tuple>
#include <cmath>
using namespace std;


Node::Node() {
    isLeaf = true;
    numKeys = 0;
    keyArray = new int[MAX_KEYS];
    pointerArray = new void *[MAX_KEYS+1]();
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
            DataAddressList * dummyPtrArray[MAX_KEYS+2];
            createDummyArrays(key, leafNode, dummyKeyArray, dummyPtrArray, recAddress); // create dummy key array with new key inserted

            // Split current node into two
            auto[firstNode, secondNode] = splitLeafNode(dummyKeyArray,dummyPtrArray,leafNode);
            numOfNodes++;

            if (firstNode == rootNode) {
                // If current node is root node, create new root node
                Node *newRootNode = new Node;
                newRootNode->keyArray[0] = secondNode->keyArray[0];
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
{
    // find correct postion to insert new key
    // int position=0;
    // while(key.value > leafNode->keyArray[position].value && position < leafNode->numKeys)
    // {
    //     position++;
    // }

    // if(key.value == leafNode->keyArray[position].value)
    // {
    // }
    // else
    // {

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
    leafNode->pointerArray[leafNode->numKeys+1] = leafNode->pointerArray[leafNode->numKeys];
    for (int nodeindex=leafNode->numKeys; nodeindex>position; nodeindex--) 
    {
        leafNode->keyArray[nodeindex] = leafNode->keyArray[nodeindex-1]; // move all keys to make space
        leafNode->pointerArray[nodeindex] = leafNode->pointerArray[nodeindex-1];
    }
    //insert key
    leafNode->keyArray[position] = key;
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
    tempPtrArray[MAX_KEYS] = (DataAddressList*)leafNode->pointerArray[MAX_KEYS];
    int index = 0;
    while (key > tempKeyArray[index]) {
        index++; // find postion to insert key in temp array
        if(index == MAX_KEYS)
        {
            break;
        }
    }

    tempPtrArray[MAX_KEYS+1] = tempPtrArray[MAX_KEYS];
    for (int j=MAX_KEYS; j>index; j--) {
        tempKeyArray[j] = tempKeyArray[j-1]; // make space in temp array for key
        tempPtrArray[j] = tempPtrArray[j-1]; // make space in temp array for ptr
    }
    tempKeyArray[index] = key; // insert key in temp array
    tempPtrArray[index] = new DataAddressList;
    tempPtrArray[index]->addressList[0] = (Record*)recAddress;
    tempPtrArray[index]->size++;
}




tuple<Node*,Node*> BPlusTree::splitLeafNode(int dummyKeyArray[],DataAddressList* dummyPtrArray[], Node* leafNode)
{
    // create second node after splitting
    Node *secondNode = new Node;
    secondNode->isLeaf = true;
    // split current leaf node in half
    leafNode->numKeys = ceil(float((MAX_KEYS+1)) / 2.0);
    // set second node numKeys to max keys minus current node numkeys
    secondNode->numKeys = floor(float((MAX_KEYS+1)) / 2.0);

    // Assign new pointers in pointer array to both nodes from dummy ptr array
    
    for(int i=0; i<leafNode->numKeys; i++)
    {
        leafNode->pointerArray[i] = dummyPtrArray[i];
    }
    for (int i=0, j=leafNode->numKeys; i<secondNode->numKeys; i++, j++) {
        secondNode->pointerArray[i] = dummyPtrArray[j];
    }

    leafNode->pointerArray[leafNode->numKeys] = secondNode; // last pointer of leaf node point to next leaf node
    secondNode->pointerArray[secondNode->numKeys] = leafNode->pointerArray[MAX_KEYS]; // second node point to next node

    //Assign values of keys to both nodes from dummy key array
    for (int i=0; i<leafNode->numKeys; i++) {
    leafNode->keyArray[i] = dummyKeyArray[i];
    }
    for (int i=0, j=leafNode->numKeys; i<secondNode->numKeys; i++, j++) {
        secondNode->keyArray[i] = dummyKeyArray[j];
    }

    return {leafNode, secondNode};
}




void BPlusTree::insertIntoNonLeaf(int key, Node *parentNode, Node *childNode) {
    if (parentNode->numKeys < MAX_KEYS) {
        // If parent node is not full, insert new key into it
        
        int i = 0;
        while (key > parentNode->keyArray[i] && i < parentNode->numKeys) {
            i++;
            if(i == parentNode->numKeys)
            {
                break;
            }
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
        while (key > tempKeyArray[i] && i < MAX_KEYS) {
            i++;
        }
        for (j=MAX_KEYS; j>i; j--) {
            tempKeyArray[j] = tempKeyArray[j-1];
        }
        tempKeyArray[i] = key;
        for (j=MAX_KEYS+1; j>i+1; j--) {
            tempPointerArray[j] = tempPointerArray[j-1];
        }
        tempPointerArray[i+1] = childNode;

        // cout << "tempkeyarray" <<endl;
        // for(int z=0; z<MAX_KEYS+1;z++)
        // {
        //     cout << tempKeyArray[z].value << endl;
        // }

        // Split parent node into two
        Node *newInternalNode = new Node;
        newInternalNode->isLeaf = false;
        parentNode->numKeys = ceil((float(MAX_KEYS)) / 2.0);
        newInternalNode->numKeys = floor(float((MAX_KEYS)) / 2.0);
        
        int keyForParent = tempKeyArray[parentNode->numKeys];

        for (i=0, j=parentNode->numKeys+1; i<newInternalNode->numKeys; i++, j++) {
            newInternalNode->keyArray[i] = tempKeyArray[j];
            newInternalNode->pointerArray[i+1] = tempPointerArray[j+1]; //TIM CHANGE
        }
        //newInternalNode->pointerArray[newInternalNode->numKeys] = tempPointerArray[j];
        newInternalNode->pointerArray[0] = tempPointerArray[parentNode->numKeys+1];

        for(i=0; i<parentNode->numKeys; i++){
            parentNode->keyArray[i] = tempKeyArray[i];
            parentNode->pointerArray[i] = tempPointerArray[i];
        }
        parentNode->pointerArray[parentNode->numKeys] = tempPointerArray[parentNode->numKeys];
        numOfNodes++;

        if (parentNode == rootNode) {
            // If parent node is root node, create new root node
            Node *newRootNode = new Node;
            newRootNode->keyArray[0] = keyForParent;//parentNode->keyArray[parentNode->numKeys];
            newRootNode->pointerArray[0] = parentNode;
            newRootNode->pointerArray[1] = newInternalNode;
            newRootNode->isLeaf = false;
            newRootNode->numKeys = 1;
            rootNode = newRootNode;
            numOfNodes++;
            numOfLevels++;
        }
        else {
            insertIntoNonLeaf(keyForParent, findParentNode(rootNode, parentNode), newInternalNode);
        }
    }
}




Node *BPlusTree::findParentNode(Node *currentNode, Node *childNode) {
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
            //cout << "--------KEY IS " << first_leaf_node->keyArray[i].value << "----------" << endl;
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


void BPlusTree::deleteKey(int key){
    if (rootNode == NULL) {
        cout << "Tree empty\n";
        return;
    }

    auto [leafNode, parentNode] = traverseNonLeaf(rootNode,key);
    int leftSiblingIndex;
    int rightSiblingIndex;
    for(int i = 0; i < parentNode->numKeys; i++){   //loop to find left and right siblings
        if(key < parentNode->keyArray[i]){
            rightSiblingIndex = i+1;
            leftSiblingIndex = i-1;
            break;
        }
        if(i == (parentNode->numKeys)-1) // if iterate until last key
        {
            leftSiblingIndex = i;
            rightSiblingIndex = i+2;
            break;
        }
    }

    bool exist = false;
    int position=0;
    while(key > leafNode->keyArray[position] && position < leafNode->numKeys)   //find the position for key
    {
        position++;
    }
    if(leafNode->keyArray[position] == key){    //check if key exists
        exist = true;
    }

    if(!exist){ //key does not exist, return
        return;
    }

    deleteAddressList((DataAddressList*)leafNode->pointerArray[position]); //de-allocate the memory for addresslist
    for(int i = position; i < leafNode->numKeys-1; i++){    //shift keys and pointers
        leafNode->keyArray[i] = leafNode->keyArray[i + 1];
        leafNode->pointerArray[i] = leafNode->pointerArray[i +1];
    }
    leafNode->pointerArray[leafNode->numKeys-1] = leafNode->pointerArray[leafNode->numKeys];
    leafNode->numKeys -= 1;

    if(leafNode == rootNode && leafNode->numKeys == 0){ //delete the root node if size is empty
        delete[] leafNode->keyArray;
        delete[] leafNode->pointerArray;
        delete leafNode;
        rootNode = NULL;
        return;
    }

    if(leafNode->numKeys >= floor(float(MAX_KEYS+1)/2.0)){   //Case 1: if leaf node has more than minimum required keys, simply delete it
        if(position == 0 && leftSiblingIndex >= 0){
            parentNode->keyArray[leftSiblingIndex] = leafNode->keyArray[0];   //update parent if first key of the leafnode was deleted
        }
        return;
    }

    if(leftSiblingIndex >= 0){  //If left sibling exist, borrow one key
        Node* leftSibling = (Node*)parentNode->pointerArray[leftSiblingIndex];

        if(leftSibling->numKeys > floor(float(MAX_KEYS+1)/2.0)){    //check if sibling node has enough keys to borrow
            leafNode->pointerArray[leafNode->numKeys+1] = leafNode->pointerArray[leafNode->numKeys];
            for (int i = leafNode->numKeys; i > 0; i--) {    //shift keys so that we can borrow from sibling node
                leafNode->keyArray[i] = leafNode->keyArray[i - 1];
                leafNode->pointerArray[i] = leafNode->pointerArray[i - 1];
            }
            leafNode->numKeys++;
            
            leafNode->keyArray[0] = leftSibling->keyArray[leftSibling->numKeys - 1];    //insert key borrowed from left sibling
            leafNode->pointerArray[0] = leftSibling->pointerArray[leftSibling->numKeys-1];
            leftSibling->numKeys--;
            leftSibling->pointerArray[leftSibling->numKeys] = leftSibling->pointerArray[leftSibling->numKeys+1];    //shift Node pointer
            leftSibling->pointerArray[leftSibling->numKeys + 1] = NULL;
            parentNode->keyArray[leftSiblingIndex] = leafNode->keyArray[0]; //update parent
            return;
        }
    }

    if(rightSiblingIndex <= parentNode->numKeys){   //if right sibling exists, borrow one key from it
        Node* rightSibling = (Node*)parentNode->pointerArray[rightSiblingIndex];
        if(rightSibling->numKeys > floor(float(MAX_KEYS+1)/2.0)){   //check if right sibling has enough keys to borrow
            leafNode->keyArray[leafNode->numKeys] = rightSibling->keyArray[0];
            leafNode->pointerArray[leafNode->numKeys+1] = leafNode->pointerArray[leafNode->numKeys];   //shifting node pointer which points to the next leaf node
            leafNode->pointerArray[leafNode->numKeys] = rightSibling->pointerArray[0];
            leafNode->numKeys++;

            for(int i = 0; i < rightSibling->numKeys-1; i++){ //shift keys in right sibling after borrowing one key
                rightSibling->keyArray[i] = rightSibling->keyArray[i+1];
                rightSibling->pointerArray[i] = rightSibling->pointerArray[i+1];
            }
            rightSibling->pointerArray[rightSibling->numKeys-1] = rightSibling->pointerArray[rightSibling->numKeys];
            rightSibling->numKeys--;

            parentNode->keyArray[rightSiblingIndex - 1] = rightSibling->keyArray[0];    //update parent
            return;
        }
    }

    if(leftSiblingIndex >= 0){
        Node *leftSibling = (Node*)parentNode->pointerArray[leftSiblingIndex];
        for(int i = leftSibling->numKeys, j = 0; j < leafNode->numKeys; i++, j++) { //merge leaf node with left sibling
            leftSibling->keyArray[i] = leafNode->keyArray[j];
            leftSibling->pointerArray[i] = leafNode->pointerArray[j];
        }
        leftSibling->numKeys += leafNode->numKeys;
        leftSibling->pointerArray[leftSibling->numKeys] = leafNode->pointerArray[leafNode->numKeys];

        delete[] leafNode->keyArray;
        delete[] leafNode->pointerArray;
        delete leafNode;

        removeInternal(parentNode->keyArray[leftSiblingIndex], parentNode, leafNode);
    }else if(rightSiblingIndex <= parentNode->numKeys){
        Node* rightSibling = (Node*)parentNode->pointerArray[rightSiblingIndex];
        for (int i = leafNode->numKeys, j = 0; j < rightSibling->numKeys; i++, j++) {   //merge leaf node with right sibling
            leafNode->keyArray[i] = rightSibling->keyArray[j];
            leafNode->pointerArray[i] = rightSibling->pointerArray[j];
        }
        leafNode->numKeys += rightSibling->numKeys;
        leafNode->pointerArray[leafNode->numKeys] = rightSibling->pointerArray[rightSibling->numKeys];

        delete[] rightSibling->keyArray;
        delete[] rightSibling->pointerArray;
        delete rightSibling;

        removeInternal(parentNode->keyArray[rightSiblingIndex - 1], parentNode, rightSibling);
    }
}

void BPlusTree::removeInternal(int key, Node* parent, Node* leaf){
    return;
}

void BPlusTree::deleteAddressList(DataAddressList* addressList){
    DataAddressList* addressList_cursor = addressList;
    DataAddressList* nextAddressList;
    while(true){    // loop through the addressList
        nextAddressList = addressList_cursor->nextList;
        delete addressList_cursor;
        if(nextAddressList == NULL){   //end of linked addressList
            break;
        }
        addressList_cursor = nextAddressList;
    }
}