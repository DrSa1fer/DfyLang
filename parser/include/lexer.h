#pragma once

#include "token.h"

#ifndef LEXER_PANIC
#define LEXER_PANIC(lexer, fmt, ...)                                                           \
    do {                                                                                       \
        fprintf(stderr, "(lex): [row(%d), col(%d)] \n" fmt "\n", lexer->row, lexer->column, ##__VA_ARGS__);   \
    } while (0);
#endif //LEXER_PANIC

#ifdef LEXER_LOG
# define LEXER_TRACE(fmt, ...) ;
# define LEXER_DEBUG(fmt, ...) ;
# define LEXER_ERROR(fmt, ...) ;
#else

# define LEXER_TRACE(fmt, ...)\
    do { fprintf(stderr, "[lexer](trace): " fmt "\n", ##__VA_ARGS__); } while (0);
# define LEXER_DEBUG(fmt, ...)\
    do { fprintf(stderr, "[lexer](debug): " fmt "\n", ##__VA_ARGS__); } while (0);
# define LEXER_ERROR(fmt, ...)\
    do { fprintf(stderr, "[lexer](error): " fmt "\n", ##__VA_ARGS__); } while (0);

#endif

typedef struct lexer {
    char *chars;
    size_t seek;

    size_t column;
    size_t row;
} lexer_t;

/// Representation of a keyword
typedef struct keyword  { char *value; size_t size; int type; } keyword_t;
/// Representation of an operator
typedef struct operator { char *value; size_t size; int type; } operator_t;
/// Representation of a literal
typedef struct literal  { token_t* (*handler)(lexer_t *lexer); } literal_t;

extern operator_t operators [];
extern keyword_t  keywords  [];
extern literal_t  literals  [];

token_t *lex(lexer_t *lexer);