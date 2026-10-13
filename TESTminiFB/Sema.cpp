#include "Sema.h"

bool Sema::CheckBinNode(Node* _bin, Scope* _scope)
{
	for (Node* node : _bin->childs)
	{
		if (dynamic_cast<VarNode*>(node))
		{
			VarNode* var = (VarNode*)node;
			if (!_scope->CheckVar(var))
			{
				cout << "Error : this variable doesn't exist" << endl;
				std::exit(1);
				return false;
			}
		}
		else if (dynamic_cast<BinaryNode*>(node))
		{
			if (!CheckBinNode(node, _scope))
			{
				return false;
			}
		}
	}
	return true;
}

bool Sema::CheckConNode(ConditionNode* _con, Scope* _scope)
{
	CompNode* comNode;

	if (dynamic_cast<CompNode*>(_con->childs[0]))
	{
		comNode = (CompNode*)_con->childs[0];
		for (Node* var : comNode->childs)
		{
			if (dynamic_cast<VarNode*>(var))
			{
				VarNode* vn = (VarNode*)var;
				if (!_scope->CheckVar(vn))
				{
					cout << "Error : this variable doesn't exist" << endl;
					std::exit(1);
					return false;
				}
			}
		}
	}
	else
	{
		cout << "Error : you need a proper comparison for the condition in line : " << _con->childs[0]->line << endl;
		std::exit(1);
		return false;
	}

	CheckNodeSco(_con, _scope);

	return true;
}

bool Sema::CheckNodeSco(Node* _node, Scope* _scope)
{
	Scope* scope = _scope;
	scope = new Scope;
	scope->SetParent(_scope);
	scopeTemp.push_back(scope);

	for (int i = 1; i < _node->childs.size(); i++)
	{
		Node* node = _node->childs[i];

		if (dynamic_cast<VarNode*>(node))
		{
			VarNode* varNo = (VarNode*)(node);
			scope->LoadVariable(varNo);
		}
		else if (dynamic_cast<BinaryNode*>(node))
		{
			BinaryNode* binNo = (BinaryNode*)node;
			if (!CheckBinNode(binNo, scope))
			{
				return false;
			}
		}
		else if (dynamic_cast<ConditionNode*>(node))
		{
			ConditionNode* conNo = (ConditionNode*)node;
			if (!CheckConNode(conNo, scope))
			{
				return false;
			}
		}
		else if (dynamic_cast<CallFunctionNode*>(node))
		{
			CallFunctionNode* cFunNo = (CallFunctionNode*)node;
			if (!CheckCalNode(cFunNo, scope))
			{
				return false;
			}
		}
	}

	scopeTemp.pop_back();
	delete scope;
	return true;
}

bool Sema::CheckFunNode(Node* _fun, Scope* _scope)
{
	FunctionNode* fun = (FunctionNode*)_fun;

	if (fun->params.size() > 0)
	{
		for (ParamFunct param : fun->params)
		{
			VarNode* var = new VarNode;
			var->name = param.name;
			_scope->LoadVariable(var);
		}
	}

	for (Node* node : _fun->childs)
	{
		if (dynamic_cast<VarNode*>(node))
		{
			VarNode* varNo = (VarNode*)(node);
			_scope->LoadVariable(varNo);
		}
		else if (dynamic_cast<BinaryNode*>(node))
		{
			BinaryNode* binNo = (BinaryNode*)node;
			if (!CheckBinNode(binNo, _scope))
			{
				return false;
			}
		}
		else if (dynamic_cast<ConditionNode*>(node))
		{
			ConditionNode* conNo = (ConditionNode*)node;
			if (!CheckConNode(conNo, _scope))
			{
				return false;
			}
		}
		else if (dynamic_cast<CallFunctionNode*>(node))
		{
			CallFunctionNode* cFunNo = (CallFunctionNode*)node;
			if (!CheckCalNode(cFunNo, _scope))
			{
				return false;
			}
		}
	}
	return true;
}

bool Sema::CheckCalNode(CallFunctionNode* _call, Scope* _scope)
{
	for (FunctionNode* fun : functions)
	{
		if (_call->name == fun->name)
		{
			return true;
		}
	}
	return false;
}

void Sema::SemaCheck(Node* _nodeAST)
{
	nodeG = _nodeAST;

	for (Node* node : nodeG->childs)
	{
		if (dynamic_cast<FunctionNode*>(node))
		{
			FunctionNode* funcNode = (FunctionNode*)node;
			functions.push_back(funcNode);
		}
		//deja verifier dans le parser
	}

	for (FunctionNode* f : functions)
	{
		if (f->returnType == Type::VOID && f->name == "init" && f->params.size() == 0)
		{
			hasInit = true;
		}
		else if (f->returnType == Type::VOID && f->name == "update" && f->params.size() == 0)
		{
			hasUpdate = true;
		}
	}
	if (hasInit == false || hasUpdate == false)
	{
		cout << "Error : you need a function init or update in this language" << endl;
		std::exit(1);
		return;
	}

	for (FunctionNode* funcNode : functions)
	{
		Scope* scope = scopeG;

		if (funcNode->name != "init")
		{
			scope = new Scope;
			scope->SetParent(scopeG);
			scopeTemp.push_back(scope);
		}

		CheckFunNode(funcNode, scope);
	}
}
