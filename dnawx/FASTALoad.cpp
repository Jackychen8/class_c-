//
//  FASTALoad.cpp
//  dnawx-mac
//
//  Created by Jacky Chen on 3/15/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "FASTALoad.h"
#include "Exceptions.h"
#include <iostream>

//Constructor: Check if FASTA file is valid
fileParse::fileParse(const std::string& filePath){
    
    std::ifstream::pos_type size;
    
    std::ifstream file (filePath, std::ios::in|std::ios::ate);
    
    std::string line;
    
    if (file.is_open())
    {
        size = file.tellg();//Find the size
        content.reserve(size);//reserve so you don't have to constantly resize
        
        file.seekg (0, std::ios::beg); // Seek back to start of file
        
        //get Header
        std::getline(file, line);
        
        if(line.size() >= 2){
            
            header = line.substr(1,line.size() - 1);
            
        }
        else{
            
            header = line;
            
        }
        
        bool match = true;
        char got;
        
        // Check if all characters are T,C,A, or G
        while(file.eof() != true && match == true){
            
            got = file.get();
            
            if( got != 'T' && got != 'C' && got != 'A' && got != 'G' && got != '\n' && got != '\r' && got != '\xff'){

                match = false;
                
                throw new FileLoadExcept;
                
                }
            if( got == 'T' || got == 'C' || got == 'A' || got == 'G'){
                
                content += got;
                
            }
            
        }//end of while
        
    }//end of if
    
}


std::string fileParse::showContent(){
    return content;
}

std::string* fileParse::getHeader(){
    return &header;
};

std::string  fileParse::showHeader(){
    return header;
};








