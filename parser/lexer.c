#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammatic.h"
#include "lexer.h"

/// Return current symbol of lexer`s text
char current_char(const lexer_t *lexer);
/// Advance current seek of lexer`s text
void advance_char(lexer_t *lexer);
/// Retreat current seek of lexer`s text
void retreat_char(lexer_t *lexer);

void main() {
    lexer_t lexer = {
            .text =
                "var a = 100 + 1.1 + 1.. .1 + \n"
                "func _ () > number {\n}\n"
                "val x = \"abc\"",
            .seek = 0,
            .column = 0,
            .row = 0,
    };

    token_t *token = NULL;

    while (1) {
        token = lex(&lexer);
        if (token == NULL || token->type == TOKEN_EOF) {
            break;
        }
        printf("Token: %d; %s\n", token->type, (char *)token->value);
        free(token->value);
        free(token);
    }
}

token_t *lex_keyword(lexer_t *lexer);
token_t *lex_identifier(lexer_t *lexer);
token_t *lex_string(lexer_t *lexer);
token_t *lex_number(lexer_t *lexer);
token_t *lex_comment(lexer_t *lexer); //not implemented now. I WANT SLEEP!!!
token_t *lex_operator(lexer_t *lexer);

token_t *lex(lexer_t *lexer) {
    const size_t size = 5;
    token_t *(*lex_function[size])(lexer_t *lexer);

    lex_function[size - 5] = lex_keyword;
    lex_function[size - 4] = lex_identifier;
    lex_function[size - 3] = lex_number;
    lex_function[size - 2] = lex_string;
    lex_function[size - 1] = lex_operator;

    for (int i = 0; i < size; i++) {
        token_t *token = lex_function[i](lexer);
        if (token != NULL) {
            return token;
        }
    }

    LEXER_PANIC(lexer, "Invalid token '%c'", current_char(lexer));
    advance_char(lexer);

    return NULL;
}
token_t *lex_keyword(lexer_t *lexer) {
    const size_t size = 7; // is max length keyword

    size_t seek = 0;
    char buf[size];

    while (seek < size) {
        if (isalpha(current_char(lexer))) {
            buf[seek++] = current_char(lexer);
            advance_char(lexer);
            continue;
        }
        if (isdigit(current_char(lexer))) {
            goto def;
        }
        if (current_char(lexer) == '_') {
            goto def;
        }
        break;
    }

    switch (seek) {
        case 2: {
            if (buf[0] == 'i' && buf[1] == 'f') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_IF;
                token->value = strdup("if");
                return token;
            }
            if (buf[0] == 'i' && buf[1] == 's') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_IS;
                token->value = strdup("is");
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'q') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_EQ;
                token->value = strdup("eq");
                return token;
            }
            if (buf[0] == 'o' && buf[1] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_OR;
                token->value = strdup("or");
                return token;
            }

            goto def;
        }
        case 3: {
            if (buf[0] == 'v' && buf[1] == 'a' && buf[2] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_VAR;
                token->value = strdup("var");
                return token;
            }
            if (buf[0] == 'v' && buf[1] == 'a' && buf[2] == 'l') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_VAL;
                token->value = strdup("val");
                return token;
            }
            if (buf[0] == 'a' && buf[1] == 'n' && buf[2] == 'd') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_AND;
                token->value = strdup("and");
                return token;
            }
            if (buf[0] == 'n' && buf[1] == 'o' && buf[2] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_NOT;
                token->value = strdup("not");
                return token;
            }
            if (buf[0] == 'f' && buf[1] == 'o' && buf[2] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FOR;
                token->value = strdup("for");
                return token;
            }
            if (buf[0] == 't' && buf[1] == 'r' && buf[2] == 'y') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_TRY;
                token->value = strdup("try");
                return token;
            }

            goto def;
        }
        case 4: {
            if (buf[0] == 't' && buf[1] == 'y' && buf[2] == 'p' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_TYPE;
                token->value = strdup("type");
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'n' && buf[2] == 'u' && buf[3] == 'm') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ENUM;
                token->value = strdup("enum");
                return token;
            }
            if (buf[0] == 'f' && buf[1] == 'u' && buf[2] == 'n' && buf[3] == 'c') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FUNC;
                token->value = strdup("func");
                return token;
            }
            if (buf[0] == 'c' && buf[1] == 'a' && buf[2] == 's' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_CASE;
                token->value = strdup("case");
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'l' && buf[2] == 's' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ELSE;
                token->value = strdup("else");
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'l' && buf[2] == 'i' && buf[3] == 'f') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ELIF;
                token->value = strdup("elif");
                return token;
            }
            if (buf[0] == 'n' && buf[1] == 'e' && buf[2] == 'x' && buf[3] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_NEXT;
                token->value = strdup("next");
                return token;
            }
            if (buf[0] == 'b' && buf[1] == 'a' && buf[2] == 'c' && buf[3] == 'k') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_BACK;
                token->value = strdup("back");
                return token;
            }
            if (buf[0] == 's' && buf[1] == 't' && buf[2] == 'o' && buf[3] == 'p') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_STOP;
                token->value = strdup("stop");
                return token;
            }

            goto def;
        }
        case 5: {
            if (buf[0] == 's' && buf[1] == 'p' && buf[2] == 'a' && buf[3] == 's' && buf[4] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_SPACE;
                token->value = strdup("space");
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'j' && buf[2] == 'e' && buf[3] == 'c' && buf[4] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_EJECT;
                token->value = strdup("eject");
                return token;
            }
            if (buf[0] == 'y' && buf[1] == 'i' && buf[2] == 'e' && buf[3] == 'l' && buf[4] == 'd') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_YIELD;
                token->value = strdup("yield");
                return token;
            }
            if (buf[0] == 'w' && buf[1] == 'h' && buf[2] == 'i' && buf[3] == 'l' && buf[4] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_WHILE;
                token->value = strdup("while");
                return token;
            }
            if (buf[0] == 'u' && buf[1] == 'n' && buf[2] == 't' && buf[3] == 'i' && buf[4] == 'l') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_UNTIL;
                token->value = strdup("until");
                return token;
            }
            if (buf[0] == 'c' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'c' && buf[4] == 'h') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_CATCH;
                token->value = strdup("catch");
                return token;
            }
            if (buf[0] == 't' && buf[1] == 'h' && buf[2] == 'r' && buf[3] == 'o' && buf[4] == 'w') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_THROW;
                token->value = strdup("throw");
                return token;
            }

            goto def;
        }
        case 6: {
            if (buf[0] == 'r' && buf[1] == 'e' && buf[2] == 't' && buf[3] == 'u' && buf[4] == 'r' && buf[5] == 'n') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_RETURN;
                token->value = strdup("return");
                return token;
            }
            if (buf[0] == 's' && buf[1] == 'w' && buf[2] == 'i' && buf[3] == 't' && buf[4] == 'c' && buf[5] == 'h') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_SWITCH;
                token->value = strdup("switch");
                return token;
            }
            if (buf[0] == 'r' && buf[1] == 'e' && buf[2] == 'p' && buf[3] == 'e' && buf[4] == 'a' && buf[5] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_REPEAT;
                token->value = strdup("repeat");
                return token;
            }
        }
        case 7: {
            if (buf[0] == 'f' && buf[1] == 'i' && buf[2] == 'n' && buf[3] == 'a' && buf[4] == 'l' && buf[5] == 'l' && buf[6] == 'y') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FINALLY;
                token->value = strdup("finally");
                return token;
            }

            goto def;
        }
        default: { goto def; }
    }

    def: {
        while (seek > 0) {
            retreat_char(lexer);
            seek--;
        }
        return NULL;
    }
}
token_t *lex_identifier(lexer_t *lexer) {
    if (current_char(lexer) != '_' && !isalpha(current_char(lexer))) {
        return NULL;
    }

    size_t size = 7;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    buf[seek++] = current_char(lexer);
    while (1) {
        advance_char(lexer);
        if (current_char(lexer) != '_' &&
            !isalpha(current_char(lexer)) &&
            !isdigit(current_char(lexer))) {
            break;
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

        buf[seek++] = current_char(lexer);
    }

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
    token->type = TOKEN_IDENTIFIER;
    token->value = ptr;
    return token;
}
token_t *lex_number(lexer_t *lexer) {
    if (current_char(lexer) != '.' &&  !isdigit(current_char(lexer))) {
        return NULL;
    }

    size_t size = 6;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    while (isdigit(current_char(lexer))) {
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
        buf[seek++] = current_char(lexer);
        advance_char(lexer);
    }
    if (current_char(lexer) == '.') {
        buf[seek++] = current_char(lexer);
        advance_char(lexer);

        if (isdigit(current_char(lexer))) {
            while (isdigit(current_char(lexer))) {
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
                buf[seek++] = current_char(lexer);
                advance_char(lexer);
            }
        }
        else {
            if (seek == 1) {
                free(buf);
                return NULL;
            }

            retreat_char(lexer);
            seek--;
        }
    }

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
    token->type = TOKEN_NUMBER;
    token->value = ptr;
    return token;
}
token_t *lex_string(lexer_t *lexer) {
    if (current_char(lexer) != '"' && current_char(lexer) != '\'') {
        return NULL;
    }

    size_t size = 15;
    size_t seek = 0;
    char *buf = malloc(sizeof *buf * size);

    const char open = current_char(lexer);
    advance_char(lexer); //skip open

    while (current_char(lexer) != open) {
        if (current_char(lexer) == '\0') {
            buf[seek] = '\0';
            LEXER_PANIC(lexer, "String \'%s\' await end with '%c'", buf, open);
            free(buf);
            return NULL;
        }
        if (current_char(lexer) == '\\') {
            advance_char(lexer);
            switch (current_char(lexer)) {
                case '\\': {
                    advance_char(lexer);
                    buf[seek++] = '\\';
                    continue;
                }
                case '\'': {
                    advance_char(lexer);
                    buf[seek++] = '\'';
                    continue;
                }
                case '\"': {
                    advance_char(lexer);
                    buf[seek++] = '\"';
                    continue;
                }
                case 'n': {
                    advance_char(lexer);
                    buf[seek++] = '\n';
                    continue;
                }
                case 'r': {
                    advance_char(lexer);
                    buf[seek++] = '\r';
                    continue;
                }
                case 't': {
                    advance_char(lexer);
                    buf[seek++] = '\t';
                    continue;
                }
                case 'b': {
                    advance_char(lexer);
                    buf[seek++] = '\b';
                    continue;
                }
                case 'f': {
                    advance_char(lexer);
                    buf[seek++] = '\f';
                    continue;
                }
                case 'a': {
                    advance_char(lexer);
                    buf[seek++] = '\a';
                    continue;
                }
                case 'v': {
                    advance_char(lexer);
                    buf[seek++] = '\v';
                    continue;
                }
                case '0': {
                    advance_char(lexer);
                    buf[seek++] = '\0';
                    continue;
                }
                default: {
                    LEXER_PANIC(lexer, "Invalid escape character '\\%c'", current_char(lexer));
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
        buf[seek++] = current_char(lexer);
        advance_char(lexer);
    }
    advance_char(lexer); //skip close
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
    token->type = TOKEN_IDENTIFIER;
    token->value = ptr;
    return token;
}

token_t *lex_operator(lexer_t *lexer) {
    switch (current_char(lexer)) {
        case '+': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '+': {
                    advance_char(lexer);
                    token->type = TOKEN_INCREMENT;
                    token->value = strdup("++");
                    return token;
                }
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_PLUS_ASSIGN;
                    token->value = strdup("+=");
                    return token;
                }
                default: {
                    token->type = TOKEN_PLUS;
                    token->value = strdup("+");
                    return token;
                }
            }
        }
        case '-': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '-': {
                    advance_char(lexer);
                    token->type = TOKEN_DECREMENT;
                    token->value = strdup("--");
                    return token;
                }
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_MINUS_ASSIGN;
                    token->value = strdup("-=");
                    return token;
                }
                default: {
                    token->type = TOKEN_MINUS;
                    token->value = strdup("-");
                    return token;
                }
            }
        }
        case '*': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_MULTIPLY_ASSIGN;
                    token->value = strdup("*=");
                    return token;
                }
                case '*': {
                    advance_char(lexer);
                    token->type = TOKEN_EXPONENT;
                    token->value = strdup("**");
                    return token;
                }
                default: {
                    token->type = TOKEN_MULTIPLY;
                    token->value = strdup("*");
                    return token;
                }
            }
        }
        case '/': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_DIVIDE_ASSIGN;
                    token->value = strdup("/=");
                    return token;
                }
                default: {
                    token->type = TOKEN_DIVIDE;
                    token->value = strdup("/");
                    return token;
                }
            }
        }
        case '%': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_MODULUS_ASSIGN;
                    token->value = strdup("%=");
                    return token;
                }
                default: {
                    token->type = TOKEN_MODULUS;
                    token->value = strdup("%");
                    return token;
                }
            }
        }
        case '.': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '.': {
                    advance_char(lexer);

                    switch (current_char(lexer)) {
                        case '.': {
                            advance_char(lexer);
                            token->type = TOKEN_ELLIPSIS;
                            token->value = strdup("...");
                            return token;
                        }
                        default: {
                            token->type = TOKEN_RANGE;
                            token->value = strdup("..");
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_DOT;
                    token->value = strdup(".");
                    return token;
                }
            }
        }
        case '(': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_PARENTHESIS_LEFT;
            token->value = strdup("(");
            return token;
        }
        case ')': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_PARENTHESIS_RIGHT;
            token->value = strdup(")");
            return token;
        }
        case '[': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SQUARE_BRACKET_LEFT;
            token->value = strdup("[");
            return token;
        }
        case ']': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SQUARE_BRACKET_RIGHT;
            token->value = strdup("]");
            return token;
        }
        case '{': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_CURLY_BRACKET_LEFT;
            token->value = strdup("{");
            return token;
        }
        case '}': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_CURLY_BRACKET_RIGHT;
            token->value = strdup("}");
            return token;
        }
        case '<': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_LESS_OR_EQUAL;
                    token->value = strdup("<=");
                    return token;
                }
                default: {
                    token->type = TOKEN_LESS; // == TOKEN_ANGEL_BRACKET_LEFT
                    token->value = strdup("<");
                    return token;
                }
            }
        }
        case '>': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_GREATER_OR_EQUAL;
                    token->value = strdup(">=");
                    return token;
                }
                default: {
                    token->type = TOKEN_GREATER; // == TOKEN_ANGEL_BRACKET_RIGHT
                    token->value = strdup(">");
                    return token;
                }
            }
        }
        case ':': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_COLON;
            token->value = strdup(":");
            return token;
        }
        case ';': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SEMICOLON;
            token->value = strdup(":");
            return token;
        }
        case '|': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_OR;
            token->value = strdup(":");
            return token;
        }
        case '&': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_AND;
            token->value = strdup(":");
            return token;
        }
        case '!': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    switch (current_char(lexer)) {
                        case '=': {
                            advance_char(lexer);
                            token->type = TOKEN_IS_NOT;
                            token->value = strdup("!==");
                            return token;
                        }
                        default: {
                            token->type = TOKEN_EQ_NOT;
                            token->value = strdup("!=");
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_NOT;
                    token->value = strdup("!");
                    return token;
                }
            }
        }
        case '=': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    switch (current_char(lexer)) {
                        case '=': {
                            advance_char(lexer);
                            token->type = TOKEN_IS;
                            token->value = strdup("===");
                            return token;
                        }
                        default: {
                            token->type = TOKEN_EQ;
                            token->value = strdup("==");
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_ASSIGN;
                    token->value = strdup("=");
                    return token;
                }
            }
        }
        case ' ': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SPACE_CHAR;
            token->value = strdup("' '");
            return token;
        }
        case '\t': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_TAB_CHAR;
            token->value = strdup("\\t");
            return token;
        }
        case '\n': {
            lexer->seek++;
            lexer->column = 0;
            lexer->row++;

            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_NEWLINE_CHAR;
            token->value = strdup("\\n");
            return token;
        }
        case '\r': {
            lexer->seek++;
            lexer->column = 0;
            lexer->row++;

            if (lexer->text[lexer->seek] == '\n') {
                lexer->seek++;
            }

            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_NEWLINE_CHAR;
            token->value = strdup("\\n");
            return token;
        }
        case '\0': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_EOF;
            token->value = strdup("'\\0'");
            return token;
        }
        default: {
            return NULL;
        }
    }
}

char current_char(const lexer_t *lexer) {
    return lexer->text[lexer->seek];
}
void advance_char(lexer_t *lexer) {
    lexer->column++;
    lexer->seek++;
}
void retreat_char(lexer_t *lexer) {
    lexer->column--;
    lexer->seek--;
}