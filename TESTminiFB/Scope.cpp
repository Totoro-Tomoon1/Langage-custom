#include "Scope.h"

void Scope::SetParent(Scope* _parent)
{
	for (VarNode* var : _parent->varG)
	{
		varG.push_back(var);
	}
}

void Scope::LoadVariable(VarNode* _var)
{
	for (VarNode* v : varG)
	{
		if (v->name == _var->name)
		{
			cout << "Error : this variable " << _var->name << " already exists in line : " << v->line << endl;
			std::exit(1);
		}
	}
	varG.push_back(_var);
}

bool Scope::CheckVar(VarNode* _var)
{
	for (VarNode* v : varG)
	{
		if (v->name == _var->name)
		{
			return true;
		}
	}
	return false;
}
