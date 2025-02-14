#pragma once
#include "lexer.h"
#include "token.h"
#include "node.h"

struct parser;
typedef struct parser parser_t;

struct parser {
    lexer_t *lexer;
    token_t *token;
};

node_t *parse(parser_t *parser);