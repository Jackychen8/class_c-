//
//  bruteForce.h
//  password-mac
//
//  Created by Jacky Chen on 2/18/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "dictionary.h"
#include <tbb/parallel_for_each.h>

struct listStore{
    
    int entryNumber;
    std::string plainPassword;
    unsigned char hash[20];
    std::string hex_str;
    
};

class bruteForce{//Magic
    
public:
    
    // Name: Convert
    // Function: converts a normal character into the hex_str version
    // Input: integers to keep track of whether you are creating 4, 3, 2, or 1 digit numbers
    // Output: none
    void convert(int, int, int, std::string& abcd);
    
    // Name: Count
    // Function: counts and carries each digit at index 35, also calls Convert and compareStore
    // so basically everything happens from here.
    // Input: the list of unbroken passwords and the map which you push solved passwords to
    // Output: none
    void count(std::map<int, listStore*>& tempStore, std::list<listStore*>& unbrokenPasswords);
    
    // Name: compareStore
    // Function: compares each of the items in the unsolved passwords list with the current count
    // and stores them in map if they match
    // Input: the map to store it in, the list of unsolved passwords, the current string being compared
    // Output: none
    void compareStore(std::map<int, listStore*>& tempStore, std::list<listStore*>& unbrokenPasswords, std::string& abcd);
    
private:
    
    //Need
    int passCrack[4] = {0, 0, 0, 0};
    
    //Array of 36 integers: can also populate with ASCII
    char digitConvert[36] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't','u', 'v', 'w', 'x','y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    // Stores the converted password from passCrack
    // std::string abcd;
    
};




