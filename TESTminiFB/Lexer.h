#pragma once
#include "pch.h"
#include "Token.h"

class Lexer
{
private:
	static bool CheckAuthorizedCharacter(char c);
	static void CheckIfOpratorOrPBH(Token* token);

public:
	static std::vector<Token> LexerCheck(std::vector<std::string> text);
};

