#include "Parser.h"

bool Parser::CheckBoolCondition(ConditionNode* cn)
{
	CompNode* cmn = new CompNode;
	cn->childs.push_back(cmn);

	//token juste apres token de debut
	if (m_Tokens[currentI].type == LEFT_PAR)
	{
		currentI++;
		bool insidePar = true;
		bool hasValue = false;
		while (insidePar)
		{
			if (currentI >= m_Tokens.size())
				insidePar = false;
			else if (m_Tokens[currentI].type == RIGHT_PAR && hasValue)
			{
				currentI++;
				insidePar = false;
				return true;
			}
			else if (m_Tokens[currentI].type == IDENTIFIER)
			{
				hasValue = true;
				VarNode* vn = new VarNode;
				vn->line = m_Tokens[currentI].ligne;
				vn->col = m_Tokens[currentI].column;
				vn->name = m_Tokens[currentI].info;
				cmn->childs.push_back(vn);
			}
			else if (m_Tokens[currentI].type == INT)
			{
				hasValue = true;
				IntNode* in = new IntNode;
				in->line = m_Tokens[currentI].ligne;
				in->col = m_Tokens[currentI].column;
				in->value = std::stoi(m_Tokens[currentI].info);
				cmn->childs.push_back(in);
			}
			else if (IS <= m_Tokens[currentI].type <= EQUAL && hasValue)
			{
				hasValue = false;

				switch (m_Tokens[currentI].type)
				{
				case IS:
				{
					cmn->op = '<';
					break;
				}
				case SI:
				{
					cmn->op = '>';
					break;
				}
				case EQUAL:
				{
					cmn->op = '=';
					break;
				}
				}
			}
			else if (m_Tokens[currentI].type == LEFT_PAR)
			{
				hasValue = CheckBoolCondition(cn); ////////////
			}
			else
			{
				std::cout << "Parser : unexpected " << m_Tokens[currentI].info << " on ligne " << m_Tokens[currentI].ligne << std::endl;
				std::exit(1);
			}
			currentI++;
		}
	}

	return false;
}

bool Parser::CheckBrace(Node* fn)
{
	if (m_Tokens[currentI].type == LEFT_BRACE)
	{
		currentI++;
		bool insideFun = true;
		bool hasValue = false;
		while (insideFun)
		{
			if (currentI >= m_Tokens.size())
				insideFun = false;
			else if (m_Tokens[currentI].type == RIGHT_BRACE)
			{
				currentI++;
				insideFun = false;
				return true;
			}
			else if (m_Tokens[currentI].type == IDENTIFIER)
			{
				VarNode* vn = new VarNode;
				vn->name = m_Tokens[currentI].info;
				vn->line = m_Tokens[currentI].ligne;
				vn->col = m_Tokens[currentI].column;
				currentI++;
				hasValue = true;
				CheckIdentifieur(vn, fn);
			}
			else if (m_Tokens[currentI].type == TYPE)//init var ??
			{
				currentI++;
				if (m_Tokens[currentI].type == IDENTIFIER)
				{
					VarNode* vn = new VarNode;
					vn->name = m_Tokens[currentI].info;
					vn->line = m_Tokens[currentI].ligne;
					vn->col = m_Tokens[currentI].column;
					currentI++;
					fn->childs.push_back(vn);
					CheckIdentifieur(vn, fn);
				}
				else
				{
					cout << "Parser : missing variable after type : " << m_Tokens[currentI - 1].info << " on ligne : " << m_Tokens[currentI - 1].ligne << endl;
					exit(1);
				}
			}
			else if (m_Tokens[currentI].type == TokenType::WHILE || m_Tokens[currentI].type == TokenType::IF || m_Tokens[currentI].type == TokenType::EIF)
			{
				ConditionNode* cn = new ConditionNode;
				cn->line = m_Tokens[currentI].ligne;
				cn->col = m_Tokens[currentI].column;
				fn->childs.push_back(cn);
			
				switch (m_Tokens[currentI].type)
				{
				case TokenType::WHILE:
				{
					cn->type = ConditionType::WHILE;
					break;
				}
				case TokenType::IF:
				{
					cn->type = ConditionType::IF;
					break;
				}
				case TokenType::EIF:
				{
					cn->type = ConditionType::ELSEIF;
					break;
				}
				case TokenType::ELSE:
				{
					cn->type = ConditionType::ELSE;
					break;
				}
				}

				currentI++;
				if (CheckBoolCondition(cn))
				{
					CheckBrace(cn);
				}
			}
			else if (m_Tokens[currentI].type == ELSE)
			{
				currentI++;
				CheckBrace(fn);
			}
			else if (m_Tokens[currentI].type == LEFT_BRACE)
			{
				currentI++;
				hasValue = CheckBrace(fn);
			}
			else if (m_Tokens[currentI].type == RETURN)
			{
				currentI++;
				CheckReturn(fn);
			}
			else if (m_Tokens[currentI].type == PRINT)
			{
				currentI++;

				PrintNode* printNode = new PrintNode;
				fn->childs.push_back(printNode);

				//var only
				VarNode* varNode = new VarNode;
				varNode->name = m_Tokens[currentI].info;

				printNode->childs.push_back(varNode);

				currentI++;
			}
			else if (m_Tokens[currentI].type == STOP)
			{
				currentI++;
				StopNode* stopNode = new StopNode;
				fn->childs.push_back(stopNode);
			}
			else if (m_Tokens[currentI].type == DRAWPOS)
			{
				currentI++;

				DrawPosNode* drawNode = new DrawPosNode;
				fn->childs.push_back(drawNode);

				if (m_Tokens[currentI].type != LEFT_PAR)
				{
					cout << "Paser : Error after call Draw missing c1" << endl;
					std::exit(1);
				}

				bool insidePar = true;
				currentI++;
				while (insidePar)
				{
					if (m_Tokens[currentI].type == RIGHT_PAR)
					{
						insidePar = false;
						currentI++;
						continue;
					}
					else if (m_Tokens[currentI].type == IDENTIFIER)
					{
						VarNode* vn = new VarNode;
						vn->name = m_Tokens[currentI].info;
						vn->line = m_Tokens[currentI].ligne;
						vn->col = m_Tokens[currentI].column;
						drawNode->childs.push_back(vn);

						currentI++;
					}
					else if (m_Tokens[currentI].type == INT)
					{
						IntNode* in = new IntNode;
						in->value = std::stoi(m_Tokens[currentI].info);
						in->line = m_Tokens[currentI].ligne;
						in->col = m_Tokens[currentI].column;
						drawNode->childs.push_back(in);

						currentI++;
					}
					else
					{
						cout << "Parser : Error in parameters of try calling function" << endl;
						std::exit(1);
					}
				}
			}
			else if (m_Tokens[currentI].type == DRAWCOLOR)
			{
				currentI++;

				DrawColorNode* drawNode = new DrawColorNode;
				fn->childs.push_back(drawNode);

				if (m_Tokens[currentI].type != LEFT_PAR)
				{
					cout << "Paser : Error after call Draw missing c1" << endl;
					std::exit(1);
				}

				bool insidePar = true;
				currentI++;
				while (insidePar)
				{
					if (m_Tokens[currentI].type == RIGHT_PAR)
					{
						insidePar = false;
						currentI++;
						continue;
					}
					else if (m_Tokens[currentI].type == IDENTIFIER)
					{
						VarNode* vn = new VarNode;
						vn->name = m_Tokens[currentI].info;
						vn->line = m_Tokens[currentI].ligne;
						vn->col = m_Tokens[currentI].column;
						drawNode->childs.push_back(vn);

						currentI++;
					}
					else if (m_Tokens[currentI].type == INT)
					{
						IntNode* in = new IntNode;
						in->value = std::stoi(m_Tokens[currentI].info);
						in->line = m_Tokens[currentI].ligne;
						in->col = m_Tokens[currentI].column;
						drawNode->childs.push_back(in);

						currentI++;
					}
					else
					{
						cout << "Parser : Error in parameters of try calling function" << endl;
						std::exit(1);
					}
				}
			}
			else
			{
				std::cout << "Parser : incorrect information : " << m_Tokens[currentI].info << " on ligne " << m_Tokens[currentI].ligne << std::endl;
				std::exit(1);
				return false;
			}
		}
	}
	else
	{
		std::cout << "Parser : Error of parsing missing c5 on ligne  : " << m_Tokens[currentI].ligne << std::endl;
		std::exit(1);
		return false;
	}

	return false;
}

bool Parser::CheckParCondition(FunctionNode* fn)
{
	if (m_Tokens[currentI].type == LEFT_PAR)
	{
		currentI++;
		//check inside untile ) or unexpected token
		bool insidePar = true;
		bool hasType = false;
		ParamFunct param;
		while (insidePar)
		{
			if (m_Tokens[currentI].type == RIGHT_PAR)
			{
				if (hasType)
				{
					std::cout << "Parser : Missing variable name with type " << m_Tokens[currentI - 1].info << " on ligne " << m_Tokens[currentI - 1].ligne << std::endl;
					std::exit(1);
				}
				currentI++;
				insidePar = false;
				return true;
			}
			else if (m_Tokens[currentI].type == TYPE)
			{
				hasType = true;

				if (m_Tokens[currentI].info == "VOID")
					param.type = Type::VOID;
				else
					param.type = Type::INT;
			}
			else if (m_Tokens[currentI].type == IDENTIFIER)
			{
				if (hasType)
				{
					hasType = false;
					param.name = m_Tokens[currentI].info;
					fn->params.push_back(param);
				}
				else
				{
					std::cout << "Parser : Unknown type with variable : " << m_Tokens[currentI].info << " on ligne " << m_Tokens[currentI].ligne << std::endl;
					std::exit(1);
				}
				
			}
			else
			{
				std::cout << "Parser : Uncorect type " << m_Tokens[currentI].info << " found inside par ligne : " << m_Tokens[currentI].ligne << std::endl;
				std::exit(1);
				return false;
			}

			currentI++;
			if (currentI >= m_Tokens.size())
			{
				std::cout << "Parser : Error of parsing missing c2 to close parameters : " << std::endl;
				std::exit(1);
				return false;
			}
		}
	}
	else
	{
		std::cout << "Parser : Parser : missing c1 on ligne " << m_Tokens[currentI].ligne << std::endl;
		std::exit(1);
		return false;
	}
	return false;
}

bool Parser::CheckReturn(Node* fn)
{
	//we know that before current i there was a return
	//if nothing return with true without changing i
	ReturnNode* returnNode = new ReturnNode;
	fn->childs.push_back(returnNode);

	if (m_Tokens[currentI].type == RIGHT_BRACE)
	{
		return true;
	}
	else if (m_Tokens[currentI].type == IDENTIFIER || m_Tokens[currentI].type == INT)
	{
		if (m_Tokens[currentI].type == IDENTIFIER)
		{
			VarNode* varNode = new VarNode;
			varNode->name = m_Tokens[currentI].info;
			varNode->line = m_Tokens[currentI].ligne;
			varNode->col = m_Tokens[currentI].column;

			returnNode->childs.push_back(varNode);
		}
		else
		{
			IntNode* intNode = new IntNode;
			intNode->value = std::stoi(m_Tokens[currentI].info);
			intNode->line = m_Tokens[currentI].ligne;
			intNode->col = m_Tokens[currentI].column;

			returnNode->childs.push_back(intNode);
		}

		currentI++;

		return true;
	}
	else
	{
		std::cout << "Parser : incorrect type of return : " << m_Tokens[currentI].info << " on ligne " << m_Tokens[currentI].ligne << std::endl;
		std::exit(1);
		return false;
	}

	return false;
}

bool Parser::CheckIdentifieur(VarNode* vn, Node* fn)
{
	if (m_Tokens[currentI].type == EQUAL)
	{
		BinaryNode* bn = new BinaryNode;
		bn->op = '=';
		bn->line = m_Tokens[currentI].ligne;
		bn->col = m_Tokens[currentI].column;
		bn->childs.push_back(vn);

		fn->childs.push_back(bn);

		currentI++;
		if (m_Tokens[currentI].type == LEFT_PAR)
		{
			currentI++;
			if (CheckOperation(bn))
				if (m_Tokens[currentI].type == RIGHT_PAR)
				{
					currentI++;
				}
				else
				{
					cout << "Parser : missing c2 on ligne " << m_Tokens[currentI].ligne << endl;
					std::exit(1);
				}
		}
		else
		{
			CheckOperation(bn);
		}
	}
	else if (m_Tokens[currentI].type == LEFT_PAR) //means it a function call
	{
		CallFunctionNode* cn = new CallFunctionNode;
		cn->line = m_Tokens[currentI].ligne;
		cn->col = m_Tokens[currentI].column;
		cn->name = vn->name;
		fn->childs.push_back(cn);
		//serie of identifieur or int
		bool insidePar = true;
		currentI++;
		while (insidePar)
		{
			if (m_Tokens[currentI].type == RIGHT_PAR)
			{
				insidePar = false;
				currentI++;
				continue;
			}
			else if (m_Tokens[currentI].type == IDENTIFIER)
			{
				VarNode* vn = new VarNode;
				vn->name = m_Tokens[currentI].info;
				vn->line = m_Tokens[currentI].ligne;
				vn->col = m_Tokens[currentI].column;
				cn->childs.push_back(vn);

				currentI++;
			}
			else if (m_Tokens[currentI].type == INT)
			{
				IntNode* in = new IntNode;
				in->value = std::stoi(m_Tokens[currentI].info);
				in->line = m_Tokens[currentI].ligne;
				in->col = m_Tokens[currentI].column;
				cn->childs.push_back(in);

				currentI++;
			}
			else
			{
				cout << "Parser : Error in parameters of try calling function" << endl;
				std::exit(1);
			}
		}
	}
	return false;
}

bool Parser::CheckOperation(BinaryNode* bn)
{
	if (m_Tokens[currentI].type == IDENTIFIER || m_Tokens[currentI].type == INT)
	{
		Node* newNode;

		if (m_Tokens[currentI].type == IDENTIFIER)
		{
			//check if call function or var

			if (m_Tokens[currentI + 1].type == LEFT_PAR)
			{
				//is a function call
				CallFunctionNode* cn = new CallFunctionNode;
				cn->line = m_Tokens[currentI].ligne;
				cn->col = m_Tokens[currentI].column;
				cn->name = m_Tokens[currentI].info;
				newNode = cn;
				bool insidePar = true;
				currentI += 2;
				while (insidePar)
				{
					if (m_Tokens[currentI].type == RIGHT_PAR)
					{
						insidePar = false;
						currentI++;
						continue;
					}
					else if (m_Tokens[currentI].type == IDENTIFIER)
					{
						VarNode* vn = new VarNode;
						vn->name = m_Tokens[currentI].info;
						vn->line = m_Tokens[currentI].ligne;
						vn->col = m_Tokens[currentI].column;
						cn->childs.push_back(vn);

						currentI++;
					}
					else if (m_Tokens[currentI].type == INT)
					{
						IntNode* in = new IntNode;
						in->value = std::stoi(m_Tokens[currentI].info);
						in->line = m_Tokens[currentI].ligne;
						in->col = m_Tokens[currentI].column;
						cn->childs.push_back(in);

						currentI++;
					}
					else
					{
						cout << "Parser : Error in parameters of try calling function" << endl;
						std::exit(1);
					}
				}
				currentI--;
			}
			else
			{
				VarNode* vn = new VarNode;
				vn->name = m_Tokens[currentI].info;
				vn->line = m_Tokens[currentI].ligne;
				vn->col = m_Tokens[currentI].column;
				newNode = vn;
			}
		}
		else
		{
			IntNode* in = new IntNode;
			in->value = std::stoi(m_Tokens[currentI].info);
			in->line = m_Tokens[currentI].ligne;
			in->col = m_Tokens[currentI].column;
			newNode = in;
		}
		currentI++;
		if ((int)TokenType::ADD <= (int)m_Tokens[currentI].type && (int)m_Tokens[currentI].type <= (int)TokenType::DIV)
		{
			BinaryNode* newBn = new BinaryNode;
			newBn->childs.push_back(newNode);
			newBn->line = m_Tokens[currentI].ligne;
			newBn->col = m_Tokens[currentI].column;
			newNode = newBn;

			switch (m_Tokens[currentI].type)
			{
			case TokenType::EQUAL:
			{
				newBn->op = '=';
				break;
			}
			case TokenType::ADD:
			{
				newBn->op = '+';
				break;
			}
			case TokenType::SUBS:
			{
				newBn->op = '-';
				break;
			}
			case TokenType::DIV:
			{
				newBn->op = '/';
				break;
			}
			case TokenType::TIMES:
			{
				newBn->op = '*';
				break;
			}
			}
			currentI++;
			if (CheckOperation(newBn));
			{
				bn->childs.push_back(newBn);
				return true;
			}
		}
		else
		{
			bn->childs.push_back(newNode);
			return true;
		}
	}
	return false;
}

Node* Parser::ParserCheck(std::vector<Token> tokens)
{
	m_Tokens = tokens;

	GlobalNode* ng = new GlobalNode;
	nodeG = static_cast<Node*>(ng);

	while (currentI < tokens.size())//for each function
	{
		FunctionNode* fn = new FunctionNode;
		fn->line = tokens[currentI].ligne;
		fn->col = tokens[currentI].column;
		nodeG->childs.push_back(fn);

		//start checking start of function
		if (tokens[currentI].type == TYPE)
		{
			if (tokens[currentI].info == "tb12")
				fn->returnType = Type::VOID;
			else
				fn->returnType = Type::INT;
			currentI++;
			//check if name of function correct
			if (tokens[currentI].type == IDENTIFIER)
			{
				fn->name = tokens[currentI].info;
				currentI++;
				if (CheckParCondition(fn))
					CheckBrace(fn);
			}
			else
			{
				cout << "Parser : uncorrect name for a function " << tokens[currentI].info << " its not an identifieur on ligne " << tokens[currentI].ligne << endl;
				return nodeG;
			}
		}
		else
		{
			cout << "Parser : missing type before the function ligne " << tokens[currentI].ligne << endl;
			std::exit(1);
		}
	}

	return nodeG;
}