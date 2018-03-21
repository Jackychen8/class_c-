#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "op.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>

// Defines state data accessible by the machine and ops
struct MachineState// This exists for every instance of a Human/Zombie
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: m_ProgramCounter(1)
		, m_ActionsTaken(0)
		, m_Facing(UP)
		, m_Test(false)
		, m_Memory(nullptr)
	{ }

	~MachineState()
	{
		delete[] m_Memory;
	}

	// Active line number in behavior program
	int m_ProgramCounter;
	// Number of actions taken this turn
	int m_ActionsTaken;
	// Current facing of this character
	Facing m_Facing;
	// Test flag for branches
	bool m_Test;
    // Checks if machineState has died (Start out Alive)
    bool died = 0;
    // the memory that is being pointed to
    int currentMemory = 0;
    // Memory of location in the world //
    int xlocation;
    int ylocation;
    // The number of Ops that this MachineState's bound machine has
    unsigned long totalOps;
    
    
	int GetActionsPerTurn() const NOEXCEPT { return m_ActionsPerTurn; }
	int GetMaxMemory() const NOEXCEPT { return m_MaxMemory; }
	bool GetInfect() const NOEXCEPT { return m_InfectOnAttack; }
	int GetMemory(int location)
	{
        return m_Memory[location];
	}
	void SetMemory(int location, int value)
	{
		m_Memory[location] = value;
	}
    
private:
	// Data which is set by the traits
	int m_ActionsPerTurn;
	int m_MaxMemory;
	bool m_InfectOnAttack;
    int* m_Memory;
};


// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine // An instance of this exists for every set of ops
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

    long GetOps();
    
	// Destructor
	~Machine();
private:
	std::vector<Op*> m_Ops;
};


template <typename MachineTraits>
long Machine<MachineTraits>::GetOps()
{
    return m_Ops.size();
}

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)// filepath
{
    m_Ops.clear();
    std::ifstream file (filename, std::ios::in);
    std::string line;
    
    if (file.is_open())
    {
        
        while( file.eof() != true ){
            
            // 1st: get line
            std::getline(file, line);
            std::istringstream iss(line);
            std::string word;
            
            bool commentFlag = 0;
            
            while(iss >> word && commentFlag == 0) {// 2nd: get word from the line
                
                if( word.find(";") != std::string::npos){
                    
                    commentFlag = 1;
                    
                }
                
                // Checking to see if we also need to keep in mind parameter
                if( word.find(",") != std::string::npos){
                    
                    std::string firstPart;
                    std::string secondPart;
                    std::size_t pos = word.find(",");
                    
                    firstPart = word.substr(0,pos);
                    secondPart = word.substr(pos+1);
                    int second = atoi(secondPart.c_str());
                    
                    if( firstPart == "rotate" ){
                        
                        m_Ops.push_back(new OpRotate(second));//
                        
                    }
                    if( firstPart == "je" ){
                        
                        m_Ops.push_back(new OpJE(second));
                        
                    }
                    if( firstPart == "goto" ){
                        
                        m_Ops.push_back(new OpGoto(second));
                        
                    }
                    if( firstPart == "test_human" ){
                        
                        m_Ops.push_back(new OpTestHuman(second));
                        
                    }
                    if( firstPart == "test_zombie" ){
                        
                        m_Ops.push_back(new OpTestZombie(second));
                        
                    }
                    if( firstPart == "jne" ){
                        
                        m_Ops.push_back(new OpJNE(second));
                        
                    }
                    if( firstPart == "mem" ){
                        
                        m_Ops.push_back(new OpMem(second));
                        
                    }
                    if( firstPart == "set" ){
                        
                        m_Ops.push_back(new OpSet(second));
                        
                    }
                    if( firstPart == "test_mem" ){
                        
                        m_Ops.push_back(new OpTestMem(second));
                        
                    }
                }

                if( word == "forward" ){
                    
                    m_Ops.push_back(new OpForward);
                    
                }
                if( word == "test_wall" ){
                    
                    m_Ops.push_back(new OpTestWall);
                    
                }
                if( word == "test_random" ){
                    
                    m_Ops.push_back(new OpTestRandom);
                    
                }
                if( word == "attack" ){
                    
                    m_Ops.push_back(new OpAttack);
                    
                }
                if( word == "ranged_attack" ){
                    
                    m_Ops.push_back(new OpRangedAttack);
                    
                }
                if( word == "endturn" ){
                    
                    m_Ops.push_back(new OpEndTurn);
                    
                }
                if( word == "test_passable" ){
                    
                    m_Ops.push_back(new OpTestPassable);
                    
                }
                if( word == "inc" ){
                    
                    m_Ops.push_back(new OpInc);
                    
                }
                if( word == "dec" ){
                    
                    m_Ops.push_back(new OpDec);
                    
                }
                if( word == "save_loc" ){
                    
                    m_Ops.push_back(new OpSaveLoc);
                    
                }
            }//while word
            
        }//while not end of file
        file.close();
    }
    
}


template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.m_ActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.m_MaxMemory = MachineTraits::MEMORY_LOCATIONS;
	state.m_InfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
	delete state.m_Memory;
    
	state.m_Memory = new int[state.m_MaxMemory];
    state.totalOps = m_Ops.size();
    
	for (int i = 0; i < state.m_MaxMemory; i++)
	{
		state.m_Memory[i] = 0;
	}
    
}


template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	//std::cout << "TAKING TURN" << std::endl;
	state.m_ActionsTaken = 0;//
	while (state.m_ActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		m_Ops.at(state.m_ProgramCounter - 1)->Execute(state);
	}
}


template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	for (auto op : m_Ops)
	{
		delete op;
	}

	m_Ops.clear();
}
