//
//  NeedlemanWunsch.cpp
//  dnawx-mac
//
//  Created by Jacky Chen on 3/24/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "NeedlemanWunsch.h"

//Constructor
//Checks if strings are valid FASTA
//Resizes the two Vector of Vectors
NeedWun::NeedWun(std::string& A, std::string& B){
    
    fileParse file1(A);
    fileParse file2(B);
    
    content1 = file1.showContent();
    content2 = file2.showContent();
    header1 = file1.showHeader();
    header2 = file2.showHeader();
    
    paths.resize(content1.length() + 1);//# of columns
    scores.resize(content1.length() + 1);
    
    for (int i = 0; i<=content1.length(); ++i) {
        
        paths[i].resize(content2.length() + 1);//# of rows
        scores[i].resize(content2.length() + 1);
        
    }
    
};

// Populates the score/direction grid, runs the algorithm, and produces the resultant sequence strings
void NeedWun::FindWun(){
    
    int i = 0;//column index
    int j = 1;//row index
    
    int leng1 = int (content1.length());
    int leng2 = int (content2.length());
    
    ////////////////populating with 0's////////////////////////
    while(i <= leng1){
        
        scores[i][0] = -i;//going through row 0
        paths[i][0] = left;
        ++i;
        
    }
    while(j <= leng2){
        
        scores[0][j] = -j;//going through column 0
        paths[0][j] = up;
        ++j;
        
    }//////////////////////////////////////////////////////////
    

    ////// Compute the possible scores and select best ///////
    j = 1;

    while(j <= leng2){
        
        i = 1;//start in column [1] each time and go through the row
        int match;
        
        while(i <= leng1){
            
            if(content1[i-1] == content2[j-1]){//the content index is one less than the grid dimensions
                
                match = 1;
                
            }
            else{
                
                match = -1;
                
            }
            
            int upLeftScore = scores[i-1][j-1] + match;
            int leftScore = scores[i-1][j] - 1;
            int upScore = scores[i][j-1] - 1;
            
            // First, check if topleft is the best
            if( (upLeftScore >= leftScore) && (upLeftScore >= upScore) ){
                
                scores[i][j] = upLeftScore;
                paths[i][j] = upleft;
                
            }
            // Then, check if left is best
            else if( leftScore >= upScore){
                
                scores[i][j] = leftScore;
                paths[i][j] = left;
                
            }
            // Last, top must be best
            else{
                
                scores[i][j] = upScore;
                paths[i][j] = up;

            }
            ++i;
        }
        ++j;
        
    }//Finished computing all possible scores and filling the grid
    
    //Find the best Path
    int x = leng1;
    int y = leng2;
    
    while( x != 0 || y != 0){
        
        switch(paths[x][y]){
                
            case(0):
                stringA += content1[x-1];
                stringB += content2[y-1];
                --x;
                --y;
                break;
                
            case(1):
                stringA += content1[x-1];
                stringB += '_';
                --x;
                break;
            case(2):
                stringB += content2[y-1];
                stringA += '_';
                --y;
                break;
        }
    }
    
    std::reverse(std::begin(stringA), std::end(stringA));
    std::reverse(std::begin(stringB), std::end(stringB));
    
};

// Writes results of the match to the .result file
void NeedWun::WriteWun(std::string& fileName){
    
    std::ofstream results;
    results.open(fileName);
    
    results << "A: " << header1 << std::endl;
    results << "B: " << header2 << std::endl;
    results << "Score: " << scores[content1.length()][content2.length()] << std::endl << std::endl;//Extra Line
    
    int x = 0;
    int i = 0;
    int y = 0;
    char matchLine[70];
    
    while(x < stringA.length()){
        
        y = x;
        
        //////Print StringA (70 char)////////
        while(i < 70 && x < stringA.length()){

            results << stringA[x];
            
            if(stringA[x] == stringB[x]){
                
                matchLine[i] = '|';
                
            }
            else{
                matchLine[i] = ' ';
            }
            
            ++i;
            ++x;
        }
        
        results << std::endl;
        i = 0;
        x = y;//reset and let the last loop's value persist
        
        ////////Print matchLine (70 char)/////////
        while(i < 70 && x < stringA.length()){
            
            results << matchLine[i];
            ++i;
            ++x;
            
        }
        
        results << std::endl;
        i = 0;
        x = y;
        
        /////////Print stringB (70 char)////////////
        while(i < 70 && x < stringA.length()){
            
            results << stringB[x];
            ++i;
            ++x;
            
        }
        
        results << std::endl;
        results << std::endl;// Extra Line
        i = 0;
        
    }
    
    results.close();
    std::cout << "Finished writing to File" << std::endl;
    
};
