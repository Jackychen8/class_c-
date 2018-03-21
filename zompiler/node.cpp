#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	m_Statements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context)
{
	// Loop through statements in list and code gen them
    for (auto it = m_Statements.begin(); it != m_Statements.end(); it++) {
        (*(*it)).CodeGen(context);
    }
    if( m_bMainBlock == 1 ){
        context.m_Ops.push_back("goto,1");
        context.gotoMap.insert (std::pair<unsigned long ,unsigned long>( context.m_Ops.size(), 1) );
    }
}

NNumeric::NNumeric(std::string& value)
{
	m_value = std::atoi(value.c_str());
}

NRotate::NRotate(NNumeric* dir)
	: m_Dir(dir)
{
}

void NRotate::CodeGen(CodeContext& context)
{
	if (m_Dir->m_value == 0)
	{
		context.m_Ops.push_back("rotate,0");
	}
	else if (m_Dir->m_value == 1)
	{
		context.m_Ops.push_back("rotate,1");
	}
}


////////////////////////////////////////
///////NStatement sub-classes///////////
////////////////////////////////////////
NIfElse::NIfElse(NBoolean* boo, NBlock* block, NBlock* block2)
: m_Boo(boo), m_IfBlock(block), m_ElseBlock(block2)
{
}
void NIfElse::CodeGen(CodeContext& context)
{
    m_Boo->CodeGen(context);
    
    //keep track of where the start of the else and if blocks are (je and goto,)
    unsigned long jeLoc = 0;
    unsigned long gotoLoc = 0;
    
    std::string jeStr = "je,";
    std::string gotoStr = "goto,";
    
    std::ostringstream oss;
    
    jeLoc = context.m_Ops.size();// Record je location in the vector
    context.m_Ops.push_back("je,");// Write the je Line
    
    m_ElseBlock->CodeGen(context);// Write the Else Block
    
    gotoLoc = context.m_Ops.size();// Record goto location in the vector
    context.m_Ops.push_back("goto,");// Write the goto line
    
    oss << context.m_Ops.size() + 1;
    jeStr += oss.str();//Append the if jump location to string
    oss.str(std::string());
    context.m_Ops[jeLoc] = jeStr;//replace the old je string
    
    m_IfBlock->CodeGen(context);// Write the If Block
    
    oss << context.m_Ops.size() + 1;
    gotoStr += oss.str();//Append the goto after if location
    context.m_Ops[gotoLoc] = gotoStr;//replace the old goto string
    
    //gotoLoc is the [] location in the m_Ops vector, gotoLoc is the line number
    context.gotoMap.insert (std::pair<unsigned long ,unsigned long>(gotoLoc + 1, context.m_Ops.size() + 1) );
}



void NForward::CodeGen(CodeContext& context)
{
    context.m_Ops.push_back("forward");
}

void NAttack::CodeGen(CodeContext& context)
{
    context.m_Ops.push_back("attack");
}

void NRAttack::CodeGen(CodeContext& context)
{
    context.m_Ops.push_back("ranged_attack");
}

void NSave::CodeGen(CodeContext &context)
{
    context.m_Ops.push_back("save_loc");
}

NMemory::NMemory(NNumeric* memLo, NStatement* symb)
: location(memLo), symbol(symb)
{
}

void NMemory::CodeGen(CodeContext &context){
    
    std::string memor = "mem,";
    std::ostringstream oss;
    oss << (location->m_value);
    
    memor += oss.str();
    context.m_Ops.push_back(memor);

    symbol->CodeGen(context);
}

void NInc::CodeGen(CodeContext& context){
    context.m_Ops.push_back("inc");
}

void NDec::CodeGen(CodeContext& context){
    context.m_Ops.push_back("dec");
}

NSet::NSet(NNumeric* setTo)
: setToValue(setTo)
{
}
void NSet::CodeGen(CodeContext& context){
    
    std::string set = "set,";
    std::ostringstream oss;
    
    oss << (setToValue->m_value);
    set += oss.str();
    
    context.m_Ops.push_back(set);
}

////////////////////////////////////////
/////////NBoolean sub-classes///////////
////////////////////////////////////////
NZombie::NZombie(NNumeric* NZ)
        : Numb(NZ)
{
}

void NZombie::CodeGen(CodeContext &context){
    if (Numb->m_value == 1)
    {
        context.m_Ops.push_back("test_zombie,1");
    }
    else if (Numb->m_value == 2)
    {
        context.m_Ops.push_back("test_zombie,2");
    }
}

NHuman::NHuman(NNumeric* NH)
    : Numb(NH)
{
}

void NHuman::CodeGen(CodeContext &context){
    if (Numb->m_value == 1)
    {
        context.m_Ops.push_back("test_human,1");
    }
    else if (Numb->m_value == 2)
    {
        context.m_Ops.push_back("test_human,2");
    }
}

void NPass::CodeGen(CodeContext &context){
    context.m_Ops.push_back("test_passable");
}

void NRand::CodeGen(CodeContext &context){
    context.m_Ops.push_back("test_random");
}
