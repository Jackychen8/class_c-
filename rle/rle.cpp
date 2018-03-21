// rle.cpp : Defines the entry point for the console application.
//

#include "rletest.h"
#include "rle-algo.hpp"
#include <iostream>
#include "rle-files.h"



void Part1Tests()
{
	TestFixtureFactory::theInstance().runTests();
}

int main(int argc, char* argv[])
{
    std::string choice;
    int numChoice;
    
    while( numChoice != 4 ){
        
        // Main entry point
        std::cout << "Select an option:" << std::endl;
        std::cout << "1. Part 1 (Tests)" << std::endl;
        std::cout << "2. Part 2 (Single Files)" << std::endl;
        std::cout << "3. Part 3 (Directories)" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "> " ;
        std::getline(std::cin, choice);
        
        while( choice != "1" && choice != "2" && choice != "3" && choice != "4" ){
            std::cout << "Invalid Choice! Input should only be 1, 2, 3, or 4. Try Again!" << std::endl << "> ";
            choice.clear();
            std::getline(std::cin, choice);
        }
        
        //String Stream or std::stoi both work
        std::stringstream(choice) >> numChoice;
        //numChoice = std::stoi(choice);
        //std::cout << "You chose " << numChoice << std::endl;
        
        
        
        if( numChoice == 1 ){
            Part1Tests();
        }
        
        int newChoice = 0;
        
        while ( numChoice == 2 && newChoice < 3 ){ // choice 3 will return to main menu and other choices will be invalid
                
                std::cout << "Select an option:" << std::endl;
                std::cout << "1. Create an Archive" << std::endl;
                std::cout << "2. Extract an Archive" << std::endl;
                std::cout << "3. Return to Main Menu" << std::endl;
                std::cout << "> ";
                std::getline(std::cin, choice);
                
                // Checks if input is valid
                while( choice != "1" && choice != "2" && choice != "3" ){
                    std::cout << "Invalid Choice! Input should only be 1, 2, or 3. Try Again!" << std::endl << "> ";
                    choice.clear();
                    std::getline(std::cin, choice);
                }
                
                std::stringstream(choice) >> newChoice;
                
                if( newChoice == 1){
                    std::cout << "What is the name of the file you would like to compress?" << std::endl;
                    std::cout << "> ";
                    std::getline(std::cin, choice);// choice is a string
                    

                    //create an instance of RLE_v1 and call the CreateArchive function with the file name as the parameter
                    RLE_v1 newArchive;
                    newArchive.CreateArchive(choice);

                }
                if( newChoice == 2){
                    std::cout << "What is the name of the file you would like to extract?" << std::endl;
                    std::cout << "> ";
                    std::getline(std::cin, choice);// choice is a string
                    
                    RLE_v1 newArchive;
                    newArchive.ExtractArchive(choice);
                    
                }
                if( newChoice == 3){// Return to Main menu
                    std::cout << "Returned to Main Menu" << std::endl;
                }
        }

        
        while ( numChoice == 3 && newChoice < 3 ){ // choice 3 will return to main menu and other choices will be invalid
            
            std::cout << "Select an option:" << std::endl;
            std::cout << "1. Create an Archive" << std::endl;
            std::cout << "2. Extract an Archive into a Directory" << std::endl;
            std::cout << "3. Return to Main Menu" << std::endl;
            std::cout << "> ";
            std::getline(std::cin, choice);
            
            // Checks if input is valid
            while( choice != "1" && choice != "2" && choice != "3" ){
                std::cout << "Invalid Choice! Input should only be 1, 2, or 3. Try Again!" << std::endl << "> ";
                choice.clear();
                std::getline(std::cin, choice);
            }
            
            std::stringstream(choice) >> newChoice;
            
            if( newChoice == 1){
                //ask for the name of a directory
                std::cout << "What is the name of the directory you would like to compress?" << std::endl;
                std::cout << "> ";
                std::getline(std::cin, choice);// choice is a string
                
                //create an instance of RLE_v2 and call the CreateArchive function with the file name as the parameter
                RLE_v2 newArchive;
                newArchive.CreateArchive(choice);
                
            }
            if( newChoice == 2){
                std::cout << "What is the name of the directory you would like to extract?" << std::endl;
                std::cout << "> ";
                std::getline(std::cin, choice);// choice is a string
                
                RLE_v2 newArchive;
                newArchive.ExtractArchive(choice);
                
            }
            if( newChoice == 3){// Return to Main menu
                std::cout << "Returned to Main Menu" << std::endl;
            }
        }
    }
	return 0;
}

