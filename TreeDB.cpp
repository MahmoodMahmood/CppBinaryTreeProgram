/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TreeDB.cpp
 * Author: mah-a
 * 
 * Created on November 18, 2016, 11:06 PM
 */

#include "TreeDB.h"
#include "TreeNode.h"
#include <iostream>

using namespace std;

TreeDB::TreeDB() {
    root = NULL;
}

TreeDB::~TreeDB() {
    while (root != NULL) {//Just delete all the entries
        remove(root->getEntry()->getName());
    }
}

// inserts the entry pointed to by newEntry into the database. 
// If an entry with the same key as newEntry's exists 
// in the database, it returns false. Otherwise, it returns true.

bool TreeDB::insert(DBentry* newEntry) {
    TreeNode* node = new TreeNode(newEntry); //Create a new node
    if (recursiveFind(root, newEntry->getName()) != NULL) {
        delete node;
        return false;
    } else {
        recursiveInsert(root, node);
        return true;
    }
}

/* This function uses recursion to insert a new node to the tree*/
void TreeDB::recursiveInsert(TreeNode* current, TreeNode* newNode) {
    if (current == NULL) {//Base case 1 (only happens for root node )
        root = newNode;
        return;
        //Otherwise insert the new node to the appropriate side
    } else if (newNode->getEntry()->getName() < current->getEntry()->getName()) {
        //If entry belongs to the left side
        if (current->getLeft() == NULL) {//Base case 2
            current->setLeft(newNode);
            return;
        }
        //If there are more nodes on the left then keep recursing
        recursiveInsert(current->getLeft(), newNode);
    } else {//If the entry belongs to the right
        if (current->getRight() == NULL) {//Base case 3
            current->setRight(newNode);
            return;
        }
        //If there are more nodes on the right then keep recursing
        recursiveInsert(current->getRight(), newNode);
    }
}

/* searches the database for an entry with a key equal to name.
 * If the entry is found, a pointer to it is returned.
 *  If the entry is not found, the NULL pointer is returned.
 */
DBentry* TreeDB::find(string name) {
    probesCount = 0; //Reset probesCount
    return recursiveFind(root, name);
}

/*This function uses recursion to find a node in the list*/
DBentry* TreeDB::recursiveFind(TreeNode* current, string name) {
    probesCount++; //increment probesCount
    if (current == NULL) {//Base case 1
        return NULL;
    }
    if (current->getEntry()->getName() == name) {//Base case 2
        return current->getEntry();
    }
    //Recurse through the left if the name would belong to the left side
    if (name < current->getEntry()->getName())
        return recursiveFind(current->getLeft(), name);
    else//Recurse through the right side if the name would belong to the right side
        return recursiveFind(current->getRight(), name);
}

// Prints the entire tree, in ascending order of key/name

ostream& operator<<(ostream& out, const TreeDB& rhs) {
    rhs.recursiveLoadStream(rhs.root, out);
    return out;
}

/*Recursively traverses tree to load output stream*/
void TreeDB::recursiveLoadStream(TreeNode* current, ostream& out) const {
    if (current == NULL) {
        return;
    }
    recursiveLoadStream(current->getLeft(), out); //Go through left branch
    out << *(current->getEntry()); //Load current branch
    recursiveLoadStream(current->getRight(), out); //Go through right branch
}

/* computes and prints out the total number of active entries
 * in the database (i.e. entries with active==true).
 */
void TreeDB::countActive() const {
    int total = recursiveCountActive(root, 0);
    cout << total << endl;
}

/*This function recursively counts the number of active items in the */
int TreeDB::recursiveCountActive(TreeNode* current, int total) const {
    if (current == NULL) {//Base case
        return 0;
    }
    int rightTotal = 0, leftTotal = 0;
    //Traverse left side if possible
    if (current->getLeft() != NULL) 
        leftTotal = recursiveCountActive(current->getLeft(), total);
    //Traverse right side if possible
    if (current->getRight() != NULL)
        rightTotal = recursiveCountActive(current->getRight(), total);
    total = rightTotal + leftTotal;
        //Increment total if it is active
    if (current->getEntry()->getActive()) total++;
    return total;
}

/*prints the number of probes stored in probesCount*/
void TreeDB::printProbes() const {
    cout << probesCount << endl;
}

/* deletes the entry with the specified name (key)  from the database.
 * If the entry was indeed in the database, it returns true.
 * Returns false otherwise.
 * See section 6 of the lab handout for the *required* removal method.
 * If you do not use that removal method (replace deleted node by
 * maximum node in the left subtree when the deleted node has two children)
 * you will not match exercise's output.
 */
bool TreeDB::remove(string name) {
    DBentry *entryToRemove = find(name); //Find the entry to remove
    TreeNode* nodeToRemove = recursiveGetNode(entryToRemove, root); //Find the node to remove
    TreeNode* nodeToSwap = NULL; //Initialize to NULL
    if (nodeToRemove->getLeft() != NULL) {//If there is a left subtree
        nodeToSwap = getMaxNode(nodeToRemove->getLeft());
    } else if (nodeToRemove->getRight() != NULL) {//If not then check right
        //If there is no left subtree then swap with the right child
        nodeToSwap = nodeToRemove->getRight();
    } else {//No children, ie: this is a leaf
        if (nodeToRemove == root) {//If this tree ONLY has a root
            root = NULL; //Set root to null
            delete nodeToRemove;
            return true;
        }
        //Set the node we want to swap with the node we want to remove
        //There will be some  unnecessary data copying but hey who cares?
        nodeToSwap = nodeToRemove;
    }
    //Grab the parent of the swapped node
    TreeNode* parent = recursiveFindParent(root, nodeToSwap);
    /* The left pointer of the swapped node MIGHT have a subtree value in it. 
     * Therefore we need to make sure that subtree is preserved by setting what 
     * used to be the pointer to the swapped node to point to the swapped node's
     * right pointer... Hope this works lol
     */
    if (parent->getLeft() == nodeToSwap) {
        parent->setLeft(nodeToSwap->getLeft());
    } else if (parent->getRight() == nodeToSwap) {
        parent->setRight(nodeToSwap->getLeft());
    }
    //Copy the values to the new entry
    nodeToRemove->getEntry()->setActive(nodeToSwap->getEntry()->getActive());
    nodeToRemove->getEntry()->setIPaddress(nodeToSwap->getEntry()->getIPaddress());
    nodeToRemove->getEntry()->setName(nodeToSwap->getEntry()->getName());

    //Delete the node
    delete nodeToSwap;
    return true;
}

/* This function takes in an entry and finds the node its in
 * We can assume that the entry does exist in the tree!
 */
TreeNode* TreeDB::recursiveGetNode(DBentry* entry, TreeNode* currentNode) {
    if (currentNode->getEntry() == entry)//Base case
        return currentNode;
    else if (currentNode->getEntry()->getName() < entry->getName())//If smaller check left tree
        return recursiveGetNode(entry, currentNode->getRight());
    else//If this node is larger, check the right branch of the tree
        return recursiveGetNode(entry, currentNode->getLeft());
}

/*This function returns the maximum node in a portion of the tree*/
TreeNode* TreeDB::getMaxNode(TreeNode* currentNode) {
    if (currentNode->getRight() == NULL)//Base case
        return currentNode;
    else//If this isnt the biggest node then keep going
        return getMaxNode(currentNode->getRight());
}

/*This function uses recursion to find a node in the list*/
TreeNode* TreeDB::recursiveFindParent(TreeNode* current, TreeNode* child) {
    string name = child->getEntry()->getName();
    if (current == NULL) {//Base case 1
        return NULL;
    }
    if (current->getRight() != NULL && current->getRight()->getEntry()->getName() == name) {//Base case 2
        return current;
    } else if (current->getLeft() != NULL && current->getLeft()->getEntry()->getName() == name) {//Base case 3
        return current;
    }
    //Recurse through the left if the name would belong to the left side
    if (name < current->getEntry()->getName())
        return recursiveFindParent(current->getLeft(), child);
    else//Recurse through the right side if the name would belong to the right side
        return recursiveFindParent(current->getRight(), child);
}