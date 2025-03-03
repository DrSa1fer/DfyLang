#pragma once

#ifndef LEXER_LOG
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

#include <stdlib.h>
#include "token.h"

/// Representation of a lexer
typedef struct Tokenizer {
    char *chars;
    size_t seek;
} Tokenizer_t;

/// Representation of a keyword
typedef struct Keyword  { char *value; int type; } Keyword_t;
/// Representation of an operator
typedef struct Operator { char *value; int type; } Operator_t;
/// Representation of a literal
typedef struct Literal  { int (*handler)(Tokenizer_t *lexer, Token_t *token); } Literal_t;

/// Set of dfy language operators
extern Operator_t operators [];
/// Set of dfy language keywords
extern Keyword_t  keywords  [];
/// Set of dfy language literals
extern Literal_t  literals  [];

/// Generate array of tokens via lexer
Token_t *Tokenize(Tokenizer_t *lexer);