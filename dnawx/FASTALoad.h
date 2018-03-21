//
//  FASTALoad.h
//  dnawx-mac
//
//  Created by Jacky Chen on 3/15/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <fstream>
#include <map>


class fileParse {
    
public:
    
    //Constructor: Check if FASTA file is valid
    fileParse(const std::string& filePath);
    
    //Returns member variable content
    std::string showContent();
    
    //Returns member variable header
    std::string showHeader();
    
    //Returns a pointer to the header that can be shared
    std::string* getHeader();
    
    
private:
    
    std::string header;
    std::string content;
    
};





