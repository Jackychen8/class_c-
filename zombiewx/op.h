// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const char* OpName, int parameter)
		: m_OpName(OpName)
		, m_Param(parameter)
	{ }
    
    Op(const char* OpName)
        : m_OpName(OpName)
    { }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;

	MachineState* GetTarget(MachineState& state, int n = 1);
	
	virtual ~Op() { }
protected:
	int m_Param;
	const char* m_OpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op("OpRotate", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op("OpGoto", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

///////////////////////////////////////////////////////////////
struct OpForward : Op
{
    OpForward() : Op("OpForward"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestWall : Op
{
    OpTestWall() : Op("OpTestWall"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestRandom : Op
{
    OpTestRandom() : Op("OpTestRandom"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpJE : Op
{
    OpJE(int parameter) : Op("OpJE", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
struct OpAttack : Op
{
    OpAttack() : Op("OpAttack"){//Parameter?
    }
    virtual void Execute(MachineState& state) override;
};

struct OpRangedAttack : Op
{
    OpRangedAttack() : Op("OpRangedAttack"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpEndTurn : Op
{
    OpEndTurn() : Op("OpEndTurn"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestHuman : Op
{
    OpTestHuman(int parameter) : Op("OpTestHuman", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestZombie : Op
{
    OpTestZombie(int parameter) : Op("OpTestZombie", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestPassable : Op
{
    OpTestPassable() : Op("OpTestPassable"){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpJNE : Op
{
    OpJNE(int parameter) : Op("OpJNE", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpMem : Op
{
    OpMem(int parameter) : Op("OpMem", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpSet : Op
{
    OpSet(int parameter) : Op("OpSet", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpInc : Op
{
    OpInc() : Op("OpInc", 1){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpDec : Op
{
    OpDec() : Op("OpDec", 1){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpTestMem : Op
{
    OpTestMem(int parameter) : Op("OpTestMem", parameter){
    }
    virtual void Execute(MachineState& state) override;
};

struct OpSaveLoc : Op
{
    OpSaveLoc() : Op("OpSaveLoc", 1){
    }
    virtual void Execute(MachineState& state) override;
};
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
