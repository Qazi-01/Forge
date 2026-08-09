#include "token.h"

const char *token_type_name(TokenType type)
{
    switch (type)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";

        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_STRING: return "STRING";

        case TOKEN_LET: return "LET";
        case TOKEN_FN: return "FN";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";

        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";

        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_EQUAL_EQUAL: return "EQUAL_EQUAL";

        case TOKEN_BANG: return "BANG";
        case TOKEN_BANG_EQUAL: return "BANG_EQUAL";

        case TOKEN_LESS: return "LESS";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";

        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";

        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";

        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";

        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_SEMICOLON: return "SEMICOLON";
    }

    return "UNKNOWN";
}