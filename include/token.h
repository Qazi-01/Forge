#ifndef FORGE_TOKEN_H
#define FORGE_TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_ERROR,

    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_STRING,

    TOKEN_LET,
    TOKEN_FN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,

    TOKEN_BANG,
    TOKEN_BANG_EQUAL,

    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,

    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,

    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,

    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,

    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_SEMICOLON
} TokenType;

typedef struct {
    TokenType type;
    const char *start;

    int length;
    int line;
    int column;
} Token;

const char *token_type_name(TokenType type);

#endif