#pragma once
#include "pch.h"

class Parser
{
private:
	std::vector<Token> m_Tokens;
	int currentI = 0;
	Node* nodeG;

	bool CheckBoolCondition(ConditionNode* cn);
	bool CheckBrace(Node* node);
	bool CheckParCondition(FunctionNode* fn);
	//bool CheckHookCondition(int* _int);
	bool CheckReturn(Node* fn);
	bool CheckIdentifieur(VarNode* vn, Node* fn);
	bool CheckOperation(BinaryNode* bn);


public:
	Node* ParserCheck(std::vector<Token> tokens);
};