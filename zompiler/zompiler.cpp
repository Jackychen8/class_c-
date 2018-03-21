// zompiler.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "node.h"
#include <fstream>
#include <sstream>

extern int g_LineNumber;
extern NBlock* g_MainBlock;

extern int zompilerparse();
extern FILE* zompilerin;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "No input file specified." << std::endl;
		return 0;
	}

	// Read input from the first command line parameter
	zompilerin = fopen(argv[1], "r");
	
	// Start the parse
	zompilerparse();
	
    CodeContext myContext;
    
	// CodeGen from g_MainBlock
    if (g_MainBlock != nullptr) {
        
        g_MainBlock->CodeGen(myContext);
    }
	
	// Close the file stream
	fclose(zompilerin);
    
    // Optimize the gotos in the map and the vector of generated code
    for (auto it = myContext.gotoMap.begin(); it != myContext.gotoMap.end(); it++) {
        
        for (auto it2 = it; it2 != myContext.gotoMap.end(); it2++) {
            
            if(it->second == it2->first){
                
                it->second = it2->second;// change it in gotoMap
                std::string gotoInstr = "goto,";
                std::ostringstream oss;
                
                oss << it2->second;
                gotoInstr += oss.str();
                
                myContext.m_Ops[(it->first)-1] = gotoInstr;// change it in m_Ops
            }
        }
    }
    
    // Write to new file out.zom
    std::ofstream fileO("out.zom");
    
    if( fileO.is_open() ){
        
        for (unsigned long i = 0; i < myContext.m_Ops.size(); ++i) {
            
            fileO << myContext.m_Ops[i] << std::endl;
        }
        
        fileO.close();
        
    };
    
	return 0;
}

void zompilererror(const char* s)
{
	std::cout << s << " on line " << g_LineNumber << std::endl;
}


