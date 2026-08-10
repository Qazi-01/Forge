#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

#define FORGE_VERSION "0.2.0"

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size < 0)
    {
        fclose(file);
        fprintf(stderr, "Could not determine file size: %s\n", path);
        return NULL;
    }

    char *buffer = malloc((size_t)size + 1);

    if (buffer == NULL)
    {
        fclose(file);
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, (size_t)size, file);

    fclose(file);

    if (bytes_read != (size_t)size)
    {
        free(buffer);
        fprintf(stderr, "Could not read file: %s\n", path);
        return NULL;
    }

    buffer[size] = '\0';

    return buffer;
}

static int tokenize_file(const char *path)
{
    char *source = read_file(path);

    if (source == NULL)
    {
        return 1;
    }

    Lexer lexer;
    lexer_init(&lexer, source);

    while (1)
    {
        Token token = lexer_next_token(&lexer);

        printf("%-15s \"%.*s\" line: %d:%d\n", token_type_name(token.type), token.length, token.start, token.line, token.column);

        if (token.type == TOKEN_ERROR)
        {
            free(source);
            return 1;
        }

        if (token.type == TOKEN_EOF)
        {
            break;
        }
    }

    free(source);
    return 0;
}

static void print_usage(const char *program)
{
    printf("Forge - programming language toolkit\n\n");
    printf("Usage:\n");
    printf("  %s tokenize <file>\n", program);
    printf("  %s version\n", program);
    printf("  %s help\n", program);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "version") == 0)
    {
        printf("Forge version %s\n", FORGE_VERSION);
        return 0;
    }

    if (strcmp(argv[1], "help") == 0)
    {
        print_usage(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "tokenize") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr,"Usage: %s tokenize <file>\n", argv[0]);
            return 1;
        }

        return tokenize_file(argv[2]);
    }

    fprintf(stderr, "Unknown command: %s\n\n", argv[1]);
    print_usage(argv[0]);
    return 1;
}