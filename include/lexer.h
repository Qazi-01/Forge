#ifndef FORGE_LEXER_H
#define FORGE_LEXER_H

#include "token.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;

    int line;
    int column;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token lexer_next_token(Lexer *lexer);

#endif