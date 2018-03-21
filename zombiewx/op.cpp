#include "op.h"
#include "machine.h"
#include <iostream>
#include "world.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
    if( m_Param >= 0 && m_Param < 1000 ){// There are no params over 1000
        
        std::cout << state.m_ProgramCounter << ":" << m_OpName << "," << m_Param << std::endl;
        
    }
    else{
        
        std::cout << state.m_ProgramCounter << ":" << m_OpName << ", no Param" << std::endl;
        
    }
    
}

void OpRotate::Execute(MachineState& state)
{
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::RIGHT;
		}
		else
		{
			state.m_Facing = MachineState::LEFT;
		}
							break;
	case (MachineState::RIGHT) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::DOWN;
		}
		else
		{
			state.m_Facing = MachineState::UP;
		}
							   break;
	case (MachineState::DOWN) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::LEFT;
		}
		else
		{
			state.m_Facing = MachineState::RIGHT;
		}
							  break;
	default:
	case (MachineState::LEFT) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::UP;
		}
		else
		{
			state.m_Facing = MachineState::DOWN;
		}
							  break;
	}
    
    /*std::cout << "X Position: " << state.xlocation << ", Y Postion: " << state.ylocation << std::endl;
	std::cout << "Now facing ";
	switch (state.m_Facing)
	{
	case (MachineState::UP):
	std::cout << "UP";
	break;
	case (MachineState::RIGHT):
	std::cout << "RIGHT";
	break;
	case (MachineState::DOWN):
	std::cout << "DOWN";
	break;
	case (MachineState::LEFT):
	std::cout << "LEFT";
	break;
	}
	std::cout << std::endl;*/
    
	state.m_ProgramCounter++;
	state.m_ActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
    
	state.m_ProgramCounter = m_Param;

    try{
        
        if( m_Param > state.totalOps || m_Param < 1){
            throw InvalidOp();
        }
    }
    catch(InvalidOp()){
        std::cout << "The parameter was out of range. \nReset parameter to 1."<< std::endl;
    };
}

void OpForward::Execute(MachineState &state){

    switch(state.m_Facing){
        case (MachineState::UP) :
            
            if( state.ylocation != 0 && World::get().board[state.xlocation][state.ylocation-1] == nullptr ){
                
                World::get().board[state.xlocation][state.ylocation] = nullptr;
                --state.ylocation;
                World::get().board[state.xlocation][state.ylocation] = &state;
                
               }
            
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation != 19 && World::get().board[state.xlocation + 1][state.ylocation] == nullptr){
            
                World::get().board[state.xlocation][state.ylocation] = nullptr;
                ++state.xlocation;
                World::get().board[state.xlocation][state.ylocation] = &state;
                
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation != 19 && World::get().board[state.xlocation][state.ylocation+1] == nullptr){
                
                World::get().board[state.xlocation][state.ylocation] = nullptr;
                ++state.ylocation;
                World::get().board[state.xlocation][state.ylocation] = &state;
                
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation != 0 && World::get().board[state.xlocation-1][state.ylocation] == nullptr){
            
                World::get().board[state.xlocation][state.ylocation] = nullptr;
                --state.xlocation;
                World::get().board[state.xlocation][state.ylocation] = &state;
            
            }
                
            break;
    };
    
    std::cout << "X Position: " << state.xlocation << ", Y Postion: " << state.ylocation << std::endl;
    state.m_ProgramCounter++;
    state.m_ActionsTaken++;
    
}

void OpTestWall::Execute(MachineState &state){
    
    state.m_Test = 0;
    
    // Check position
    if(state.xlocation == 0 || state.ylocation == 0 || state.xlocation == 19 || state.ylocation == 19){
        
        switch(state.m_Facing){// Check facing
            case (MachineState::UP) :
                
                if(state.ylocation == 0){
                    
                    state.m_Test = 1;// testFlag = 1 or 0
                    
                }
                
                break;
            case (MachineState::RIGHT) :
                
                if(state.xlocation == 19){
                    
                    state.m_Test = 1;
                    
                }
                
                break;
            case (MachineState::DOWN) :
                
                if(state.ylocation == 19){
                    
                    state.m_Test = 1;
                    
                }
                
                break;
            default:
            case (MachineState::LEFT) :
                
                if(state.xlocation == 0){
                    
                    state.m_Test = 1;
                    
                }
                
                break;
        }//Switch
        
    }//If
    
    ++state.m_ProgramCounter;
    
}

void OpTestRandom::Execute(MachineState &state){
    
    state.m_Test = rand()%2;// Randomly set testFlag to 1 or 0
    
    ++state.m_ProgramCounter;
}

void OpJE::Execute(MachineState &state){
    
    if(state.m_Test == true){//Set the program counter to new line
                    // (jump to the new line, m_Param)
        state.m_ProgramCounter = m_Param;
        
    }
    else{// Otherwise, just go to the next line
        ++state.m_ProgramCounter;
    }
    
}

void OpAttack::Execute(MachineState& state)
{
    
    bool amZombie = state.GetInfect();// Check if the current being is a Zombie/Human
    
    switch(state.m_Facing){// Check which direction it is Facing
        case (MachineState::UP) :
            
            if(state.ylocation != 0){// If Not facing the top wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation][state.ylocation-1] != nullptr ){
                    
                    if( amZombie == false ){//Humans don't differentiates
                        
                        // Kill (Not Delete) the thing that was at [state.xlocation][state.ylocation-1]
                        World::get().board[state.xlocation][state.ylocation-1]->died = true;
                        World::get().board[state.xlocation][state.ylocation-1] = nullptr;
                        
                    }
                    else{//Zombies turn Humans into Zombies and don't kill Zombies
                        
                        // Mark as is dead, Delete at the end of Taking Turn for all
                        if( World::get().board[state.xlocation][state.ylocation-1]->GetInfect() == false ){
                            
                            World::get().board[state.xlocation][state.ylocation-1]->died = true;
                            
                            //Spawn a new zombie, add to map
                            MachineState* newZombie = new MachineState;
                            
                            newZombie->xlocation = state.xlocation;
                            newZombie->ylocation = state.ylocation-1;
                            newZombie->m_Facing = World::get().board[state.xlocation][state.ylocation-1]->m_Facing;
                            
                            World::get().zombieMachine.BindState(*newZombie);
                            World::get().board[state.xlocation][state.ylocation-1] = newZombie;
                            World::get().Zombies.push_back(newZombie);// Add a zombie to the vector
                        }
                    }
                }
            }
            
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation != 19){// If Not facing the right wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation+1][state.ylocation] != nullptr ){
                    
                    if( amZombie == false ){//Humans don't differentiates
                        
                        // Kill(Not Delete) the thing that was at [state.xlocation][state.ylocation-1]
                        World::get().board[state.xlocation+1][state.ylocation]->died = true;
                        World::get().board[state.xlocation+1][state.ylocation] = nullptr;
                        
                    }
                    else{//Zombies turn Humans into Zombies and don't kill Zombies
                        
                        //don't do anything if it is a zombie
                        //If Human, delete human and spawn Zombie
                        if(World::get().board[state.xlocation+1][state.ylocation]->GetInfect() == false){
                            
                            World::get().board[state.xlocation+1][state.ylocation]->died = true;
                            
                            //Spawn a new zombie, add to map
                            MachineState* newZombie = new MachineState;
                            
                            newZombie->xlocation = state.xlocation+1;
                            newZombie->ylocation = state.ylocation;
                            newZombie->m_Facing = World::get().board[state.xlocation+1][state.ylocation]->m_Facing;
                            
                            World::get().zombieMachine.BindState(*newZombie);
                            World::get().board[state.xlocation+1][state.ylocation] = newZombie;
                            World::get().Zombies.push_back(newZombie);// Add a zombie to the vector
                        }
                    }
                }
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation != 19){// If Not facing the bottom wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation][state.ylocation+1] != nullptr ){
                    
                    if( amZombie == false ){//Humans don't differentiates
                        
                        // Kill (Not delete) the thing that was at [state.xlocation][state.ylocation-1]
                        World::get().board[state.xlocation][state.ylocation+1]->died = true;
                        World::get().board[state.xlocation][state.ylocation+1] = nullptr;
                        
                    }
                    else{//Zombies turn Humans into Zombies and don't kill Zombies
                        
                        //don't do anything if it is a zombie
                        //If Human, delete human and spawn Zombie
                        if(World::get().board[state.xlocation][state.ylocation+1]->GetInfect() == false){
                            
                            World::get().board[state.xlocation][state.ylocation+1]->died = true;
                            
                            //Spawn a new zombie, add to map
                            MachineState* newZombie = new MachineState;
                            
                            newZombie->xlocation = state.xlocation;
                            newZombie->ylocation = state.ylocation+1;
                            newZombie->m_Facing = World::get().board[state.xlocation][state.ylocation+1]->m_Facing;
                            
                            World::get().zombieMachine.BindState(*newZombie);
                            World::get().board[state.xlocation][state.ylocation+1] = newZombie;
                            World::get().Zombies.push_back(newZombie);// Add a zombie to the vector
                        }
                        
                    }
                }
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation != 0){// If Not facing the left wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation+1][state.ylocation] != nullptr ){
                    
                    if( amZombie == false ){//Humans don't differentiates
                        
                        // Kill the thing that was at [state.xlocation-1][state.ylocation]
                        World::get().board[state.xlocation-1][state.ylocation]->died = true;
                        World::get().board[state.xlocation-1][state.ylocation] = nullptr;
                        
                    }
                    else{//Zombies turn Humans into Zombies and don't kill Zombies

                        //If Human, delete human and spawn Zombie
                        if(World::get().board[state.xlocation-1][state.ylocation]->GetInfect() == false){
                            
                            World::get().board[state.xlocation-1][state.ylocation]->died = true;
                            
                            //Spawn a new zombie, add to map
                            MachineState* newZombie = new MachineState;
                            
                            newZombie->xlocation = state.xlocation-1;
                            newZombie->ylocation = state.ylocation;
                            newZombie->m_Facing = World::get().board[state.xlocation-1][state.ylocation]->m_Facing;
                            
                            World::get().zombieMachine.BindState(*newZombie);
                            World::get().board[state.xlocation-1][state.ylocation] = newZombie;
                            World::get().Zombies.push_back(newZombie);// Add a zombie to the vector
                        }
                    }
                }
            }
            
            break;
    }
    
    state.m_ProgramCounter++;
    state.m_ActionsTaken++;
    
};

void OpRangedAttack::Execute(MachineState& state)
{
    
    // Check if the current being is a Zombie/Human
    try{
        if(state.GetInfect() == true){
            
            throw ZombieViolation();
            
        }
    }
    
    catch(ZombieViolation){
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
        
    }
    
    switch(state.m_Facing){// Check which direction it is Facing
        case (MachineState::UP) :
            
            if(state.ylocation > 1){// If Not facing the top wall, continue
                
                // Check the space it is facing
                // there is something there
                if( World::get().board[state.xlocation][state.ylocation-2] != nullptr ){
                        
                        // Delete the thing that was at [state.xlocation][state.ylocation-2]
                        // Set Zombie or Human to dead and remove from map
                        World::get().board[state.xlocation][state.ylocation-2]->died = true;
                        World::get().board[state.xlocation][state.ylocation-2] = nullptr;
                    
                    }
                }
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation < 18){// If Not facing the right wall, continue
                
                // Check the space it is facing
                // there is something there
                if( World::get().board[state.xlocation+2][state.ylocation] != nullptr ){

                    // Delete the thing that was at [state.xlocation][state.ylocation-2]
                    World::get().board[state.xlocation+2][state.ylocation]->died = true;
                    World::get().board[state.xlocation+2][state.ylocation] = nullptr;
                    
                }
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation < 18){// If Not facing the bottom wall, continue
                
                // Check the space it is facing
                // there is something there
                if( World::get().board[state.xlocation][state.ylocation+2] != nullptr ){
                        
                    // Delete the thing that was at [state.xlocation][state.ylocation-1]
                    World::get().board[state.xlocation][state.ylocation+2]->died = true;
                    World::get().board[state.xlocation][state.ylocation+2] = nullptr;
                    
                }
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation > 1){// If Not facing the left wall, continue
                
                // Check the space it is facing
                // there is something there
                if( World::get().board[state.xlocation+2][state.ylocation] != nullptr ){
                    
                    // Delete the thing that was at [state.xlocation-1][state.ylocation]
                    World::get().board[state.xlocation-2][state.ylocation]->died = true;
                    World::get().board[state.xlocation-2][state.ylocation] = nullptr;
                    
                }
            }
            break;
    }
    state.m_ProgramCounter++;
    state.m_ActionsTaken++;
};

void OpEndTurn::Execute(MachineState& state)
{
    
    state.m_ActionsTaken = state.GetActionsPerTurn();
    
};

void OpTestHuman::Execute(MachineState& state)
{
    
    state.m_Test = 0;
    int n = m_Param;
    
    // Exception if m_Param is not 1 or 2
    try{
        if(m_Param != 1 && m_Param != 2){
            
            throw InvalidParam();
            
        }
    }
    catch(InvalidParam){
        
        m_Param = 1;
        n = m_Param;
        
    }
    
    switch(state.m_Facing){// Check which direction it is Facing
        case (MachineState::UP) :
            
            if(state.ylocation > n - 1){// If Not facing the top wall, continue
                
                // Check the space it is facing
                if( (World::get().board[state.xlocation][state.ylocation-n] != nullptr) &&
                   (World::get().board[state.xlocation][state.ylocation-n]->GetInfect() == false) ){
                        
                        state.m_Test = 1;
                    
                }
                
            }
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation < 20 - n){// If Not facing the right wall, continue
                
                // Check the space it is facing
                if( (World::get().board[state.xlocation+n][state.ylocation] != nullptr) &&
                   (World::get().board[state.xlocation+n][state.ylocation]->GetInfect() == false) ){
                    
                    state.m_Test = 1;
                    
                }
                
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation < 20 - n){// If Not facing the bottom wall, continue
                
                // Check the space it is facing
                if( (World::get().board[state.xlocation][state.ylocation+n] != nullptr) &&
                   (World::get().board[state.xlocation][state.ylocation+n]->GetInfect() == false) ){
                    
                    state.m_Test = 1;
                    
                }
                
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation > n - 1){// If Not facing the left wall, continue
                
                // Check the space it is facing
                // there is something there
                if( (World::get().board[state.xlocation-n][state.ylocation] != nullptr) &&
                   (World::get().board[state.xlocation-n][state.ylocation]->GetInfect() == false) ){
                    
                    state.m_Test = 1;
                    
                }
            }
            break;
    }
    
    state.m_ProgramCounter++;
};

void OpTestZombie::Execute(MachineState& state)
{
    
    state.m_Test = 0;
    int n = m_Param;
    
    try{
        if(m_Param != 1 && m_Param != 2){
            
            throw InvalidParam();
            
        }
    }
    catch(InvalidParam){
        
        m_Param = 1;
        n = m_Param;
        
    }
    
    switch(state.m_Facing){// Check which direction it is Facing
        case (MachineState::UP) :
            
            if(state.ylocation > n-1){// If Not facing the top wall, continue
                
                // Check the space it is facing and whether or not it is a Zombie
                if( World::get().board[state.xlocation][state.ylocation-n] != nullptr){
                    
                    if(World::get().board[state.xlocation][state.ylocation-n]->GetInfect() == true){
                        
                        state.m_Test = 1;
                        
                    }
                }
            }
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation < 20 - n){// If Not facing the right wall, continue
                
                // Check the space it is facing and whether or not it is a Zombie
                if( World::get().board[state.xlocation + n][state.ylocation] != nullptr){
                    
                    if( World::get().board[state.xlocation + n][state.ylocation]->GetInfect() == true ){
                        
                        state.m_Test = 1;
                        
                    }
                }
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation < 20 - n){// If Not facing the bottom wall, continue
                
                // Check the space it is facing and whether or not it is a Zombie
                if( World::get().board[state.xlocation][state.ylocation + n] != nullptr){
                    
                    if( World::get().board[state.xlocation][state.ylocation + n]->GetInfect() == true ){
                        
                        state.m_Test = 1;
                        
                    }
                }
            }
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation > n - 1){// If Not facing the left wall, continue
                
                // Check the space it is facing and whether or not it is a Zombie
                if( World::get().board[state.xlocation - n][state.ylocation] != nullptr){
                    
                    if(World::get().board[state.xlocation - n][state.ylocation]->GetInfect() == true ){
                        
                        state.m_Test = 1;
                        
                    }
                }
            }
            break;
    }

    state.m_ProgramCounter++;
};

void OpTestPassable::Execute(MachineState& state)
{
    
    state.m_Test = 0;
    
    switch(state.m_Facing){// Check which direction it is Facing
        case (MachineState::UP) :
            
            if(state.ylocation > 0){// If Not facing the top wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation][state.ylocation-1] == nullptr){
                    
                    state.m_Test = 1;
                    
                }
                
            }
            break;
        case (MachineState::RIGHT) :
            
            if(state.xlocation < 19){// If Not facing the right wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation+1][state.ylocation] == nullptr ){
                    
                    state.m_Test = 1;
                    
                }
                
            }
            
            break;
        case (MachineState::DOWN) :
            
            if(state.ylocation < 19){// If Not facing the bottom wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation][state.ylocation+1] == nullptr ){
                    
                    state.m_Test = 1;
                    
                }
                
            }
            
            break;
        default:
        case (MachineState::LEFT) :
            
            if(state.xlocation > 0){// If Not facing the left wall, continue
                
                // Check the space it is facing
                if( World::get().board[state.xlocation-1][state.ylocation] == nullptr ){
                    
                    state.m_Test = 1;
                    
                }
            }
            break;
    }
    
    state.m_ProgramCounter++;
};

void OpJNE::Execute(MachineState& state)
{
    if(state.m_Test == false){//Set the program counter to new line
        // (jump to the new line, m_Param)
        state.m_ProgramCounter = m_Param;
        
    }
    else{// Otherwise, just go to the next line
        ++state.m_ProgramCounter;
    }
};


void OpMem::Execute(MachineState& state)
{
    state.m_ProgramCounter++;
    
    try{
        if( m_Param != 0 || m_Param != 1 || state.GetInfect() == true ){
            
            throw InvalidParam();
            
        }
    }
    catch(InvalidParam){
        
        m_Param = 0;
        
    }
    
    state.currentMemory = m_Param;
    
};

void OpSet::Execute(MachineState& state)
{
    try{
        if( state.currentMemory != 0 || state.currentMemory != 1){
            
            throw InvalidMemory();
            
        }
        if( state.GetInfect() == true ){
            
            throw ZombieViolation();
            
        }
    }
    catch( InvalidMemory ){
        
        state.currentMemory = 0;//Reset
    }
    catch(ZombieViolation){//Remove Zombie
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
    }
    
    state.SetMemory( state.currentMemory, m_Param);
    state.m_ProgramCounter++;
};

void OpInc::Execute(MachineState& state)
{
    try{
        if( state.currentMemory != 0 || state.currentMemory != 1){
            throw InvalidMemory();
        }
        if( state.GetInfect() == true ){
            throw ZombieViolation();
        }
    }
    catch( InvalidMemory ){
        
        state.currentMemory = 0;//Reset
    }
    catch(ZombieViolation){//Remove Zombie
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
    }
    
    state.SetMemory(state.currentMemory, state.GetMemory(state.currentMemory) + 1);
    state.m_ProgramCounter++;
};

void OpDec::Execute(MachineState& state)
{
    try{
        if( state.currentMemory != 0 || state.currentMemory != 1){
            throw InvalidMemory();
        }
        if( state.GetInfect() == true ){
            throw ZombieViolation();
        }
    }
    catch( InvalidMemory ){
        
        state.currentMemory = 0;//Reset
    }
    catch(ZombieViolation){//Remove Zombie
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
    }

    state.SetMemory(state.currentMemory, state.GetMemory(state.currentMemory) - 1);
    
    state.m_ProgramCounter++;
};

void OpTestMem::Execute(MachineState& state)
{
    try{
        if( state.currentMemory != 0 || state.currentMemory != 1){
            throw InvalidMemory();
        }
        if( state.GetInfect() == true ){
            throw ZombieViolation();
        }
    }
    catch( InvalidMemory ){
        
        state.currentMemory = 0;//Reset
    }
    catch(ZombieViolation){//Remove Zombie
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
    }

    if( state.GetMemory( state.currentMemory ) == m_Param ){
        
        state.m_Test = 1;
        
    };
    
    state.m_ProgramCounter++;
};

void OpSaveLoc::Execute(MachineState& state)
{
    try{
        if( state.currentMemory != 0 || state.currentMemory != 1){
            throw InvalidMemory();
        }
        if( state.GetInfect() == true ){
            throw ZombieViolation();
        }
    }
    catch( InvalidMemory ){
        
        state.currentMemory = 0;//Reset
    }
    catch(ZombieViolation){//Remove Zombie
        
        World::get().board[state.xlocation][state.ylocation]->died = true;
        World::get().board[state.xlocation][state.ylocation] = nullptr;
    }
    
    state.SetMemory( 0 , state.xlocation);
    state.SetMemory( 1 , state.ylocation);
    state.m_ProgramCounter++;
};