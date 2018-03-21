//
//  NeedlemanWunsch.h
//  dnawx-mac
//
//  Created by Jacky Chen on 3/24/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <vector>
#include "FASTALoad.h"
#include <iostream>
#include <algorithm>

enum direction{ upleft, left, up };

class NeedWun{
    
public:
    
    //Constructor:
    //Checks if strings are valid FASTA
    //Resizes the two Vector of Vectors
    NeedWun(std::string& A, std::string& B);
    
    // Populates the score/direction grid, runs the algorithm, and produces the shortest path
    void FindWun();
    
    // Writes results of the match to the .result file
    void WriteWun(std::string& fileName);
    
    
private:
    
    //2 grids (vectors of vectors) to hold directions and scores
    std::vector<std::vector<direction>> paths;
    std::vector<std::vector<short>> scores;
    
    //Other Member Variables
    std::string header1;
    std::string header2;
    std::string content1;
    std::string content2;
    
    std::string stringA;
    std::string stringB;
    
};


