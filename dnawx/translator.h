//
//  translator.h
//  dnawx-mac
//
//  Created by Jacky Chen on 3/24/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include <stdio.h>

class DNAtranslator {
    
public:
    
    //take in "content" of fileParse class instance
    //start the state machine and translate the string to protein
    void stateMachine( const std::string& content );
    
    //returns a pointer to the first float in the member variable percentages[]
    float* getPercent();
    
    //returns a pointer to the first int of the member variable discreteCount[]
    int* getCount();
    
    //Fills percentages[] and discreteCount[] with floats and ints, respectively
    void calculatePercent();
    
private:
    
    //Counts for all of the different proteins produced
    std::map<char, int> proteinMap = {{'F',0}, {'L',0}, {'I',0}, {'M',0}, {'V',0}, {'S',0}, {'P',0}, {'T',0}, {'A',0}, {'Y',0}, {'H',0}, {'Q',0}, {'N',0}, {'K',0}, {'D',0}, {'E',0}, {'C',0}, {'W',0}, {'R',0}, {'G',0}};
    
    //Look Up Array of Arrays
    char stateArrays[24][4] {
        
        //stateArrays[state#][T,C,A,or G correspond to the indexes 0,1,2,and 3]
        {0, 0, 1, 0},
        {2, 0, 1, 0},
        {0, 0, 1, 'M'},
        {4, 9, 14, 19},
        {5, 6, 7, 8},
        {'F', 'F', 'L', 'L'},
        {'S', 'S', 'S', 'S'},
        {'Y', 'Y', 0, 0},
        {'C', 'C', 0, 'W'},
        {10, 11, 12, 13},
        {'L','L','L','L'},
        {'P','P','P','P'},
        {'H','H','Q','Q'},
        {'R','R','R','R'},
        {15,16,17,18},
        {'I','I','I','M'},
        {'T','T','T','T'},
        {'N','N','K','K'},
        {'S','S','R','R'},
        {20,21,22,23},
        {'V','V','V','V'},
        {'A','A','A','A'},
        {'D','D','E','E'},
        {'G','G','G','G'}
    };
    
    float percentages[20];
    int discreteCount[20];
};

