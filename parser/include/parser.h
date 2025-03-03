#pragma once

#include <stdlib.h>

#include "ast.h"
#include "token.h"

#ifndef PARSER_LOG

# define PARSER_TRACE(fmt, ...)\
    do { fprintf(stderr, "[parser](trace): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_DEBUG(fmt, ...)\
    do { fprintf(stderr, "[parser](debug): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_ERROR(fmt, ...)\
    do { fprintf(stderr, "[parser](error): " fmt "\n", ##__VA_ARGS__); } while (0);

#endif

typedef struct Parser {
    Token_t *tokens;
    size_t seek;
} Parser_t;

AST_t *Parse(Parser_t *parser);