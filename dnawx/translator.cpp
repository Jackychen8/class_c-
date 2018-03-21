//
//  translator.cpp
//  dnawx-mac
//
//  Created by Jacky Chen on 3/24/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "translator.h"
#include <iostream>

//stateMachine reads the entire string and figures out its protein make up
void DNAtranslator::stateMachine( const std::string& content ){
    
    auto size = content.size();
    auto i = 0;
    int stateNum = 0;
    char get;
    int index = 0;
    
    while ( i < size ) {
        
        // Interpret next letter in the string
        get = content[i];
        
        
        if(get == 'T'){
            
            index = 0;

        }
        if(get == 'C'){
            
            index = 1;
            
        }
        if(get == 'A'){
            
            index = 2;
            
        }
        if(get == 'G'){
            
            index = 3;
            
        }

        if( stateArrays[stateNum][index] >= 65 && stateArrays[stateNum][index] <= 90){// ASCII for Cap Letters
            
            //stateArrays[stateNum][get] returns a letter
            ++proteinMap[stateArrays[stateNum][index]];

            stateNum = 3;
            
        }
        else if(stateArrays[stateNum][index] < 65 ){
            
            stateNum = stateArrays[stateNum][index];
        }
        
        ++i;
        
    }//end of while
    
}

//////function that fills the percentages and counts arrays//////
void DNAtranslator::calculatePercent(){
    
    float total = 0;
    float stuff[20];
    int j = 0;
    
    for(auto& i: proteinMap){
        
        stuff[j] = i.second;
        
        total = total + i.second;
        //std::cout << i.first << ": " << i.second << std::endl;//Shows contents
        ++j;
    }
    
    for(int k = 0; k < 20; ++k){
        
        percentages[k] = stuff[k]/total;
        discreteCount[k] = stuff[k];
        
    }
    
}

//function that returns the countr
int* DNAtranslator::getCount(){
    
    return discreteCount;
    
}

//returns percent
float* DNAtranslator::getPercent(){
    
    return percentages;
    
}









