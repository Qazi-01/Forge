#ifndef FORGE_PARSER_H
#define FORGE_PARSER_H

#include "lexer.h"
#include "token.h"

typedef struct {
    Lexer *lexer;

    Token current;
    Token previous;

    int had_error;
    const char *error_message;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);
void parser_advance(Parser *parser);
int parser_consume(Parser *parser, TokenType type);
int parser_parse_primary(Parser *parser);
int parser_parse_expression(Parser *parser);
int parser_parse_program(Parser *parser);

#endif