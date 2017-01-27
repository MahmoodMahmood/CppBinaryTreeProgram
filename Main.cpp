/* 
 * File:   main.cpp
 * Author: mah-a
 *
 * Created on November 18, 2016, 11:03 PM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include "DBentry.h"
#include "TreeDB.h"

using namespace std;
void insert(stringstream& sStream, TreeDB& tree);
void find(stringstream& sStream, TreeDB& tree);
void printprobes(stringstream& sStream, TreeDB& tree);
void updateStatus(stringstream& sStream, TreeDB& tree);
void remove(stringstream& sStream, TreeDB& tree);

int main(int argc, char** argv) {
    string line, command;
    // May have some setup code here
    cout << "> ";
    getline(cin, line); // Get first line from standard input
    TreeDB tree = TreeDB(); //Create the tree
    while (!cin.eof()) {
        // Put the line in a stringstream for parsing
        // Making a new stringstream for each line so flags etc. are in a known state
        stringstream sStream(line);
        sStream >> command;
        if (command == "insert") {
            insert(sStream, tree); //Call the insert function
        } else if (command == "find") {
            find(sStream, tree); //Call the find function
        } else if (command == "remove") {
            remove(sStream, tree);//Call the remove function
        } else if (command == "printall") {
            cout << tree;
        } else if (command == "printprobes") {
            printprobes(sStream, tree); //Call the print probes function
        } else if (command == "removeall") {
            tree.~TreeDB();
            cout << "Success" << endl;
        } else if (command == "countactive") {
            tree.countActive();
        } else if (command == "updatestatus") {
            updateStatus(sStream, tree);//Call the update status function
        } else {
            cout << "Error: invalid command" << endl;
        }
        line = "";
        command = "";
        cout << "> ";
        getline(cin, line);
    } // End input loop until EOF.

    return 0;
}

/*This function deals with the insertR command*/
void insert(stringstream& sStream, TreeDB& tree) {
    //Declare variables
    string name, sActive;
    unsigned int IPaddress;
    bool active = false;
    sStream >> name >> IPaddress >> sActive;
    if (sActive == "active") {//convert "active" to bool
        active = true;
    }
    //make new entry and insert it
    DBentry *entry = new DBentry(name, IPaddress, active);
    if (tree.insert(entry)) 
        cout << "Success" << endl;
    else
        cout << "Error: entry already exists" << endl;
}

void find(stringstream& sStream, TreeDB& tree) {
    //Grab the name from the stream
    string name;
    sStream >> name;
    //Find the correct variable
    DBentry* entry = tree.find(name);
    if (entry == NULL)
        cout << "Error: entry does not exist" << endl;
    else
        cout << *entry;
}

void printprobes(stringstream& sStream, TreeDB& tree) {
    //Declare variables
    string name;
    sStream >> name;
    if (tree.find(name) == NULL)
        cout << "Error: entry does not exist" << endl;
    else
        tree.printProbes();
}

void updateStatus(stringstream& sStream, TreeDB& tree) {
    //Grab the name from the stream
    bool active = false;
    string name, sActive;
    sStream >> name >> sActive;
    //Find the correct variable
    DBentry* entry = tree.find(name);
    if (sActive == "active") {//convert "active" to bool
        active = true;
    }
    if (entry == NULL)//Check if the thing exists
        cout << "Error: entry does not exist" << endl;
    else {
        entry->setActive(active);//Change entry active status
        cout << "Success" << endl;
    }
}

void remove(stringstream& sStream, TreeDB& tree) {
    //Declare variables
    string name;
    sStream >> name;
    if (tree.find(name) == NULL)
        cout << "Error: entry does not exist" << endl;
    else {
        tree.remove(name);
        cout << "Success" << endl;
    }
}
