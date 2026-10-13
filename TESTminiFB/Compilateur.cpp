#include "Compilateur.h"

void Compilateur::AddVar(std::string name, bool isGlobal = false)
{
	int freeR = GetFreeRegistr();

	if (isGlobal)
		globals[name] = freeR;
	else
		localsVar[name] = freeR;

	uint32_t instr = 0;

	instr |= (uint32_t)Instruction::LOAD;
	instr |= (uint32_t)freeR << 8;
	instr |= (uint32_t)0 << 16;

	AddInstruction(instr);
	std::string line;

	line += "LOAD r";
	line += std::to_string(freeR);
	line += " 0";

	AddTxt(line);
}

int Compilateur::GetVar(std::string name)
{
	if (globals.find(name) != globals.end())
		return globals[name];
	else if (localsVar.find(name) != localsVar.end())
		return localsVar[name];

	return -1;
}

void Compilateur::FreeRegister(int nb)
{
	for (int i = 0; i < registerTaken.size(); i++)
	{
		if (registerTaken[i] == nb)
		{
			registerTaken.erase(registerTaken.begin() + i);
			return;
		}
	}

	//not in registersTaken

	for (auto name : localsVar)
	{
		if (name.second == nb)
		{
			localsVar.erase(name.first);
			return;
		}
	}
}

int Compilateur::GetIndexFunction(std::string name)
{
	for (auto key : idFunctions)
	{
		if (key.first == name)
			return key.second;
	}

	return -1;//not found
}

void Compilateur::LinkParam(std::string name, int id)
{
	localsVar[name] = id;//id 0 or 1
}

void Compilateur::AddTxt(std::string line)
{
	if (!isPostTxt)
		txt += line + "\n";
	else
		postFunctionTxt += line + "\n";
}

void Compilateur::AddInstruction(uint32_t instr)
{
	if (!isPostTxt)
		result[GetIndexFunction(currentFunction->name)].push_back(instr);
	else
		lastInstruction.push_back(instr);
}

int Compilateur::GetFreeRegistr()
{
	bool found = false;

	while (!found)
	{
		nextReg++;
		//lock max possible to return to 0 and if not found yet no space left

		if (nextReg >= maxRegister)
			nextReg = 3;

		bool temp = true;

		for (auto key : localsVar)
		{
			if (key.second == nextReg)
				temp = false;
		}

		for (int id : registerTaken)
		{
			if (id == nextReg)
				temp = false;
		}

		for (auto key : globals)
		{
			if (key.second == nextReg)
				temp = false;
		}

		found = temp;
	}

	return nextReg;
}

void Compilateur::PrintText()
{
	cout << txt << endl;
}

void Compilateur::CompileFunction(FunctionNode* node)
{
	localsVar.clear();
	registerTaken.clear();

	//nextReg = 3 + globals.size();

	if (node->name == "init")
	{
		isInInit = true;
	}
	else
		isInInit = false;

	AddTxt(node->name + " :");

	int i = 0;
	for (ParamFunct p : node->params)//we assume for the moment that there is max 2 params
	{
		LinkParam(p.name, i);
		i++;
	}

	for (Node* child : node->childs)
	{
		CompileStatement(child);
	}

	AddTxt("ret"); //instruction de return

	uint32_t instr = 0;

	instr |= (uint32_t)Instruction::RET;

	result[GetIndexFunction(currentFunction->name)].push_back(instr);
	//retLoc = 0;

	AddTxt("");
	if (postFunctionTxt.size() != 0)
		AddTxt(postFunctionTxt);
	postFunctionTxt.clear();

	if (lastInstruction.size() != 0)
	{
		for (uint32_t lastIn : lastInstruction)
		{
			AddInstruction(lastIn);
		}
	}
	lastInstruction.clear();
}

void Compilateur::CompileStatement(Node* node)
{
	if (dynamic_cast<VarNode*>(node))
	{
		//get un registre pour cette variable (int32)
		VarNode* varNode = (VarNode*)node;
		AddVar(varNode->name, isInInit);//instrucion de load 0 dans un registr ?
	}
	else if (dynamic_cast<BinaryNode*>(node))
	{
		//debute par un egale donc return le resultat dans la variable associer (attention binarynode peut avoir ces operands de type binarynode aussi)
		CompileBinaryNode((BinaryNode*)node);
	}
	else if (dynamic_cast<ConditionNode*>(node))
	{
		CompileConditionNode((ConditionNode*)node);
	}
	else if (dynamic_cast<CallFunctionNode*>(node))
	{
		CompileCallFunctionNode((CallFunctionNode*)node);
	}
	else if (dynamic_cast<ReturnNode*>(node))
	{
		if (node->childs.size() == 1)
		{
			CompileReturnNode((ReturnNode*)node);
		}
	}
	else if (dynamic_cast<PrintNode*>(node))
	{
		uint32_t instr = 0;

		instr |= (uint32_t)Instruction::PRINT;

		VarNode* varNode = (VarNode*)node->childs[0];

		instr |= (uint32_t)GetVar(varNode->name) << 8;

		AddInstruction(instr);
	}
	else if (dynamic_cast<StopNode*>(node))
	{
		uint32_t instr = 0;

		instr |= (uint32_t)Instruction::STOP;

		AddInstruction(instr);
	}
	else if (dynamic_cast<DrawColorNode*>(node))
	{
		CompileDrawColorNode((DrawColorNode*)node);
	}
	else if (dynamic_cast<DrawPosNode*>(node))
	{
		CompileDrawPosNode((DrawPosNode*)node);
	}
}

int Compilateur::CompileBinaryNode(BinaryNode* bn)
{
	int value1;
	int value2;
	int resultLoc = -1;
	bool isFirst = true;
	std::string line;

	uint32_t instr = 0;

	switch (bn->op)
	{
	case '+':
	{		
		instr |= (uint32_t)Instruction::ADD;

		line += "ADD ";
		resultLoc = GetFreeRegistr();

		instr |= (uint32_t)resultLoc << 8;

		line += " r";
		line += std::to_string(resultLoc);
		line += " ";

		break;
	}
	case '-':
	{
		instr |= (uint32_t)Instruction::SUB;

		line += "SUB ";
		resultLoc = GetFreeRegistr();

		instr |= (uint32_t)resultLoc << 8;

		line += " r";
		line += std::to_string(resultLoc);
		line += " ";
		break;
	}
	case '/':
	{
		instr |= (uint32_t)Instruction::DIV;

		line += "DIV ";
		resultLoc = GetFreeRegistr();

		instr |= (uint32_t)resultLoc << 8;

		line += " r";
		line += std::to_string(resultLoc);
		line += " ";
		break;
	}
	case '*':
	{
		instr |= (uint32_t)Instruction::MUL;

		line += "MUL ";
		resultLoc = GetFreeRegistr();

		instr |= (uint32_t)resultLoc << 8;

		line += " r";
		line += std::to_string(resultLoc);
		line += " ";
		break;
	}
	case '=':
	{
		instr |= (uint32_t)Instruction::MOVE;

		line += "MOVE ";

		instr |= (uint32_t)0 << 8;

		break;
	}
	}

	int test = 16;

	for (Node* child : bn->childs)
	{
		if (dynamic_cast<BinaryNode*>(child))
		{
			int loc = CompileBinaryNode((BinaryNode*)child);

			instr |= (uint32_t)loc << test;

			line += " r";
			line += std::to_string(loc);
			line += " ";
		}
		else if (dynamic_cast<VarNode*>(child))
		{
			if (isFirst)
			{
				VarNode* varNode = (VarNode*)child;
				value1 = GetVar(varNode->name); //get the register of value

				instr |= (uint32_t)value1 << test;

				line += " r";
				line += std::to_string(value1);
				line += " ";
			}
		}
		else if (dynamic_cast<IntNode*>(child))
		{
			IntNode* intNode = (IntNode*)child;

			//store int into a register
			int intLoc = StoreInt(intNode->value);

			instr |= (uint32_t)intLoc << test;

			line += " r";
			line += std::to_string(intLoc);
			line += " ";
		}
		else if (dynamic_cast<CallFunctionNode*>(child))
		{
			//move param into r0 r1
			CompileCallFunctionNode((CallFunctionNode*)child);

			instr |= (uint32_t)0 << test; //0 is the return loc

			line += " r";
			line += std::to_string(0);
			line += " ";
		}

		if (test == 16)
			test = 24;
	}

	AddTxt(line);
	//result[currentFunction->name].push_back(instr);
	AddInstruction(instr);

	return resultLoc;
}

void Compilateur::CompileConditionNode(ConditionNode* cn)
{
	std::string line;
	std::string whileTxt;
	uint32_t whileInstr;

	int instr = 0;

	if (cn->type != ConditionType::ELSE)
	{
		CompNode* compNode = (CompNode*)cn->childs[0];

		switch (compNode->op)
		{
		case '<':
		{
			line += "IS ";

			instr |= (uint32_t)Instruction::IS;

			break;
		}
		case '>':
		{
			line += "SI ";

			instr |= (uint32_t)Instruction::SI;

			break;
		}
		case '=':
		{
			line += "EQU ";

			instr |= (uint32_t)Instruction::EQU;

			break;
		}
		}

		//get return registr
		int resultLoc = GetFreeRegistr();

		instr |= (uint32_t)resultLoc << 8;

		line += " r";
		line += std::to_string(resultLoc);
		line += " ";

		//add both value

		if (dynamic_cast<VarNode*>(compNode->childs[0]))
		{
			VarNode* vn = (VarNode*)compNode->childs[0];

			instr |= (uint32_t)GetVar(vn->name) << 16;

			line += " r";
			line += std::to_string(GetVar(vn->name));
			line += " ";
		}
		else if (dynamic_cast<IntNode*>(compNode->childs[0]))
		{
			IntNode* in = (IntNode*)compNode->childs[0];

			int intLoc = StoreInt(in->value);

			instr |= (uint32_t)intLoc << 16;

			line += " ";
			line += std::to_string(intLoc);
			line += " ";
		}

		if (dynamic_cast<VarNode*>(compNode->childs[1]))
		{
			VarNode* vn = (VarNode*)compNode->childs[1];

			instr |= (uint32_t)GetVar(vn->name) << 24;

			line += " r";
			line += std::to_string(GetVar(vn->name));
			line += " ";
		}
		else if (dynamic_cast<IntNode*>(compNode->childs[1]))
		{
			IntNode* in = (IntNode*)compNode->childs[1];

			int intLoc = StoreInt(in->value);

			instr |= (uint32_t)intLoc << 24;

			line += " ";
			line += std::to_string(in->value);
			line += " ";
		}

		AddTxt(line);
		//result[currentFunction->name].push_back(instr);
		AddInstruction(instr);
		whileTxt += line;
		whileTxt += "\n";
		whileInstr = instr;
		//labelsWhile[nbS].push_back(instr);

		//jmp to create scope
		instr = 0;

		instr |= (uint32_t)Instruction::JB;
		instr |= (uint32_t)nbS << 8;
		instr |= (uint32_t)resultLoc << 16;

		labels[nbS].push_back(result[GetIndexFunction(currentFunction->name)].size());

		result[GetIndexFunction(currentFunction->name)].push_back(instr);

		line.clear();
		line += "JB ";
		line += "S";
		line += std::to_string(nbS);
		line += " ";
		line += " r";
		line += std::to_string(resultLoc);
		line += " ";

		AddTxt(line);
		whileTxt += line;
		whileTxt += "\n";

		labelsWhile[nbS] = instr;
	}

	//add scope after the function 
	//get a name for function
	isPostTxt = true;

	//retLoc = result[currentFunction->name].size();

	//correct all the labels link to this one

	for (int id : labels[nbS])
	{
		result[GetIndexFunction(currentFunction->name)][id] &= ~(0xFFu << 8); //clean berfore rewrite
		result[GetIndexFunction(currentFunction->name)][id] |= (uint32_t)result[GetIndexFunction(currentFunction->name)].size() << 8;
	}
	
	labelsWhile[nbS] &= ~(0xFFu << 8);
	labelsWhile[nbS] |= (uint32_t)result[GetIndexFunction(currentFunction->name)].size() << 8;

	line.clear();
	line += "S";
	line += std::to_string(nbS);
	line += " :";

	AddTxt(line);

	//read scope of condition
	for (int i = 1; i < cn->childs.size(); i++)
	{
		CompileStatement(cn->childs[i]);
	}

	//if while add to the end comp and jb

	if (cn->type == ConditionType::WHILE)
	{
		AddTxt(whileTxt);
		//add the indtruction to

		AddInstruction(whileInstr);

		uint32_t val = labelsWhile[nbS];

		AddInstruction(val);
	}

	instr = 0;

	AddTxt("ret");
	instr |= (uint32_t)Instruction::RET;

	AddInstruction(instr);

	isPostTxt = false;

	nbS++;
}

void Compilateur::CompileCallFunctionNode(CallFunctionNode* callNode)
{
	std::string line;
	uint32_t instr = 0;
	uint32_t instrL1 = 0;
	uint32_t instrL2 = 0;

	//store the parametrs into r0 and r1
	/*int i = 0;
	for (Node* param : callNode->childs)
	{
		if (i == 0)
		{
			instr |= (uint32_t)Instruction::LOAD;
			instr |= (uint32_t)0 << 8;
			instr |= (uint32_t) << 16;
		}
	}*/

	line += "JMP ";
	line += callNode->name;

	instr |= (uint32_t)Instruction::JMP;
	instrL1 |= (uint32_t)Instruction::MOVE;
	instrL2 |= (uint32_t)Instruction::MOVE;

	//add the index of the function

	instr |= (uint32_t)GetIndexFunction(callNode->name) << 8;
	instrL1 |= (uint32_t)0 << 8;
	instrL2 |= (uint32_t)0 << 8;

	int loc = 16;

	for (Node* child : callNode->childs)
	{
		if (dynamic_cast<VarNode*>(child))
		{
			VarNode* varNode = (VarNode*)child;

			instr |= (uint32_t)GetVar(varNode->name) << loc;

			line += " r";
			line += std::to_string(GetVar(varNode->name));

			if (loc == 16)//means its the first param
			{
				instrL1 |= (uint32_t)0 << 16;
				instrL1 |= (uint32_t)GetVar(varNode->name) << 24;
			}
			else
			{
				instrL2 |= (uint32_t)1 << 16;
				instrL2 |= (uint32_t)GetVar(varNode->name) << 24;
			}
		}
		else if (dynamic_cast<IntNode*>(child))
		{
			IntNode* intNode = (IntNode*)child;

			int intLoc = StoreInt(intNode->value);

			instr |= (uint32_t)intLoc << loc;

			line += " ";
			line += std::to_string(intNode->value);
		}

		loc += 8;
	}

	AddTxt(line);
	//result[currentFunction->name].push_back(instr);
	AddInstruction(instrL1);
	AddInstruction(instrL2);
	AddInstruction(instr);
}

void Compilateur::CompileReturnNode(ReturnNode* returnNode)
{
	if (returnNode->childs.size() == 1)
	{
		//move data into r0
		uint32_t instr = 0;

		instr |= (uint32_t)Instruction::MOVE;
		instr |= (uint32_t)0 << 8;
		
		if (dynamic_cast<VarNode*>(returnNode->childs[0]))
		{
			VarNode* varNode = (VarNode*)returnNode->childs[0];

			instr |= (uint32_t)GetVar(varNode->name) << 16;
		}
		else if (dynamic_cast<IntNode*>(returnNode->childs[0]))
		{
			IntNode* intNode = (IntNode*)returnNode->childs[0];

			int intLoc = StoreInt(intNode->value);

			instr |= (uint32_t)intLoc << 16;
		}

		AddInstruction(instr);
	}
}

void Compilateur::CompileDrawColorNode(DrawColorNode* drawNode)
{
	uint32_t instr = 0;

	instr |= (uint32_t)Instruction::DRAWCOLOR;

	int loc = 8;

	for (int i = 0; i < 3; i++)
	{
		if (dynamic_cast<VarNode*>(drawNode->childs[i]))
		{
			VarNode* varNode = (VarNode*)drawNode->childs[i];

			instr |= (uint32_t)GetVar(varNode->name) << loc;
		}
		else
		{
			IntNode* intNode = (IntNode*)drawNode->childs[i];

			int intLoc = StoreInt(intNode->value);

			instr |= (uint32_t)intLoc << loc;
		}

		loc += 8;
	}

	AddInstruction(instr);
}

void Compilateur::CompileDrawPosNode(DrawPosNode* drawNode)
{
	uint32_t instr = 0;

	instr |= (uint32_t)Instruction::DRAWPOS;

	int loc = 8;

	for (int i = 0; i < 2; i++)
	{
		if (dynamic_cast<VarNode*>(drawNode->childs[i]))
		{
			VarNode* varNode = (VarNode*)drawNode->childs[i];

			instr |= (uint32_t)GetVar(varNode->name) << loc;
		}
		else
		{
			IntNode* intNode = (IntNode*)drawNode->childs[i];

			int intLoc = StoreInt(intNode->value);

			instr |= (uint32_t)intLoc << loc;
		}

		loc += 8;
	}

	AddInstruction(instr);
}

int Compilateur::StoreInt(int value)
{
	std::string tempLine;

	tempLine += "LOAD r";
	int intLoc = GetFreeRegistr();
	registerTaken.push_back(intLoc);

	tempLine += std::to_string(intLoc);
	tempLine += " ";
	tempLine += std::to_string(value);

	AddTxt(tempLine);

	//encode
	uint32_t load = 0;

	load |= (uint32_t)Instruction::LOAD;
	load |= (uint32_t)intLoc << 8;
	load |= (uint32_t)(uint16_t)value << 16;

	//result[currentFunction->name].push_back(load);
	AddInstruction(load);

	return intLoc;
}

std::unordered_map<int, std::vector<uint32_t>> Compilateur::CompilateurCheck(Node* nodeG)
{
	//start to write function init
	//for (Node* child : nodeG->childs)
	//{
	//	FunctionNode* fn = (FunctionNode*)child;
	//	if (fn->name == "init")
	//	{
	//		//start to write assembleur
	//		//init function
	//		CompileFunction(fn);
	//	}
	//}

	for (Node* child : nodeG->childs)
	{
		FunctionNode* fn = (FunctionNode*)child;
		//new functions
		result[currentIdFunction];
		idFunctions[fn->name] = currentIdFunction;
		currentIdFunction++;
	}

	for (Node* child : nodeG->childs)
	{
		FunctionNode* fn = (FunctionNode*)child;
		currentFunction = fn;
		//if (fn->name != "init")
		CompileFunction(fn);
	}

	PrintText();

	return result;
}
