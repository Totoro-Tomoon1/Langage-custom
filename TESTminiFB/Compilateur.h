#pragma once
#include "pch.h"
#include <unordered_map>

namespace Instruction
{
	enum INSTRUCTION
	{
		ADD,  //0
		SUB,  //1
		DIV,  //2
		MUL,  //3
		MOVE, //4
		SI,   //5
		JB,   //6
		IS,   //7
		EQU,  //8
		JMP,  //9 
		LOAD, //10
		RET,  //11
		FREE, //12
		PRINT,//13
		STOP, //14
		DRAWCOLOR,
		DRAWPOS
	};
}

class Compilateur
{
	std::string txt;
	std::map < std::string, int> globals;
	std::map < std::string, int> localsVar; //maybe something else to store release register ?
	std::vector<int> registerTaken;
	int nextReg = 2; //need to start at two because first call make it to 3
	//the 3 first one are for parameters of function
	std::string postFunctionTxt;
	int nbS = 0;
	bool isPostTxt = false;
	bool isInInit = false;

	int maxRegister = 256;

	std::unordered_map<int, std::vector<uint32_t>> result;
	FunctionNode* currentFunction = nullptr;
	std::unordered_map<int, std::vector<int>> labels;
	std::unordered_map<int, uint32_t> labelsWhile;
	//int retLoc = 0;
	std::vector<uint32_t> lastInstruction;

	std::unordered_map<std::string, int> idFunctions;
	int currentIdFunction = 0;

	void AddVar(std::string name, bool isGlobal);
	int GetVar(std::string name);
	void FreeRegister(int nb);
	int GetIndexFunction(std::string name);
	void LinkParam(std::string name, int id);

	void AddTxt(std::string line);
	void AddInstruction(uint32_t instr);
	int GetFreeRegistr();
	void PrintText();
	void CompileFunction(FunctionNode* fn);
	void CompileStatement(Node* node);

	int CompileBinaryNode(BinaryNode* bn);
	void CompileConditionNode(ConditionNode* cn);
	void CompileCallFunctionNode(CallFunctionNode* callNode);
	void CompileReturnNode(ReturnNode* returnNode);
	void CompileDrawColorNode(DrawColorNode* drawNode);
	void CompileDrawPosNode(DrawPosNode* posNode);

	int StoreInt(int value);

public:
	std::unordered_map<int, std::vector<uint32_t>> CompilateurCheck(Node* nodeG);
};

