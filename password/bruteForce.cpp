//
//  bruteForce.cpp
//  password-mac
//
//  Created by Jacky Chen on 2/18/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "bruteForce.h"
#include <algorithm>


void bruteForce::count(std::map<int, listStore*>& tempStore, std::list<listStore*>& unbrokenPasswords){
    
    // Stores the converted password from passCrack
    std::string abcd;
    
    int oneDigitFlag = 0;
    int twoDigitFlag = 0;
    int threeDigitFlag = 0;
    
    while(passCrack[0]<=35){
        
        while(passCrack[1]<=35){
            
            while(passCrack[2]<=35){
             
                while(passCrack[3]<=35){
                    
                    convert(oneDigitFlag, twoDigitFlag, threeDigitFlag, abcd);
                    compareStore(tempStore, unbrokenPasswords, abcd);
                    ++passCrack[3];
                    
                }
                
                ++passCrack[2];
                passCrack[3] = 0;
                oneDigitFlag = 1;
                
            }
            
            ++passCrack[1];
            passCrack[2] = 0;
            twoDigitFlag = 1;
            
        }
        
        ++passCrack[0];
        passCrack[1] = 0;
        threeDigitFlag = 1;
        
    }
    
}

void bruteForce::convert(int oneDigitFlag, int twoDigitFlag, int threeDigitFlag, std::string& abcd){
    
    char A = digitConvert[passCrack[0]];
    char B = digitConvert[passCrack[1]];
    char C = digitConvert[passCrack[2]];
    char D = digitConvert[passCrack[3]];
    
    if(threeDigitFlag == 1){
        abcd = A;
        abcd += B;
        abcd += C;
        abcd += D;
    }
    else if(threeDigitFlag != 1 && twoDigitFlag == 1){
        abcd = B;
        abcd += C;
        abcd += D;
    }
    else if(twoDigitFlag != 1 && oneDigitFlag == 1){
        abcd = C;
        abcd += D;
    }
    else if(oneDigitFlag != 1){
        abcd = D;
    }
    
    
    
}

void bruteForce::compareStore(std::map<int, listStore*>& tempStore, std::list<listStore *>& unbrokenPasswords, std::string& abcd){
    
    std::list<listStore*>::iterator it = unbrokenPasswords.begin();
    unsigned char hash[20];
    char hex[41];
    sha1::calc(abcd.c_str(), abcd.length(), hash); // hash
    sha1::toHexString(hash, hex);
    
    //////////////////////////// FOR_EACH //////////////////////////////
    // This is slower than w/o because of the way my code is written ///
    ////// w/o is fastest, then for_each, then parallel is slowest /////
    ////////////////////////////////////////////////////////////////////
    // I know it could be faster if I implemented it at the counting ///
    // level as well, but I'm running out of time. I've got the idea ///
    // of how it works by implementing here.////////////////////////////
    ////////////////////////////////////////////////////////////////////
    /*(
    tbb::parallel_for_each( unbrokenPasswords.begin(), unbrokenPasswords.end(), [&tempStore, &hex, &hash, &abcd](listStore* it)
    {
        std::string currentTestHex = hex;
        std::string hexFromList = (it)->hex_str;
        
        if(strcmp(currentTestHex.c_str(), hexFromList.c_str()) == 0){
            
            listStore* entry = new listStore;

            entry->plainPassword = abcd;
            sha1::calc(abcd.c_str(), abcd.length(), entry->hash); // hash
            entry->hex_str = currentTestHex;
            
            tempStore[(it)->entryNumber] = entry;//enters solved password into tempStore
            
            //This also works for entering entry into tempStore
            //std::pair<int, listStore*> pair;
            //pair.first = (it)->entryNumber;
            //pair.second = entry;
            //tempStore.insert(pair);
        }
    });
*/
    
    
    ////////////// Faster without FOR_EACH vs FOR_EACH //////////////
     
    while( unbrokenPasswords.end() != it ){//iterate through list of unbroken passwords and compare
        
        std::string currentTestHex = hex;
        std::string hexFromList = (*it)->hex_str;
        
        if(strcmp(currentTestHex.c_str(), hexFromList.c_str()) == 0){
            
            listStore* entry = new listStore;

            entry->plainPassword = abcd;
            sha1::calc(abcd.c_str(), abcd.length(), entry->hash); // hash
            entry->hex_str = currentTestHex;
            
            tempStore[(*it)->entryNumber] = entry;
            
            unbrokenPasswords.erase(it);// remove solved password from list
            
        }
        
        ++it;
        
    }
    
}
