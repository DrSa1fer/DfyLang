#pragma once

#include "token.h"

#ifndef LEXER_PANIC
#define LEXER_PANIC(lexer, fmt, ...)                                                           \
    do {                                                                                       \
        fprintf(stderr, "(lex): [row(%d), col(%d)] \n" fmt "\n", lexer->row, lexer->column, ##__VA_ARGS__);   \
    } while (0);
#endif //LEXER_PANIC

struct lexer;
typedef struct lexer lexer_t;

struct lexer {
    char *chars;
    size_t seek;

    size_t column;
    size_t row;
};

token_t *lex(lexer_t *lexer);