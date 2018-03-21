//
//  world.h
//  zombiewx-mac
//
//  Created by Jacky Chen on 4/11/15.
//  Copyright (c) 2015 Sanjay Madhav. All rights reserved.
//

#pragma once

#include <stdio.h>
#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;
#include "singleton.h"
#include "machine.h"
#include <vector>
#include "traits.h"

class World : public Singleton<World>{
  
public:
    
    //World();
    
    int monthCount;
    MachineState* board[20][20] = { nullptr };//board[xposition][yposition]
    std::vector<MachineState*> Zombies;
    std::vector<MachineState*> Humans;
    
    // 2 Machines
    Machine<HumanTraits> humanMachine;
    Machine<ZombieTraits> zombieMachine;
    
    std::shared_ptr<MachineState*> beings;// shared pointers
    
    // Loaded Files
    std::string zombieInputPath;
    std::string humanInputPath;
    
private:
    
    
    
};

