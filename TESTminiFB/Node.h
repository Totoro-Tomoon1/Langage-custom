#pragma once
#include <vector>

class Node
{
protected:
	Node() = default;
	virtual ~Node() = default;
public :
	int line = 0, col = 0;
	std::vector<Node*> childs;
};

class GlobalNode : public Node
{
	~GlobalNode() = default;
};

class VarNode : public Node
{
	~VarNode() = default;
public:
	std::string name;
};

class IntNode : public Node
{
	~IntNode() = default;
public:
	int value;
};

class BinaryNode : public Node
{
	~BinaryNode() = default;
public:
	char op;
};

namespace Type
{
	enum TYPE
	{
		VOID,
		INT
	};
}


struct ParamFunct
{
	Type::TYPE type;
	std::string name;
};

class FunctionNode : public Node
{
	~FunctionNode() = default;
public:
	Type::TYPE returnType;
	std::string name;
	std::vector<ParamFunct> params;
};

class CallFunctionNode : public Node
{
	//param in children
public:
	std::string name;
};

namespace ConditionType
{
	enum CONDITIONTYPE
	{
		WHILE,
		IF,
		ELSEIF,
		ELSE
	};
}

class ConditionNode : public Node
{
	~ConditionNode() = default;
public:
	ConditionType::CONDITIONTYPE type;
};

class CompNode : public Node
{
public:
	char op;
};

class ReturnNode : public Node
{
//data return has child ?
};

class PrintNode : public Node
{

};

class StopNode : public Node
{

};

class DrawColorNode : public Node
{
};

class DrawPosNode : public Node
{
};