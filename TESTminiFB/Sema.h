#pragma once
#include "pch.h"
#include "Scope.h"

class Sema
{
private:
	Node* nodeG;
	std::vector<FunctionNode*> functions;

	Scope* scopeG = new Scope;
	std::vector<Scope*> scopeTemp;

	bool hasInit = false;
	bool hasUpdate = false;

	bool CheckBinNode(Node* _bin, Scope* _scope);
	bool CheckConNode(ConditionNode* _con, Scope* _scope);
	bool CheckNodeSco(Node * _node, Scope* _scope);
	bool CheckFunNode(Node * _fun, Scope* _scope);
	bool CheckCalNode(CallFunctionNode* _call, Scope* _scope);

public:
	void SemaCheck(Node* _nodeAST);
};

