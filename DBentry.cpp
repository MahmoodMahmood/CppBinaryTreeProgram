/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DBentry.cpp
 * Author: mah-a
 * 
 * Created on November 18, 2016, 11:06 PM
 */

#include "DBentry.h"
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>

using namespace std;

DBentry::DBentry() {
}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active) {
    name = _name;
    IPaddress = _IPaddress;
    active = _active;
}

DBentry::~DBentry() {
}

// sets the domain name, which we will use as a key.

void DBentry::setName(string _name) {
    name = _name;
}

// sets the IPaddress data.

void DBentry::setIPaddress(unsigned int _IPaddress) {
    IPaddress = _IPaddress;
}

// sets whether or not this entry is active.

void DBentry::setActive(bool _active) {
    active = _active;
}

// returns the name.

string DBentry::getName() const {
    return name;
}

// returns the IPaddress data.

unsigned int DBentry::getIPaddress() const {
    return IPaddress;
}

// returns whether or not this entry is active.

bool DBentry::getActive() const {
    return active;
}

// prints the entry in the format 
// name : IPaddress : active  followed by newline
// active is printed as a string (active or inactive)
ostream& operator<< (ostream& out, const DBentry& rhs){
    string sActive;
    if (rhs.active) {
        sActive = "active";
    } else {
        sActive = "inactive";
    }
    out << rhs.name << " : " << rhs.IPaddress << " : " << sActive << std::endl;
    return out;
}