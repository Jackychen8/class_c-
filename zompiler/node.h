#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> m_Ops;
    std::map<int, int> gotoMap;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int m_value;
	virtual void CodeGen(CodeContext& context) override { }
};

class NStatement : public Node
{
public:
	virtual void CodeGen(CodeContext& context) = 0;
};

class NBlock : public Node
{
public:
    NBlock() { m_bMainBlock = false; }
    virtual void CodeGen(CodeContext& context) override;
    void AddStatement(NStatement* statement);
    void SetMainBlock() { m_bMainBlock = true; }
private:
    std::list<NStatement*> m_Statements;
    bool m_bMainBlock;
};

class NBoolean : public Node
{
public:
    virtual void CodeGen(CodeContext& context) = 0;
private:
    NNumeric* m_Input;
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) override;
private:
	NNumeric* m_Dir;
};

////////////////////////////////////////
///////NStatement sub-classes///////////
////////////////////////////////////////
class NForward : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NAttack : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NRAttack : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NSave : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NMemory : public NStatement
{
public:
    NMemory(NNumeric* memLo, NStatement* symb);
    virtual void CodeGen(CodeContext& context) override;
private:
    NNumeric* location;
    NStatement* symbol;
};

class NInc : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NDec : public NStatement
{
public:
    virtual void CodeGen(CodeContext& context) override;
};

class NSet : public NStatement
{
public:
    NSet(NNumeric* setTo);
    virtual void CodeGen(CodeContext& context) override;
private:
    NNumeric* setToValue;
};

class NIfElse : public NStatement
{
public:
    NIfElse(NBoolean* boo, NBlock* block, NBlock* block2);
    virtual void CodeGen(CodeContext& context) override;
private:
    NBoolean* m_Boo;
    NBlock* m_IfBlock;
    NBlock* m_ElseBlock;
};

////////////////////////////////////////
/////////NBoolean sub-classes///////////
////////////////////////////////////////
class NZombie : public NBoolean{
public:
    NZombie(NNumeric* NZSpace);
    virtual void CodeGen(CodeContext& context) override;
    void getBool();
private:
    NNumeric* Numb;
};
class NHuman : public NBoolean{
public:
    NHuman(NNumeric* NH);
    virtual void CodeGen(CodeContext& context) override;
private:
    NNumeric* Numb;
};
class NPass : public NBoolean{
public:
    virtual void CodeGen(CodeContext& context) override;
};
class NRand : public NBoolean{
public:
    virtual void CodeGen(CodeContext& context) override;
};

