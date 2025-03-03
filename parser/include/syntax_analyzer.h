#pragma once

#include "ast_node.h"
#include "lexeme.h"

#ifndef PARSER_LOG

# define PARSER_TRACE(fmt, ...)\
    do { fprintf(stderr, "[parser](trace): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_DEBUG(fmt, ...)\
    do { fprintf(stderr, "[parser](debug): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_ERROR(fmt, ...)\
    do { fprintf(stderr, "[parser](error): " fmt "\n", ##__VA_ARGS__); } while (0);

#endif

typedef struct SyntaxAnalyzer {
    Lexeme_t *tokens;
    size_t seek;

    position_t position;
} SyntaxAnalyzer;

AST_t *parse(SyntaxAnalyzer *parser);