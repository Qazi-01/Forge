#include "lexer.h"
#include "token.h"

static char advance(Lexer *lexer)
{
    char character = *lexer->current;

    if (character != '\0')
    {
        lexer->current++;
        lexer->column++;
    }

    return character;
}

static char peek(const Lexer *lexer)
{
    return *lexer->current;
}

static char peek_next(const Lexer *lexer)
{
    if (*lexer->current == '\0')
    {
        return '\0';
    }

    return *(lexer->current + 1);
}

static int match(Lexer *lexer, char expected)
{
    if (peek(lexer) != expected)
    {
        return 0;
    }

    advance(lexer);
    return 1;
}

void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->start = source;
    lexer->current = source;
    
    lexer->line = 1;
    lexer->column = 1;
}

static Token number(Lexer *lexer)
{
    while (peek(lexer) >= '0' && peek(lexer) <= '9')
    {
        advance(lexer);
    }

    Token token;
    token.type = TOKEN_INTEGER;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;

    return token;
}

static int is_alpha(char character)
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || character == '_';
}

static int is_digit(char character)
{
    return character >= '0' && character <= '9';
}

static Token error_token(Lexer *lexer)
{
    Token token;

    token.type = TOKEN_ERROR;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;

    return token;
}

static Token string(Lexer *lexer)
{
    while (peek(lexer) != '"' && peek(lexer) != '\0')
    {
        if (peek(lexer) == '\n')
        {
            advance(lexer);
            lexer->line++;
            lexer->column = 1;
        }

        else
        {
            advance(lexer);
        }
    }

    if (peek(lexer) == '\0')
    {
        return error_token(lexer);
    }

    advance(lexer);

    Token token;

    token.type = TOKEN_STRING;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;

    return token;
}

static TokenType identifier_type(Lexer *lexer)
{
    int length = (int)(lexer->current - lexer->start);

    if (length == 2 && lexer->start[0] == 'i' && lexer->start[1] == 'f')
    {
        return TOKEN_IF;
    }

    if (length == 2 && lexer->start[0] == 'f' && lexer->start[1] == 'n')
    {
        return TOKEN_FN;
    }

    if (length == 3 && lexer->start[0] == 'l' && lexer->start[1] == 'e' && lexer->start[2] == 't')
    {
        return TOKEN_LET;
    }

    if (length == 4 && lexer->start[0] == 'e' && lexer->start[1] == 'l' && lexer->start[2] == 's' && lexer->start[3] == 'e')
    {
        return TOKEN_ELSE;
    }

    if (length == 5 && lexer->start[0] == 'w' && lexer->start[1] == 'h' && lexer->start[2] == 'i' && lexer->start[3] == 'l' && lexer->start[4] == 'e')
    {
        return TOKEN_WHILE;
    }

    if (length == 6 && lexer->start[0] == 'r' && lexer->start[1] == 'e' && lexer->start[2] == 't' && lexer->start[3] == 'u' && lexer->start[4] == 'r' && lexer->start[5] == 'n')
    {
        return TOKEN_RETURN;
    }

    if (length == 4 && lexer->start[0] == 't' && lexer->start[1] == 'r' && lexer->start[2] == 'u' && lexer->start[3] == 'e')
    {
        return TOKEN_TRUE;
    }

    if (length == 5 && lexer->start[0] == 'f' && lexer->start[1] == 'a' && lexer->start[2] == 'l' && lexer->start[3] == 's' && lexer->start[4] == 'e')
    {
        return TOKEN_FALSE;
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer *lexer)
{
    while (is_alpha(peek(lexer)) || is_digit(peek(lexer)))
    {
        advance(lexer);
    }

    Token token;
    token.type = identifier_type(lexer);
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;

    return token;
}

Token lexer_next_token(Lexer *lexer)
{
    while(1)
    {
        char character = peek(lexer);

        if (character == ' ' || character == '\t' || character == '\r')
        {
            advance(lexer);
            continue;
        }

        if (character == '\n')
        {
            advance(lexer);
            lexer->line++;
            lexer->column = 1;
            continue;
        }

        break;
    }

    lexer->start = lexer->current;
    char character = advance(lexer);

    Token token;

    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = lexer->column - token.length;

    switch (character)
    {
        case '\0': token.type = TOKEN_EOF;
        break;

        case '+': token.type = TOKEN_PLUS;
        break;

        case '-': token.type = TOKEN_MINUS;
        break;

        case '*': token.type = TOKEN_STAR;
        break;

        case '/':
            if (match(lexer, '/'))
            {
                while (peek(lexer) != '\n' && peek(lexer) != '\0')
                {
                    advance(lexer);
                }

                return lexer_next_token(lexer);
            }

            token.type = TOKEN_SLASH;
            break;
            

        case '(': token.type = TOKEN_LEFT_PAREN;
        break;

        case ')': token.type = TOKEN_RIGHT_PAREN;
        break;

        case '{': token.type = TOKEN_LEFT_BRACE;
        break;

        case '}': token.type = TOKEN_RIGHT_BRACE;
        break;

        case '[': token.type = TOKEN_LEFT_BRACKET;
        break;

        case ']': token.type = TOKEN_RIGHT_BRACKET;
        break;

        case '"': return string(lexer);

        case ',': token.type = TOKEN_COMMA;
        break;

        case '.': token.type = TOKEN_DOT;
        break;

        case ';': token.type = TOKEN_SEMICOLON;
        break;

        case '=':
            if (match(lexer, '='))
            {
                token.type = TOKEN_EQUAL_EQUAL;
                token.length++;
            }

            else
            {
                token.type = TOKEN_EQUAL;
            }
            break;

        case '!':
            if (match(lexer, '='))
            {
                token.type = TOKEN_BANG_EQUAL;
                token.length++;
            }

            else
            {
                token.type = TOKEN_BANG;
            }
            break;

        case '<':
            if (match(lexer, '='))
            {
                token.type = TOKEN_LESS_EQUAL;
                token.length++;
            }

            else
            {
                token.type = TOKEN_LESS;
            }
            break;

        case '>':
            if (match(lexer, '='))
            {
                token.type = TOKEN_GREATER_EQUAL;
                token.length++;
            }

            else
            {
                token.type = TOKEN_GREATER;
            }
            break;

        default:
            if (is_digit(character))
            {
                return number(lexer);
            }

            if (is_alpha(character))
            {
                return identifier(lexer);
            }
            
            return error_token(lexer);
    }

    return token;
}