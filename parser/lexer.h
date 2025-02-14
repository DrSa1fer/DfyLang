#pragma once

#include "token.h"

#define LEXER_PANIC(lexer, fmt, ...)                                                      \
    do {                                                                                  \
        fprintf(stderr, "[%d:%d] " fmt "\n", lexer->row, lexer->column, ##__VA_ARGS__);   \
    } while (0);

struct lexer;
typedef struct lexer lexer_t;

struct lexer {
    const char *text;
    unsigned int seek;

    unsigned int column;
    unsigned int row;
};

token_t *lex(lexer_t *lexer);