//
//  dictionary.cpp
//  password-mac
//
//  Created by Jacky Chen on 2/17/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#include "dictionary.h"

void dictionary::fillDictionary(const std::string& dicName){
    
    std::ifstream file;
    file.open(dicName); // open up dictionary file
    std::string line;
    
    encoDictionary.rehash(100000);
    
    if(file.is_open()){
        
        Timer timer;
        timer.start();
        
        while(file.eof() != true){
            
            std::getline(file, line);
            encodingVars* entry = new encodingVars;
            entry->plainPassword = line; // read in each line (alphabetical name of password)
            
            sha1::calc(line.c_str(), line.length(), entry->hash); // hash each line
            sha1::toHexString(entry->hash, entry->hex_str);
            
            std::pair<std::string, encodingVars*> pair;// pass in values to hash map as pairs
            pair.first = std::string(entry->hex_str);
            pair.second = entry;
            encoDictionary.insert(pair);
        }
        
        double elapsed = timer.getElapsed();
        std::cout << "Creating Dictionary Time: " << elapsed << std::endl;
        
        file.close();
        std::cout << "Finished Loading Dictionary." << std::endl;
        
    }
    
    else{
        
        std::cout << "Error: File not found!" << std::endl;
        
    }
    
}

void dictionary::compareDiction(const std::string& passName){
    
    // Temporary Storage for finished portion of list
    std::map<int, listStore*> tempStore;
    std::list<listStore*> unbrokenPasswords;
    int entryNum = 0;
    
    std::ifstream file;
    file.open(passName); // open up password file
    std::string line;
    
    std::ofstream results; // create results file to output to
    results.open("pass_solved.txt");
    
    
    while(file.eof() != true){
        
        std::getline(file, line);
        
        
        if(encoDictionary.find(line) != encoDictionary.end()){//result was in Dictionary
            
            listStore* entry = new listStore;
            
            entry->plainPassword = encoDictionary[line]->plainPassword;
            entry->entryNumber = entryNum;
            
            sha1::calc(entry->plainPassword.c_str(), entry->plainPassword.length(), entry->hash);
            entry->hex_str = line;
            
            // pass in values to map as pairs
            std::pair<int, listStore*> pair;
            pair.first = entryNum;
            pair.second = entry;
            
            tempStore.insert(pair);
            
            //Part 2: results << line << ", " << encoDictionary[line]->plainPassword << std::endl;
            
            ++entryNum;
        }
        if(encoDictionary.find(line) == encoDictionary.end()){//result was not in Dictionary
            
            listStore* entry = new listStore;
            
            entry->entryNumber = entryNum;
            entry->hex_str = line;
            
            unbrokenPasswords.push_back(entry);//puts new entries at end
            
            //Part 2: results << line << ", ????????" << std::endl;
            
            ++entryNum;
        }
        
    }
    
    file.close();
    //Part 2: results.close();
    
    
    if(unbrokenPasswords.empty() != 1){

        bruteForce force;
    
        Timer timer;
        timer.start();
        
        force.count(tempStore, unbrokenPasswords);//checks list unbrokenPasswords against everything <= 4 digits
        
        double elapsed = timer.getElapsed();
        std::cout << "Decryption Time: " << elapsed << std::endl;
        
        //if it is still not empty
        if( unbrokenPasswords.empty() != 1 ){
            
            std::list<listStore*>::iterator it = unbrokenPasswords.begin();
            
            while(it != unbrokenPasswords.end()){
                
                if( tempStore.find((*it)->entryNumber) == tempStore.end()){
                    
                    (*it)->plainPassword = "??";
                    tempStore[(*it)->entryNumber] = (*it);
                    
                }
                
                ++it;
                
            }
            
        }
    }
    
    std::map<int, listStore*>::iterator it = tempStore.begin();
    
    // Outputting finished list to results file
    while(it != tempStore.end()){
        
        results << (*it).second->hex_str << "," << (*it).second->plainPassword << std::endl;
        
        delete (*it).second;//clean up memory
        ++it;
        
    }
    
    results.close();
    std::cout << "Finished and created pass_solved.txt" << std::endl;
}


void dictionary::deleteDict(){
    /*
    while( encoDictionary.empty() != 1 ){
        
        std::unordered_map<std::string, encodingVars*>::iterator it = encoDictionary.begin();
        
        while(it != encoDictionary.end()){
            
            std::cout << "Plain Password: " << (*it).second->plainPassword << std::endl;
            delete (*it).second;//delete dictionary
            ++it;
        }
        
    }*/
    
}
