#include <stdio.h>

#include "lexer.h"
#include "token.h"

int main(void)
{
    const char *source = "let message = \"Hello\"";
    Lexer lexer;
    lexer_init(&lexer, source);

    while (1)
    {
        Token token = lexer_next_token(&lexer);

        printf("%-15s \"%.*s\"  line %d:%d\n", token_type_name(token.type), token.length, token.start, token.line, token.column);

        if (token.type == TOKEN_EOF)
        {
            break;
        }
    }

    return 0;
}