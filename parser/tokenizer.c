#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/tokenizer.h"

/// Try to tokenize IDENTIFIER LITERAL token.
/// Return 1 if success, otherwise 0
static int Tokenize_Identifier (Tokenizer_t *lexer, Token_t *token);
/// Try to tokenize COMMENT LITERAL(TRIVIA) token,
/// Return 1 if success, otherwise 0
static int Tokenize_Comment    (Tokenizer_t *lexer, Token_t *token);
/// Try to tokenize NUMBER LITERAL token.
/// Return 1 if success, otherwise 0
static int Tokenize_Number     (Tokenizer_t *lexer, Token_t *token);
/// Try to tokenize STRING LITERAL token.
/// Return 1 if success, otherwise 0
static int Tokenize_String     (Tokenizer_t *lexer, Token_t *token);

/// Try to tokenize KEYWORD token.
/// Return 1 if success, otherwise 0
static int Tokenize_Keyword    (Tokenizer_t *lexer, Token_t *token);
/// Try to tokenize LITERAL token. Return 1 if success, otherwise 0
/// Return 1 if success, otherwise 0
static int Tokenize_Literal    (Tokenizer_t *lexer, Token_t *token);
/// Try to tokenize OPERATOR token.
/// Return 1 if success, otherwise 0
static int Tokenize_Operator   (Tokenizer_t *lexer, Token_t *token);

/// Return current symbol of lexer`s text
static char Current(const Tokenizer_t *lexer);
/// Advance current seek of lexer`s text
static void Advance(Tokenizer_t *lexer);
/// Retreat current seek of lexer`s text
static void Retreat(Tokenizer_t *lexer);

Keyword_t keywords[] = {
    {"again",   TokenTypeAgain                  },
    {"and",     TokenTypeAnd                    },
    {"back",    TokenTypeBack                   },
    {"case",    TokenTypeCase                   },
    {"catch",   TokenTypeCatch                  },
    {"do",      TokenTypeDo                     },
    {"eject",   TokenTypeEject                  },
    {"elif",    TokenTypeElif                   },
    {"else",    TokenTypeElse                   },
    {"enum",    TokenTypeEnum                   },
    {"eq",      TokenTypeEq                     },
    {"finally", TokenTypeFinally                },
    {"for",     TokenTypeFor                    },
    {"func",    TokenTypeFunc                   },
    {"if",      TokenTypeIf                     },
    {"in",      TokenTypeIn                     },
    {"is",      TokenTypeIs                     },
    {"next",    TokenTypeNext                   },
    {"not",     TokenTypeNot                    },
    {"or",      TokenTypeOr                     },
    {"return",  TokenTypeReturn                 },
    {"space",   TokenTypeSpace                  },
    {"stop",    TokenTypeStop                   },
    {"switch",  TokenTypeSwitch                 },
    {"try",     TokenTypeTry                    },
    {"type",    TokenTypeType                   },
    {"until",   TokenTypeUntil                  },
    {"val",     TokenTypeVal                    },
    {"var",     TokenTypeVar                    },
    {"while",   TokenTypeWhile                  },
    {"yield",   TokenTypeYield                  },
};

Operator_t operators[] = {
    {"...",     TokenTypeEllipsis               },
    {"..",      TokenTypeRange                  },
    {".",       TokenTypeDot                    },
    {"===",     TokenTypeIs                     },
    {"==",      TokenTypeEq                     },
    {"=",       TokenTypeAssign                 },
    {"!==",     TokenTypeNotIs                  },
    {"!=",      TokenTypeNotEq                  },
    {"!",       TokenTypeNot                    },
    {"&&",      TokenTypeAnd                    },
    {"&",       TokenTypeBitAnd                 },
    {"||",      TokenTypeOr                     },
    {"|",       TokenTypeBitOr                  },
    {"++",      TokenTypeIncrement              },
    {"+=",      TokenTypeAddAssign              },
    {"+",       TokenTypePlus                   },
    {"--",      TokenTypeDecrement              },
    {"-=",      TokenTypeSubAssign              },
    {"-",       TokenTypeMinus                  },
    {"**",      TokenTypeExponent               },
    {"*=",      TokenTypeMulAssign              },
    {"*",       TokenTypeMultiply               },
    {"/=",      TokenTypeDivAssign              },
    {"/",       TokenTypeDivide                 },
    {"%=",      TokenTypeModAssign              },
    {"%",       TokenTypeModulo                 },
    {"<=",      TokenTypeLessThanOrEqual        },
    {"<",       TokenTypeLessThan               },
    {">=",      TokenTypeGreaterThanOrEqual     },
    {">",       TokenTypeGreaterThan            },
    {"~",       TokenTypeBitNot                 },
    {"(",       TokenTypeOpenParen              },
    {")",       TokenTypeCloseParen             },
    {"{",       TokenTypeOpenCurly              },
    {"}",       TokenTypeCloseCurly             },
    {"[",       TokenTypeOpenSquare             },
    {"]",       TokenTypeCloseSquare            },
    {";",       TokenTypeSemicolon              },
    {":",       TokenTypeColon                  },
    {",",       TokenTypeComma                  },
    {"@",       TokenTypeDog                    },
    {" ",       TokenTypeSpaceTrivia            },
    {"\t",      TokenTypeTabTrivia              },
    {"\n\r",    TokenTypeNewLineTrivia          },
    {"\n",      TokenTypeNewLineTrivia          },
    {"\r",      TokenTypeNewLineTrivia          },
};

Literal_t literals[] = {
    { Tokenize_Number     },
    { Tokenize_String     },
    { Tokenize_Comment    },
    { Tokenize_Identifier },
};

const size_t operators_length = sizeof operators / sizeof *operators;
const size_t keywords_count   = sizeof keywords  / sizeof *keywords;
const size_t literals_count   = sizeof literals  / sizeof *literals;

Token_t *Tokenize   (Tokenizer_t *lexer) {
    size_t size = 9;
    size_t seek = 0;
    Token_t *buf = malloc(sizeof(*buf) * size);

    if (buf == NULL) {
        LEXER_ERROR("Out of memory. Buffer cant be allocated");
        return NULL;
    }

    while(Current(lexer) != '\0') {
        Token_t token;

        if (Tokenize_Keyword(lexer, &token)) {
            goto append;
        }
        if (Tokenize_Literal(lexer, &token)) {
            goto append;
        }
        if (Tokenize_Operator(lexer, &token)) {
            goto append;
        }

        LEXER_DEBUG("Unexpected token '%c'", Current(lexer));
        token.type = TokenTypeERR;
        token.start = lexer->seek;
        token.end   = lexer->seek;
        Advance(lexer);

        append: {
            while (seek >= size) {
                void *tmp = realloc(buf, sizeof(*buf) * (size = size * 2 + 1));
                if (tmp == NULL) {
                    LEXER_ERROR("Out of memory");
                    free(buf);
                    return NULL;
                }
                buf = tmp;
            }
            buf[seek++] = token;
        }
    }

    Token_t *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        LEXER_ERROR("Out of memory");
        free(buf);
        return NULL;
    }
    for (size_t i = 0; i < seek; i++) {
        ptr[i] = buf[i];
    }
    free(buf);

    ptr[seek].type = TokenTypeEOF;
    return ptr;
}

int Tokenize_Keyword    (Tokenizer_t *lexer, Token_t *token) {
    if (isalpha(Current(lexer)) == 0) { return 0; }

    for (int i = 0; i < keywords_count; i++) {
        const struct Keyword keyword = keywords[i];
        const size_t size = strlen(keyword.value);

        int seek = 0;
        while (seek < size + 1) {
            if (seek == size) {
                if (isalpha(Current(lexer))) {
                    break;
                }
                if (isdigit(Current(lexer))) {
                    break;
                }
                if (Current(lexer) == '_') {
                    break;
                }

                LEXER_DEBUG("KEYWORD = {%s}", keyword.value);
                token->type  = keyword.type;
                token->start = lexer->seek - size;
                token->end   = lexer->seek - 1;
                return 1;
            }
            if (Current(lexer) != keyword.value[seek]) {
                break;
            }
            Advance(lexer); seek++;
        }
        while (seek > 0) {
            Retreat(lexer); seek--;
        }
    }
    return 0;
}
int Tokenize_Operator   (Tokenizer_t *lexer, Token_t *token) {
    if (isalnum(Current(lexer))) { return 0; }

    for (int i = 0; i < operators_length; i++) {
        const struct Operator operator = operators[i];
        const size_t size = strlen(operator.value);

        int seek = 0;
        while (seek < size + 1) {
            if (seek == size) {
                LEXER_DEBUG("OPERATOR = {%s}", operator.value);
                token->type  = operator.type;
                token->start = lexer->seek - size;
                token->end   = lexer->seek - 1;
                return 1;
            }
            if (Current(lexer) != operator.value[seek]) {
                break;
            }
            Advance(lexer); seek++;
        }
        while (seek > 0) {
            Retreat(lexer); seek--;
        }
    }

    return 0;
}
int Tokenize_Literal    (Tokenizer_t *lexer, Token_t *token) {
    for (int i = 0; i < literals_count; i++) {
        if (literals[i].handler(lexer, token)) {
            return 1;
        }
    }
    return 0;
}
int Tokenize_Number     (Tokenizer_t *lexer, Token_t *token) {
    if (!isdigit(Current(lexer))) {
        return 0;
    }

    double value = 0;
    while (isdigit(Current(lexer))) {
        value = value * 10 + (Current(lexer) - '0');
        Advance(lexer);
    }
    if (Current(lexer) == '.') {
        Advance(lexer); //skip .
        if (isdigit(Current(lexer))) {
            double fraction = 1;
            while (isdigit(Current(lexer))) {
                fraction /= 10;
                value += (Current(lexer) - '0') * fraction;
                Advance(lexer);
            }
        }
        else {
            Retreat(lexer);
            if (!value) {
                return 0;
            }
        }
    }

    double *data = malloc(sizeof *data);
    *data = value;

    LEXER_DEBUG("NUMBER = {%g}", value);
    token->type = TokenTypeDecimal;
    token->data.decimal = value;
    return 1;
}
int Tokenize_String     (Tokenizer_t *lexer, Token_t *token) {
    if (Current(lexer) != '"' && Current(lexer) != '\'') {
        return 0;
    }

    size_t size = 15;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        LEXER_ERROR("Out of memory");
        return 0;
    }

    const char open = Current(lexer);
    Advance(lexer); // skip open
    while (Current(lexer) != open) {
        if (Current(lexer) == '\0') {
            buf[seek] = '\0';
            LEXER_ERROR("String \'%s\' await end with '%c'", buf, open);
            free(buf);
            return 0;
        }
        if (Current(lexer) == '\\') {
            Advance(lexer);
            switch (Current(lexer)) {
                case '\\': {
                    Advance(lexer);
                    buf[seek++] = '\\';
                    continue;
                }
                case '\'': {
                    Advance(lexer);
                    buf[seek++] = '\'';
                    continue;
                }
                case '\"': {
                    Advance(lexer);
                    buf[seek++] = '\"';
                    continue;
                }
                case 'n': {
                    Advance(lexer);
                    buf[seek++] = '\n';
                    continue;
                }
                case 'r': {
                    Advance(lexer);
                    buf[seek++] = '\r';
                    continue;
                }
                case 't': {
                    Advance(lexer);
                    buf[seek++] = '\t';
                    continue;
                }
                case 'b': {
                    Advance(lexer);
                    buf[seek++] = '\b';
                    continue;
                }
                case 'f': {
                    Advance(lexer);
                    buf[seek++] = '\f';
                    continue;
                }
                case 'a': {
                    Advance(lexer);
                    buf[seek++] = '\a';
                    continue;
                }
                case 'v': {
                    Advance(lexer);
                    buf[seek++] = '\v';
                    continue;
                }
                case '0': {
                    Advance(lexer);
                    buf[seek++] = '\0';
                    continue;
                }
                default: {
                    LEXER_ERROR("Invalid escape character '\\%c'", current(lexer));
                    free(buf);
                    return 0;
                }
            }
        }
        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(tmp);
                free(buf);
                LEXER_ERROR("Out of memory");
                return 0;
            }
            buf = tmp;
        }
        buf[seek++] = Current(lexer);
        Advance(lexer);
    }
    Advance(lexer); // skip close

    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(ptr);
        free(buf);
        LEXER_ERROR("Out of memory");
        return 0;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    LEXER_DEBUG("STRING = {%s}", ptr);
    token->type = TokenTypeString;
    token->data.string = ptr;
    return 1;
}
int Tokenize_Comment    (Tokenizer_t *lexer, Token_t *token) {
    if (Current(lexer) != '#') {
        return 0;
    }

    size_t size = 15;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        return 0;
    }

    Advance(lexer);//skip '#'
    while (Current(lexer) != '\n' && Current(lexer) != '\r' && Current(lexer) != '\0') {
        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(tmp);
                free(buf);
                LEXER_ERROR("Out of memory");
                return 0;
            }
            buf = tmp;
        }
        buf[seek++] = Current(lexer);
        Advance(lexer);
    }
    Advance(lexer); //'\n'
    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(ptr);
        free(buf);
        LEXER_ERROR("Out of memory");
        return 0;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    LEXER_DEBUG("COMMENT = {%s}", ptr);
    token->type = TokenTypeComment;
    token->data.comment = ptr;
    return 1;
}
int Tokenize_Identifier (Tokenizer_t *lexer, Token_t *token) {
    if (Current(lexer) != '_' && !isalpha(Current(lexer))) {
        return 0;
    }

    size_t size = 7;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    if (buf == NULL) {
        return 0;
    }

    buf[seek++] = Current(lexer);
    while (1) {
        Advance(lexer);
        if (Current(lexer) != '_' && !isalnum(Current(lexer))) {
            break;
        }

        if (seek >= size) {
            void *tmp = realloc(buf, sizeof *buf * (size *= 2));
            if (tmp == NULL) {
                free(buf);
                LEXER_ERROR("Out of memory");
                return 0;
            }
            buf = tmp;
        }

        buf[seek++] = Current(lexer);
    }

    char *ptr = malloc(sizeof *ptr * (seek + 1));
    if (ptr == NULL) {
        free(buf);
        LEXER_ERROR("Out of memory");
        return 0;
    }

    strncpy(ptr, buf, seek);
    ptr[seek] = '\0';
    free(buf);

    LEXER_DEBUG("IDENTIFIER = {%s}", ptr);

    token->type = TokenTypeIdentifier;
    token->data.identifier = ptr;

    return 1;
}

char Current(const Tokenizer_t *lexer) {
    return lexer->chars[lexer->seek];
}
void Advance(Tokenizer_t *lexer) {
    lexer->seek++;
}
void Retreat(Tokenizer_t *lexer) {
    lexer->seek--;
}