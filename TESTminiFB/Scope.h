#pragma once
#include "pch.h"

class Scope
{
private:
	Scope* child;
	std::vector<VarNode*> varG;

public:
	void SetParent(Scope* _parent);
	void LoadVariable(VarNode* _var);
	bool CheckVar(VarNode* _var);
};

