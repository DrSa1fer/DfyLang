#pragma once

#include "lexer.h"
#include "token.h"
#include "node.h"

#ifndef PARSER_PANIC //override if missing
# define PARSER_PANIC(parser, fmt, ...)                                               \
    do {                                                                             \
        /* if(parser->token_seek > 0) {                                              \
             token_t *token = (*parser->tokens) + parser->token_seek - 1;            \
             fprintf(stderr, "(syn): [row(%d:%d), col(%d-%d)]{\'%s\'}\n" fmt "\n",   \
             token->row_start, token->row_end,                                       \
             token->col_start, token->col_end,                                       \
             token->value, ##__VA_ARGS__);                                           \
         }*/                                                                         \
        fprintf(stderr, "(syn): [row(0), col(0)]\n" fmt "\n", ##__VA_ARGS__);        \
    } while (0);
#endif //PARSER_PANIC

struct parser;
typedef struct parser parser_t;

struct parser {
    lexer_t *lexer;

    token_t *tokens;
    size_t seek;
};

node_t *parse(parser_t *parser);