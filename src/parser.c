#include "parser.h"
#include <stddef.h>

static int parse_unary(Parser *parser);
static int parse_factor(Parser *parser);
static int parse_term(Parser *parser);
static int parse_comparison(Parser *parser);
static int parse_equality(Parser *parser);
static int parse_declaration(Parser *parser);
static int parse_variable_declaration(Parser *parser);
static int parse_statement(Parser *parser);
static int parse_expression_statement(Parser *parser);
static int parse_if_statement(Parser *parser);
static int parse_while_statement(Parser *parser);
static int parse_block(Parser *parser);
static int parse_function_declaration(Parser *parser);
static int parse_return_statement(Parser *parser);
static int parse_call(Parser *parser);
static int parse_arguments(Parser *parser);

void parser_init(Parser *parser, Lexer *lexer)
{
    parser->lexer = lexer;
    parser->current = lexer_next_token(lexer);
    parser->previous = parser->current;
    parser->had_error = 0;
    parser->error_message = NULL;
}

void parser_advance(Parser *parser)
{
    parser->previous = parser->current;
    parser->current = lexer_next_token(parser->lexer);
}

int parser_consume(Parser *parser, TokenType type)
{
    if (parser->current.type != type)
    {
        parser->had_error = 1;
        parser->error_message = "unexpected token";
        return 0;
    }

    parser_advance(parser);
    return 1;
}

int parser_parse_primary(Parser *parser)
{
    switch (parser->current.type)
    {
        case TOKEN_INTEGER:
        case TOKEN_STRING:
        case TOKEN_IDENTIFIER:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            parser_advance(parser);
            return 1;

        case TOKEN_LEFT_PAREN:
            parser_advance(parser);

            if (!parser_parse_expression(parser))
            {
                return 0;
            }

            if (!parser_consume(parser, TOKEN_RIGHT_PAREN))
            {
                parser->error_message = "Expected ')'";
                return 0;
            }

            return 1;
            
        default:
            parser->had_error = 1;
            parser->error_message = "Expected expression.";
            return 0;
    }
}

static int parse_unary(Parser *parser)
{
    if (parser->current.type == TOKEN_BANG || parser->current.type == TOKEN_MINUS)
    {
        parser_advance(parser);
        return parse_unary(parser);
    }

    return parse_call(parser);
}

static int parse_factor(Parser *parser)
{
    if (!parse_unary(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_STAR || parser->current.type == TOKEN_SLASH)
    {
        parser_advance(parser);

        if (!parse_unary(parser))
        {
            return 0;
        }
    }

    return 1;
}

static int parse_term(Parser *parser)
{
    if (!parse_factor(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS)
    {
        parser_advance(parser);

        if (!parse_factor(parser))
        {
            return 0;
        }
    }

    return 1;
}

static int parse_comparison(Parser *parser)
{
    if (!parse_term(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_LESS || parser->current.type == TOKEN_LESS_EQUAL || parser->current.type == TOKEN_GREATER || parser->current.type == TOKEN_GREATER_EQUAL)
    {
        parser_advance(parser);

        if (!parse_term(parser))
        {
            return 0;
        }
    }

    return 1;
}

static int parse_equality(Parser *parser)
{
    if (!parse_comparison(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_EQUAL_EQUAL || parser->current.type == TOKEN_BANG_EQUAL)
    {
        parser_advance(parser);

        if (!parse_comparison(parser))
        {
            return 0;
        }
    }

    return 1;
}

int parser_parse_expression(Parser *parser)
{
    return parse_equality(parser);
}

static int parse_expression_statement(Parser *parser)
{
    if (!parser_parse_expression(parser))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_SEMICOLON))
    {
        parser->error_message = "expected ';'";
        return 0;
    }

    return 1;
}

static int parse_statement(Parser *parser)
{
    if (parser->current.type == TOKEN_IF)
    {
        return parse_if_statement(parser);
    }

    if (parser->current.type == TOKEN_WHILE)
    {
        return parse_while_statement(parser);
    }

    if (parser->current.type == TOKEN_RETURN)
    {
        return parse_return_statement(parser);
    }

    if (parser->current.type == TOKEN_LEFT_BRACE)
    {
        return parse_block(parser);
    }

    return parse_expression_statement(parser);
}

static int parse_variable_declaration(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_LET))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_IDENTIFIER))
    {
        parser->error_message = "expected identifier after 'let'";
        return 0;
    }

    if (!parser_consume(parser, TOKEN_EQUAL))
    {
        parser->error_message = "expected '=' after variable name";
        return 0;
    }

    if (!parser_parse_expression(parser))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_SEMICOLON))
    {
        parser->error_message = "expected ';' after variable declaration";
        return 0;
    }

    return 1;
}

static int parse_declaration(Parser *parser)
{
    if (parser->current.type == TOKEN_FN)
    {
        return parse_function_declaration(parser);
    }

    if (parser->current.type == TOKEN_LET)
    {
        return parse_variable_declaration(parser);
    }

    return parse_statement(parser);
}

int parser_parse_program(Parser *parser)
{
    while (parser->current.type != TOKEN_EOF)
    {
        if (!parse_declaration(parser))
        {
            return 0;
        }
    }

    return !parser->had_error;
}

static int parse_block(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_LEFT_BRACE))
    {
        return 0;
    }

    while (parser->current.type != TOKEN_RIGHT_BRACE && parser->current.type != TOKEN_EOF)
    {
        if (!parse_declaration(parser))
        {
            return 0;
        }
    }

    if (!parser_consume(parser, TOKEN_RIGHT_BRACE))
    {
        parser->error_message = "expected '}' at the end of block";
        return 0;
    }

    return 1;
}

static int parse_if_statement(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_IF))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_LEFT_PAREN))
    {
        parser->error_message = "expected '(' after 'if'";
        return 0;
    }

    if (!parser_parse_expression(parser))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_RIGHT_PAREN))
    {
        parser->error_message = "expected ')' after condition";
        return 0;
    }

    if (!parse_statement(parser))
    {
        return 0;
    }

    if (parser->current.type == TOKEN_ELSE)
    {
        parser_advance(parser);

        if (!parse_statement(parser))
        {
            return 0;
        }
    }

    return 1;
}

static int parse_while_statement(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_WHILE))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_LEFT_PAREN))
    {
        parser->error_message = "expected '(' after 'while'";
        return 0;
    }

    if (!parser_parse_expression(parser))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_RIGHT_PAREN))
    {
        parser->error_message = "expected ')' after condition";
        return 0;
    }

    if (!parse_statement(parser))
    {
        return 0;
    }

    return 1;
}

static int parse_function_declaration(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_FN))
    {
        return 0;
    }

    if (!parser_consume(parser, TOKEN_IDENTIFIER))
    {
        parser->error_message = "expected function name";
        return 0;
    }

    if (!parser_consume(parser, TOKEN_LEFT_PAREN))
    {
        parser->error_message = "expected '(' after function name";
        return 0;
    }

    if (parser->current.type != TOKEN_RIGHT_PAREN)
    {
        do
        {
            if (!parser_consume(parser, TOKEN_IDENTIFIER))
            {
                parser->error_message = "expected parameter name";
                return 0;
            }
        }

        while (parser->current.type == TOKEN_COMMA && (parser_advance(parser), 1));
    }

    if (!parser_consume(parser, TOKEN_RIGHT_PAREN))
    {
        parser->error_message = "expected ')' after parameters";
        return 0;
    }

    if (!parse_block(parser))
    {
        return 0;
    }
    
    return 1;
}

static int parse_return_statement(Parser *parser)
{
    if (!parser_consume(parser, TOKEN_RETURN))
    {
        return 0;
    }

    if (parser->current.type != TOKEN_SEMICOLON)
    {
        if (!parser_parse_expression(parser))
        {
            return 0;
        }
    }

    if (!parser_consume(parser, TOKEN_SEMICOLON))
    {
        parser->error_message = "expected ';' after return";
        return 0;
    }

    return 1;
}

static int parse_call(Parser *parser)
{
    if (!parser_parse_primary(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_LEFT_PAREN)
    {
        parser_advance(parser);

        if (parser->current.type != TOKEN_RIGHT_PAREN)
        {
            if (!parse_arguments(parser))
            {
                return 0;
            }
        }

        if (!parser_consume(parser, TOKEN_RIGHT_PAREN))
        {
            parser->error_message = "expected ')' after arguments";
            return 0;
        }
    }

    return 1;
}

static int parse_arguments(Parser *parser)
{
    if (!parser_parse_expression(parser))
    {
        return 0;
    }

    while (parser->current.type == TOKEN_COMMA)
    {
        parser_advance(parser);

        if (!parser_parse_expression(parser))
        {
            return 0;
        }
    }

    return 1;
}