#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../grammar.h"
#include "lexer.h"

/// Return current symbol of lexer`s text
static char current(const lexer_t *lexer);
/// Advance current seek of lexer`s text
static void advance(lexer_t *lexer);
/// Retreat current seek of lexer`s text
static void retreat(lexer_t *lexer);

struct keyword  { char *value; size_t size; int type; };
struct operator { char *value; size_t size; int type; };

//require order by length
static struct keyword keywords[] = {
    {"space",  5,TOKEN_TYPE_NAMESPACE           },
    {"finally",7,TOKEN_TYPE_FINALLY             },
    {"while",  5,TOKEN_TYPE_WHILE               },
    {"until",  5,TOKEN_TYPE_UNTIL               },
    {"type",   4,TOKEN_TYPE_TYPE                },
    {"enum",   4,TOKEN_TYPE_ENUM                },
    {"func",   4,TOKEN_TYPE_FUNC                },
    {"else",   4,TOKEN_TYPE_ELSE                },
    {"elif",   4,TOKEN_TYPE_ELIF                },
    {"var",    3,TOKEN_TYPE_VAR                 },
    {"val",    3,TOKEN_TYPE_VAL                 },
    {"for",    3,TOKEN_TYPE_FOR                 },
    {"if",     2,TOKEN_TYPE_IF                  },
    {"is",     2,TOKEN_TYPE_IS                  },
    {"eq",     2,TOKEN_TYPE_EQ                  },
};

//require order by length
static struct operator operators[] = {
    {"===", 3, TOKEN_TYPE_IS                    },
    {"!==", 3,TOKEN_TYPE_IS_NOT                 },

    {"==",  2, TOKEN_TYPE_EQ                    },
    {"!=",  2, TOKEN_TYPE_EQ_NOT                },

    {"+=",  2, TOKEN_TYPE_ASSIGN_PLUS           },
    {"-=",  2, TOKEN_TYPE_ASSIGN_MINUS          },
    {"*=",  2, TOKEN_TYPE_ASSIGN_MULTIPLY       },
    {"/=",  2, TOKEN_TYPE_ASSIGN_DIVIDE         },
    {"%=",  2, TOKEN_TYPE_ASSIGN_MODULUS        },
    {"++",  2, TOKEN_TYPE_INCREMENT             },
    {"--",  2, TOKEN_TYPE_DECREMENT             },
    {"=",   1, TOKEN_TYPE_ASSIGN                },

    {"+",   1, TOKEN_TYPE_PLUS                  },
    {"-",   1, TOKEN_TYPE_MINUS                 },
    {"*",   1, TOKEN_TYPE_MULTIPLY              },
    {"/",   1, TOKEN_TYPE_DIVIDE                },
    {"%",   1, TOKEN_TYPE_MODULUS               },

    {"(",   1, TOKEN_TYPE_PARENTHESIS_LEFT      },
    {")",   1, TOKEN_TYPE_PARENTHESIS_RIGHT     },
    {"{",   1, TOKEN_TYPE_CURLY_BRACKET_LEFT    },
    {"}",   1, TOKEN_TYPE_CURLY_BRACKET_RIGHT   },
    {"[",   1, TOKEN_TYPE_SQUARE_BRACKET_LEFT   },
    {"]",   1, TOKEN_TYPE_SQUARE_BRACKET_RIGHT  },
    {"<",   1, TOKEN_TYPE_ANGEL_BRACKET_LEFT    },
    {">",   1, TOKEN_TYPE_ANGEL_BRACKET_RIGHT   },

    {";",   1, TOKEN_TYPE_SEMICOLON             },
    {":",   1, TOKEN_TYPE_COLON                 },
    {".",   1, TOKEN_TYPE_DOT                   },
    {",",   1, TOKEN_TYPE_COMMA                 },

    {" ",   1, TOKEN_TYPE_SPACE                 },
    {"\t",  1, TOKEN_TYPE_TAB                   },
    {"\n",  1, TOKEN_TYPE_NEWLINE               },
    {"\r",  1, TOKEN_TYPE_NEWLINE               },
    {"\0",  1, TOKEN_TYPE_EOF                   },
};

static size_t cached_keyword_max_len = 0;
static size_t cached_operator_max_len = 0;

static size_t keyword_max_len(void) {
    if (cached_keyword_max_len > 0) {
        return cached_keyword_max_len;
    }
    const size_t count = sizeof(keywords) / sizeof(*keywords);
    for (size_t i = 0; i < count; i++) {
        const size_t t = keywords[i].size;
        if (t > cached_keyword_max_len) {
            cached_keyword_max_len = t;
        }
    }
    return cached_keyword_max_len;
}
static size_t operator_max_len(void) {
    if (cached_operator_max_len > 0) {
        return cached_operator_max_len;
    }
    const size_t count = sizeof(operators) / sizeof(*operators);
    for (size_t i = 0; i < count; i++) {
        const size_t t = operators[i].size;
        if (t > cached_operator_max_len) {
            cached_operator_max_len = t;
        }
    }
    return cached_operator_max_len;
}

token_t *lex_string(lexer_t *lexer);
token_t *lex_number(lexer_t *lexer);
token_t *lex_boolean(lexer_t *lexer); //not implemented now.
token_t *lex_comment(lexer_t *lexer);

token_t *lex_keyword(lexer_t *lexer);
token_t *lex_operator(lexer_t *lexer);

token_t *lex_identifier(lexer_t *lexer);

token_t *lex(lexer_t *lexer) {
    size_t size = 100;
    size_t seek = 0;
    token_t *buf = malloc(sizeof(*buf) * size);

    if (buf == NULL) {
        return NULL;
    }

    while(1) {
        token_t *token = NULL;

        if (token == NULL) { token = lex_number     (lexer); }
        if (token == NULL) { token = lex_comment    (lexer); }
        if (token == NULL) { token = lex_string     (lexer); }
        if (token == NULL) { token = lex_keyword    (lexer); }
        if (token == NULL) { token = lex_operator   (lexer); }
        if (token == NULL) { token = lex_identifier (lexer); }

        if (token == NULL) {
            LEXER_PANIC(lexer, "Invalid token '%c'", current(lexer));
            advance(lexer);
            free(buf);
            return NULL;
        }

        if (seek >= size) {
             void *tmp = realloc(buf, sizeof(*buf) * (size *= 2));
             if (tmp == NULL) {
                 LEXER_PANIC(lexer, "Out of memory");
                 free(buf);
                 free(token);
                 return NULL;
             }
             buf = tmp;
        }
        buf[seek++] = *token;

        if (token->type == TOKEN_TYPE_EOF) {
            token_t *ptr = malloc(sizeof *ptr * seek);
            if (ptr == NULL) {
                LEXER_PANIC(lexer, "Out of memory");
                free(buf);
                free(token);
                return NULL;
            }

            for (size_t i = 0; i < seek; i++) {
                ptr[i] = buf[i];
            }

            free(buf);
            free(token);
            return ptr;
        }

        free(token);
    }
}
token_t *lex_string(lexer_t *lexer) {
    if (current(lexer) != '"' && current(lexer) != '\'') {
        return NULL;
    }

    size_t size = 15;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        return NULL;
    }

    const char open = current(lexer);
    advance(lexer); //skip open

    while (current(lexer) != open) {
        if (current(lexer) == '\0') {
            buf[seek] = '\0';
            LEXER_PANIC(lexer, "String \'%s\' await end with '%c'", buf, open);
            free(buf);
            return NULL;
        }
        if (current(lexer) == '\\') {
            advance(lexer);
            switch (current(lexer)) {
                case '\\': {
                    advance(lexer);
                    buf[seek++] = '\\';
                    continue;
                }
                case '\'': {
                    advance(lexer);
                    buf[seek++] = '\'';
                    continue;
                }
                case '\"': {
                    advance(lexer);
                    buf[seek++] = '\"';
                    continue;
                }
                case 'n': {
                    advance(lexer);
                    buf[seek++] = '\n';
                    continue;
                }
                case 'r': {
                    advance(lexer);
                    buf[seek++] = '\r';
                    continue;
                }
                case 't': {
                    advance(lexer);
                    buf[seek++] = '\t';
                    continue;
                }
                case 'b': {
                    advance(lexer);
                    buf[seek++] = '\b';
                    continue;
                }
                case 'f': {
                    advance(lexer);
                    buf[seek++] = '\f';
                    continue;
                }
                case 'a': {
                    advance(lexer);
                    buf[seek++] = '\a';
                    continue;
                }
                case 'v': {
                    advance(lexer);
                    buf[seek++] = '\v';
                    continue;
                }
                case '0': {
                    advance(lexer);
                    buf[seek++] = '\0';
                    continue;
                }
                default: {
                    LEXER_PANIC(lexer, "Invalid escape character '\\%c'", current(lexer));
                    free(buf);
                    return NULL;
                }
            }
        }
        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(tmp);
                free(buf);
                LEXER_PANIC(lexer, "Out of memory");
                return NULL;
            }
            buf = tmp;
        }
        buf[seek++] = current(lexer);
        advance(lexer);
    }
    advance(lexer); //skip close
    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(ptr);
        free(buf);
        LEXER_PANIC(lexer, "Out of memory");
        return NULL;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    token_t *token = malloc(sizeof *token);
    token->type = TOKEN_TYPE_STRING;
    token->value = ptr;
    return token;
}
token_t *lex_number(lexer_t *lexer) {
    if (current(lexer) != '.' &&  !isdigit(current(lexer))) {
        return NULL;
    }

    char is_init = 0;
    double value = 0;
    while (isdigit(current(lexer))) {
        value = value * 10 + (current(lexer) - '0');
        advance(lexer);
        is_init = 1;
    }
    if (current(lexer) == '.') {
        advance(lexer); //skip .
        if (isdigit(current(lexer))) {
            double fraction = 1;
            while (isdigit(current(lexer))) {
                fraction /= 10;
                value += (current(lexer) - '0') * fraction;
                advance(lexer);
            }
        }
        else {
            retreat(lexer);
            if (!is_init) {
                return NULL;
            }
        }
    }

    token_t *token = malloc(sizeof *token);
    double *ptr = malloc(sizeof *ptr);
    *ptr = value;
    token->type = TOKEN_TYPE_NUMBER;
    token->value = ptr;
    return token;
}
token_t *lex_comment(lexer_t *lexer) {
    if (current(lexer) != '#') {
        return NULL;
    }

    size_t size = 15;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        return NULL;
    }

    advance(lexer);//skip '#'
    while (current(lexer) != '\n' && current(lexer) != '\r' && current(lexer) != '\0') {
        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(tmp);
                free(buf);
                LEXER_PANIC(lexer, "Out of memory");
                return NULL;
            }
            buf = tmp;
        }
        buf[seek++] = current(lexer);
        advance(lexer);
    }
    advance(lexer); //'\n'
    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(ptr);
        free(buf);
        LEXER_PANIC(lexer, "Out of memory");
        return NULL;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    token_t *token = malloc(sizeof *token);
    token->type = TOKEN_TYPE_COMMENT;
    token->value = ptr;
    return token;
}
token_t *lex_keyword(lexer_t *lexer) {
    const size_t max_len = keyword_max_len();
    const size_t count = sizeof(keywords) / sizeof(*keywords);
    char buf[max_len + 1]; memset(buf, 0, sizeof *buf);

    for (int i = 0; i < max_len; i++) {
        buf[i] = current(lexer);
        advance(lexer);
    }
    buf[max_len] = '\0';

    for (int i = 0; i < count; i++) {
        const struct keyword *keyword = &keywords[i];
        const size_t key_size = keyword->size;
        const size_t buf_size = max_len;

        if (strncmp(buf, keyword->value, key_size) == 0) {
            for (size_t r = buf_size - key_size; r > 0; r--) {
                retreat(lexer);
            }
            token_t *token = malloc(sizeof *token);
            token->type = keyword->type;
            token->value = NULL;
            return token;
        }
    }
    for (int i = 0; i < max_len; i++) {
        retreat(lexer);
    }
    return NULL;
}
token_t *lex_operator(lexer_t *lexer) {
    const size_t max_len = operator_max_len();
    const size_t count = sizeof(operators) / sizeof(*operators);
    char buf[max_len + 1]; memset(buf, 0, sizeof *buf);

    for (int i = 0; i < max_len; i++) {
        buf[i] = current(lexer);
        advance(lexer);
    }
    buf[max_len] = '\0';

    for (int i = 0; i < count; i++) {
        const struct operator *operator = &operators[i];
        const size_t opr_size = operator->size;
        const size_t buf_size = max_len;

        if (strncmp(buf, operator->value, opr_size) == 0) {
            for (size_t r = buf_size - opr_size; r > 0; r--) {
                retreat(lexer);
            }
            token_t *token = malloc(sizeof *token);
            token->type = operator->type;
            token->value = NULL;
            return token;
        }
    }
    for (int i = 0; i < max_len; i++) {
        retreat(lexer);
    }
    return NULL;
}
token_t *lex_identifier(lexer_t *lexer) {
    if (current(lexer) != '_' && !isalpha(current(lexer))) {
        return NULL;
    }

    size_t size = 7;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        return NULL;
    }

    buf[seek++] = current(lexer);
    while (1) {
        advance(lexer);
        if (current(lexer) != '_' && !isalnum(current(lexer))) {
            break;
        }

        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(buf);
                LEXER_PANIC(lexer, "Out of memory");
                return NULL;
            }
            buf = tmp;
        }

        buf[seek++] = current(lexer);
    }

    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(buf);
        LEXER_PANIC(lexer, "Out of memory");
        return NULL;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    token_t *token = malloc(sizeof *token);
    token->type = TOKEN_TYPE_IDENTIFIER;
    token->value = ptr;
    return token;
}

char current(const lexer_t *lexer) {
    return lexer->chars[lexer->seek];
}
void advance(lexer_t *lexer) {
    if (current(lexer) == '\n') {
        lexer->seek++;
        lexer->column = 0;
        lexer->row++;
        return;
    }

    if (current(lexer) == '\r') {
        lexer->seek++;
        lexer->column = 0;
        lexer->row++;
        if (current(lexer) == '\n') {
            lexer->seek++;
        }
        return;
    }

    lexer->column++;
    lexer->seek++;
}
void retreat(lexer_t *lexer) {
    lexer->column--;
    lexer->seek--;
}