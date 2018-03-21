//
//  dictionary.h
//  password-mac
//
//  Created by Jacky Chen on 2/15/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include "unordered_map"
#include <string>
#include "map"
#include "list"
#include <iostream>
#include "timer.h"
#include "sha1.h"
#include <fstream>
#include "bruteForce.h"
#include <iterator>

struct encodingVars{
    std::string plainPassword;
    unsigned char hash[20];
    char hex_str[41];
};


class dictionary{
    
public:
    
    // Name: fillDictionary
    // Function: fill a map (encoDictionary) with passwords from a file to check common passwords
    // Input: the name of the dictionary file
    // Output: none
    void fillDictionary(const std::string& dicName);
    
    // Name: compareDiction
    // Function: Basically does everything.
    // This runs through all of the passwords in encoDictionary. Then it calls a function from bruteForce
    // to run through all 4 digit permutations of lower case letters and numbers.
    // Then it creates the pass_solved.txt file
    // Input: the name of the file containing all the passwords you want to crack
    // Output: none
    void compareDiction(const std::string& passName);
    
    // Name: delete dictionary
    // Function: runs through all the dictionary's .seconds (encodingVars*)
    // and deletes them
    // Input: none
    // Output: none
    void deleteDict();
 
private:
    
    // Stores the "most often used" passwords Dictionary that is matched with passwords
    std::unordered_map<std::string, encodingVars*> encoDictionary;
    
};
