#include "Lexer.h"

bool Lexer::CheckAuthorizedCharacter(char c)
{
    if (isalpha(c))
        return true;
    if (c == '_')
        return true;
    if (isdigit(c))
        return true;

    return false;
}

void Lexer::CheckIfOpratorOrPBH(Token* token)
{
    std::string txt = token->info;

    if (token->info == "ma")
    {
        token->type = TokenType::EQUAL;
    }
    else if (token->info == "mb")
    {
        token->type = TokenType::ADD;
    }
    else if (token->info == "mc")
    {
        token->type = TokenType::SUBS;
    }
    else if (token->info == "md")
    {
        token->type = TokenType::TIMES;
    }
    else if (token->info == "me")
    {
        token->type = TokenType::DIV;
    }
    else if (token->info == "f1")
    {
        token->type = TokenType::IS;
    }
    else if (token->info == "f2")
    {
        token->type = TokenType::SI;
    }
    else if (token->info == "c1")
    {
        token->type = TokenType::LEFT_PAR;
    }
    else if (token->info == "c2")
    {
        token->type = TokenType::RIGHT_PAR;
    }
    else if (token->info == "c5")
    {
        token->type = TokenType::LEFT_BRACE;
    }
    else if (token->info == "c6")
    {
        token->type = TokenType::RIGHT_BRACE;
    }
    else if (token->info == "c8")
    {
        token->type = TokenType::LEFT_HOOK;
    }
    else if (token->info == "c9")
    {
        token->type = TokenType::RIGHT_HOOK;
    }
    else if (token->info == "t2")
    {
        token->type = TokenType::WHILE;
    }
    else if (token->info == "t3")
    {
        token->type = TokenType::IF;
    }
    else if (token->info == "t7")
    {
        token->type = TokenType::EIF;
    }
    else if (token->info == "t4")
    {
        token->type = TokenType::ELSE;
    }
    else if (token->info == "t6")
    {
        token->type = TokenType::SWITCH;
    }
    else if (token->info == "t5")
    {
        token->type = TokenType::RETURN;
    }
    else if (token->info == "tb12" || token->info == "tb11")
    {
        token->type = TokenType::TYPE;
    }
    else if (token->info == "P")
    {
        token->type = TokenType::PRINT;
    }
    else if (token->info == "c3")
    {
        token->type = TokenType::STOP;
    }
    else if (token->info == "DrawColor")
    {
        token->type = TokenType::DRAWCOLOR;
    }
    else if (token->info == "DrawPos")
    {
        token->type = TokenType::DRAWPOS;
    }
}

std::vector<Token> Lexer::LexerCheck(std::vector<std::string> text)
{
    std::vector<Token> tokens;

    int ligneID = 1;
    for (std::string ligne : text)
    {
        for (int i = 0; i < ligne.size(); i++)
        {
            if (ligne[i] == ' ' || ligne[i] == '\t')
                continue;

            Token token;
            token.ligne = ligneID;
            token.column = i + 1;

            //check type
            if (isdigit(ligne[i]))
                token.type = TokenType::INT;
            else
                token.type = TokenType::IDENTIFIER;

            bool endWord = false;
            while (!endWord && i < ligne.size()) //recuper mots jusque a espace
            {
                if (ligne[i] == ' ')
                {
                    endWord = true;
                }
                else
                {
                    //check character
                    if (token.type == TokenType::INT && ligne[i] == '.')
                    {
                        std::cout << "Lexer : Float arent allowed at position : " << token.ligne << " ligne, " << token.column << " column" << std::endl;
                        std::exit(1);
                    }

                    else if (token.type == TokenType::IDENTIFIER && !CheckAuthorizedCharacter(ligne[i]))//check authorized character for an identifier
                    {
                        token.info.push_back(ligne[i]);
                        std::cout << "Lexer : Error on indentifier : " << token.info << std::endl;
                        std::exit(1);
                    }

                    token.info.push_back(ligne[i]);

                    i++;
                }
            }
            CheckIfOpratorOrPBH(&token);
            tokens.push_back(token);
        }
        ligneID++;
    }
    

    return tokens;
}
