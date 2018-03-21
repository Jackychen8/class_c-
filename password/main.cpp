// password.cpp : Defines the entry point for the console application.
//
#include<iostream>
#include"sha1.h"
#include"dictionary.h"
#include"timer.h"
#include"bruteForce.h"

#include<sstream>
#include<fstream>


/////////////////// Option 2 from Main Menu ///////////////////
void loadDictionary(int& numChoice, dictionary& myDictionary){
    
    std::string decision;
    std::cout << "Would you like to use the default password dictionary? (y/n)" << std::endl;
    std::getline(std::cin, decision);
    
    while( decision != "y" && decision != "n"){
        std::cout << "Invalid Choice! Input should only be y or n. Try Again!" << std::endl << "> ";
        decision.clear();
        std::getline(std::cin, decision);
    }
    
    std::string dicName;
    
    if(decision == "y"){
        //load d8.txt, default library
        dicName = "d8.txt";
    }
    if(decision == "n"){
        //custom library
        std::cout << "What password dictionary would you like to use?" << std::endl;
        std::getline(std::cin, dicName);
    }
    
    myDictionary.fillDictionary(dicName);
    
    numChoice = 0;
}

/////////////////// Option 3 from Main Menu ///////////////////
void decryptDictionary(int& numChoice, dictionary& myDictionary){
    
    std::string decision;
    std::cout << "Would you like to use the default password file? (y/n)" << std::endl;
    std::getline(std::cin, decision);
    
    while( decision != "y" && decision != "n"){
        std::cout << "Invalid Choice! Input should only be y or n. Try Again!" << std::endl << "> ";
        decision.clear();
        std::getline(std::cin, decision);
    }
    
    std::string passName;
    
    if(decision == "y"){
        //load d8.txt, default library
        passName = "pass.txt";
    }
    
    if(decision == "n"){
        //custom File
        std::cout << "What password file would you like to use?" << std::endl;
        std::getline(std::cin, passName);
    }
    
    myDictionary.compareDiction(passName);
    
    numChoice = 0;
}


/////////////////////////////// Start Here ///////////////////////////////////
int main(int argc, char* argv[])
{
    
    std::string choice;//// Stores Answer for chosen option
    int numChoice = 0;//// Tracks current Menu Option
    int dicFlag = 0;//// Tracks whether or not dictionary has been loaded
    
    dictionary myDictionary;// custom dictionary class instance
    
    while( numChoice !=4 ){
        
        /////////////////// MAIN MENU ///////////////////
        std::cout << "Select an option:" << std::endl;
        std::cout << "1. Basic Hashing" << std::endl;
        std::cout << "2. Load Dictionary" << std::endl;
        std::cout << "3. Decrypt" << std::endl;
        std::cout << "4. Exit" << std::endl << "> ";
        std::getline(std::cin, choice);
        
        //////////////////// INPUT CHECKING ////////////////////
        while( choice != "1" && choice != "2" && choice != "3" && choice != "4" ){
            std::cout<< "Invalid Choice! Input should only be 1, 2, 3, or 4. Try Again!" << std::endl << "> ";
            choice.clear();
            std::getline(std::cin, choice);
        }
    
        std::stringstream(choice) >> numChoice;
        
        while( numChoice == 1){
            std::string sample;
            std::cout << "What password would you like to hash?" << std::endl;
            std::getline(std::cin, sample);
            
            
            unsigned char hash[20];
            sha1::calc(sample.c_str(), sample.length(), hash);
            
            char hex_str[41];
            sha1::toHexString(hash, hex_str);
            
            std::cout << "Hashed: " << hex_str << std::endl;
        }
        
        // Load Dictionary
        while( numChoice == 2 ){
            
            loadDictionary(numChoice, myDictionary);
            dicFlag = 1;
            
        }

        if(numChoice == 3 && dicFlag == 0){
            
            std::cout << "You must load a dictionary before you can start decrypting." << std::endl;
            
        }
        // Decryption
        while( numChoice == 3 && dicFlag == 1){
            
            
            decryptDictionary(numChoice, myDictionary);
            
        }
        //Remember to delete Dictionary
    }
    
    myDictionary.deleteDict();
    std::cout << "Deleted dictionary" << std::endl;

	return 0;
}

