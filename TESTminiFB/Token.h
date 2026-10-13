#pragma once

#include "pch.h"

enum TokenType
{
    IDENTIFIER,
    FUNCTION_CALL,
    LEFT_PAR,
    RIGHT_PAR,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_HOOK,
    RIGHT_HOOK,
    INT,
    TYPE,
    IS, //inferieur superieur <
    SI, // >
    EQUAL,
    ADD,
    SUBS,
    TIMES,
    DIV,
    WHILE,
    IF,
    EIF,
    ELSE,
    SWITCH,
    RETURN,
    PRINT,//only for debug
    STOP,
    DRAWCOLOR,
    DRAWPOS
};

struct Token
{
    int ligne;
    int column;
    TokenType type;
    std::string info;
};

