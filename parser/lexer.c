#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lexer.h"

/// Alloc new token and set value
static struct token *new_token(int type, void *data);
/// Return current symbol of lexer`s text
static char current(const lexer_t *lexer);
/// Advance current seek of lexer`s text
static void advance(lexer_t *lexer);
/// Retreat current seek of lexer`s text
static void retreat(lexer_t *lexer);

/// Try to lex NUMBER LITERAL lexeme, return NULL if not possible
static token_t *lex_number      (lexer_t *lexer);
/// Try to lex STRING LITERAL lexeme, return NULL if not possible
static token_t *lex_string      (lexer_t *lexer);
/// Try to lex COMMENT LITERAL(TRIVIA) lexeme, return NULL if not possible
static token_t *lex_comment     (lexer_t *lexer);
/// Try to lex IDENTIFIER LITERAL lexeme, return NULL if not possible
static token_t *lex_identifier  (lexer_t *lexer);
/// Try to lex KEYWORD lexeme, return NULL if not possible
static token_t *lex_keyword     (lexer_t *lexer);
/// Try to lex OPERATOR lexeme, return NULL if not possible
static token_t *lex_operator    (lexer_t *lexer);

/// Set of dfy language keywords
keyword_t keywords[] = {
    {"finally",7,TOKEN_TYPE_FINALLY             },
    {"space",  5,TOKEN_TYPE_SPACE               },
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
/// Set of dfy language operators
operator_t operators[] = {
    {"===", 3, TOKEN_TYPE_IS                    },
    {"!==", 3, TOKEN_TYPE_IS_NOT                },

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

    {" ",   1, TOKEN_TYPE_TRIVIA_SPACE          },
    {"\t",  1, TOKEN_TYPE_TRIVIA_TAB            },
    {"\n",  1, TOKEN_TYPE_TRIVIA_NEWLINE        },
    {"\r",  1, TOKEN_TYPE_TRIVIA_NEWLINE        },
    {"\0",  1, TOKEN_TYPE_EOF                   },
};
/// Set of dfy language literals
literal_t literals[] = {
    { lex_number     },
    { lex_string     },
    { lex_comment    },
    { lex_identifier },
};

const size_t keywords_count   = sizeof(keywords)  / sizeof(*keywords);
const size_t operators_length = sizeof(operators) / sizeof(*operators);
const size_t literals_count   = sizeof(literals)  / sizeof(*literals);

token_t *lex                    (lexer_t *lexer) {
    size_t size = 100;

    token_t *buf = malloc(sizeof(*buf) * size);
    size_t seek = 0;

    if (buf == NULL) {
        LEXER_ERROR("Out of memory");
        return NULL;
    }

    while(1) {
        token_t *token = NULL;

        if (token == NULL) { token = lex_keyword    (lexer); }
        for (int i = 0; token == NULL && i < literals_count; i++) { token = literals[i].handler(lexer); }
        if (token == NULL) { token = lex_operator   (lexer); }

        if (token == NULL) {
            LEXER_ERROR("Invalid char '%c'", current(lexer));
            free(buf); return NULL;
        }
        if (seek >= size) {
             void *tmp = realloc(buf, sizeof(*buf) * (size *= 2));
             if (tmp == NULL) {
                 LEXER_ERROR("Out of memory");
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
token_t *lex_keyword            (lexer_t *lexer) {
    if (isalpha(current(lexer)) == 0) { return NULL; }

    for (int i = 0; i < keywords_count; i++) {
        const struct keyword keyword = keywords[i];

        int seek = 0;
        while(seek < keyword.size + 1) {
            if (seek == keyword.size) {
                if (isalnum(current(lexer))) {
                    break;
                }
                if (current(lexer) == '_') {
                    break;
                }
                LEXER_DEBUG("KEYWORD = {%s}", keyword.value);
                return new_token(keyword.type, NULL);
            }
            if (current(lexer) != keyword.value[seek]) {
                break;
            }
            advance(lexer); seek++;
        }
        while (seek > 0) {
            retreat(lexer); seek--;
        }
    }
    return NULL;
}
token_t *lex_operator           (lexer_t *lexer) {
    if (isalnum(current(lexer))) { return NULL; }

    for (int i = 0; i < operators_length; i++) {
        const struct operator operator = operators[i];

        int seek = 0;
        while(seek < operator.size + 1) {
            if (seek == operator.size) {
                LEXER_DEBUG("OPERATOR = {%s}", operator.value);
                return new_token(operator.type, NULL);
            }
            if (current(lexer) != operator.value[seek]) {
                break;
            }
            advance(lexer); seek++;
        }
        while (seek > 0) {
            retreat(lexer); seek--;
        }
    }

    return NULL;
}
token_t *lex_number             (lexer_t *lexer) {
    if (!isdigit(current(lexer))) {
        return NULL;
    }
    double value = 0;
    while (isdigit(current(lexer))) {
        value = value * 10 + (current(lexer) - '0');
        advance(lexer);
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
            if (!value) {
                return NULL;
            }
        }
    }

    double *data = malloc(sizeof *data);
    *data = value;

    LEXER_DEBUG("NUMBER = {%g}", value);
    return new_token(TOKEN_TYPE_LITERAL_NUMBER, data);
}
token_t *lex_string             (lexer_t *lexer) {
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
    advance(lexer); // skip open
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
    advance(lexer); // skip close

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

    LEXER_DEBUG("STRING = {%s}", ptr);
    return new_token(TOKEN_TYPE_LITERAL_STRING, ptr);
}
token_t *lex_comment            (lexer_t *lexer) {
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

    LEXER_DEBUG("COMMENT = {%s}", ptr);
    return new_token(TOKEN_TYPE_TRIVIA_COMMENT, ptr);
}
token_t *lex_identifier         (lexer_t *lexer) {
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

    LEXER_DEBUG("IDENTIFIER = {%s}", ptr);
    return new_token(TOKEN_TYPE_LITERAL_IDENTIFIER, ptr);
}

struct token *new_token(const int type, void *data) {
    struct token *token = malloc(sizeof *token);
    token->type = type;
    token->data = data;
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
    lexer->seek--;
    lexer->column--;
}