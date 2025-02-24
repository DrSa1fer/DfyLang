#pragma once

#include "token.h"
#include "node.h"

#ifndef PARSER_LOG

# define PARSER_TRACE(fmt, ...)\
    do { fprintf(stderr, "[parser](trace): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_DEBUG(fmt, ...)\
    do { fprintf(stderr, "[parser](debug): " fmt "\n", ##__VA_ARGS__); } while (0);
# define PARSER_ERROR(fmt, ...)\
    do { fprintf(stderr, "[parser](error): " fmt "\n", ##__VA_ARGS__); } while (0);

#endif

struct parser;
typedef struct parser parser_t;

struct parser {
    token_t *tokens;
    size_t seek;
};

node_t *parse(parser_t *parser);