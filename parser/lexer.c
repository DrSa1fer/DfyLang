#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../grammar.h"
#include "lexer.h"

/// Return current symbol of lexer`s text
char current_char(const lexer_t *lexer);
/// Advance current seek of lexer`s text
void advance_char(lexer_t *lexer);
/// Retreat current seek of lexer`s text
void retreat_char(lexer_t *lexer);

// void main() {
//     char **text = malloc(sizeof *text);
//     *text = strdup(
//                 "var a = 100 + 1.1 + 1.. .1 + \n"
//                 "func _ () > number {\n}\n"
//                 "val x = \"abc\" hello");
//     lexer_t lexer = {
//             .chars = text,
//             .seek = 0,
//             .column = 0,
//             .row = 0,
//     };
//
//     token_t *token = NULL;
//
//     while (1) {
//         token = lex(&lexer);
//         if (token == NULL || token->type == TOKEN_EOF) {
//             break;
//         }
//         printf("Token: %d; %s\n", token->type, (char *)token->value);
//         free(token->value);
//         free(token);
//     }
// }

token_t *lex_keyword(lexer_t *lexer);
token_t *lex_identifier(lexer_t *lexer);
token_t *lex_string(lexer_t *lexer);
token_t *lex_number(lexer_t *lexer);
token_t *lex_comment(lexer_t *lexer); //not implemented now. I WANT SLEEP!!!
token_t *lex_operator(lexer_t *lexer);

token_t *lex(lexer_t *lexer) {
    const size_t lex_function_size = 6;
    token_t *(*lex_function[lex_function_size])(lexer_t *lexer);

    lex_function[0] = lex_number;
    lex_function[1] = lex_comment;
    lex_function[2] = lex_string;
    lex_function[3] = lex_keyword;
    lex_function[4] = lex_operator;
    lex_function[5] = lex_identifier;

    size_t size = 20;
    token_t *tokens = malloc(sizeof *tokens * size);

    for(int i = 0;; i++) {
        const token_t *token = NULL;
        for (int j = 0; j < lex_function_size; j++) {
            token = lex_function[j](lexer);
            if (token != NULL) {
                break;
            }
        }
        if (token == NULL) {
            LEXER_PANIC(lexer, "Invalid token '%c'", current_char(lexer));
            advance_char(lexer);
            free(tokens);
            return NULL;
        }
        if (token->type == TOKEN_ERR) {
            LEXER_PANIC(lexer, "ERR token");
            advance_char(lexer);
            free(tokens);
            return NULL;
        }
        if (i >= size) {
            void *tmp = realloc(tokens, sizeof *tokens * (size = size * 3 / 2));
            if (tmp == NULL) {
                LEXER_PANIC(lexer, "Out of memory");
                return NULL;
            }
            tokens = tmp;
        }
        tokens[i] = *token;
        // printf("type: %d\n", tokens[i].type);
        if (token->type == TOKEN_EOF) {
            return tokens;
        }
    }
}
token_t *lex_keyword(lexer_t *lexer) {
    if (!isalpha(current_char(lexer))) {
        return NULL;
    }
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
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'i' && buf[1] == 's') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_IS;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'q') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_EQ;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'o' && buf[1] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_OR;
                token->value = NULL;
                return token;
            }

            goto def;
        }
        case 3: {
            if (buf[0] == 'v' && buf[1] == 'a' && buf[2] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_VAR;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'v' && buf[1] == 'a' && buf[2] == 'l') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_VAL;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'a' && buf[1] == 'n' && buf[2] == 'd') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_AND;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'n' && buf[1] == 'o' && buf[2] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_NOT;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'f' && buf[1] == 'o' && buf[2] == 'r') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FOR;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 't' && buf[1] == 'r' && buf[2] == 'y') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_TRY;
                token->value = NULL;
                return token;
            }

            goto def;
        }
        case 4: {
            if (buf[0] == 't' && buf[1] == 'y' && buf[2] == 'p' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_TYPE;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'n' && buf[2] == 'u' && buf[3] == 'm') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ENUM;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'f' && buf[1] == 'u' && buf[2] == 'n' && buf[3] == 'c') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FUNC;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'c' && buf[1] == 'a' && buf[2] == 's' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_CASE;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'l' && buf[2] == 's' && buf[3] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ELSE;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'l' && buf[2] == 'i' && buf[3] == 'f') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_ELIF;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'n' && buf[1] == 'e' && buf[2] == 'x' && buf[3] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_NEXT;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'b' && buf[1] == 'a' && buf[2] == 'c' && buf[3] == 'k') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_BACK;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 's' && buf[1] == 't' && buf[2] == 'o' && buf[3] == 'p') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_STOP;
                token->value = NULL;
                return token;
            }

            goto def;
        }
        case 5: {
            if (buf[0] == 's' && buf[1] == 'p' && buf[2] == 'a' && buf[3] == 's' && buf[4] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_SPACE;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'e' && buf[1] == 'j' && buf[2] == 'e' && buf[3] == 'c' && buf[4] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_EJECT;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'y' && buf[1] == 'i' && buf[2] == 'e' && buf[3] == 'l' && buf[4] == 'd') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_YIELD;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'w' && buf[1] == 'h' && buf[2] == 'i' && buf[3] == 'l' && buf[4] == 'e') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_WHILE;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'u' && buf[1] == 'n' && buf[2] == 't' && buf[3] == 'i' && buf[4] == 'l') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_UNTIL;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'c' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'c' && buf[4] == 'h') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_CATCH;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 't' && buf[1] == 'h' && buf[2] == 'r' && buf[3] == 'o' && buf[4] == 'w') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_THROW;
                token->value = NULL;
                return token;
            }

            goto def;
        }
        case 6: {
            if (buf[0] == 'r' && buf[1] == 'e' && buf[2] == 't' && buf[3] == 'u' && buf[4] == 'r' && buf[5] == 'n') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_RETURN;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 's' && buf[1] == 'w' && buf[2] == 'i' && buf[3] == 't' && buf[4] == 'c' && buf[5] == 'h') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_SWITCH;
                token->value = NULL;
                return token;
            }
            if (buf[0] == 'r' && buf[1] == 'e' && buf[2] == 'p' && buf[3] == 'e' && buf[4] == 'a' && buf[5] == 't') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_REPEAT;
                token->value = NULL;
                return token;
            }
        }
        case 7: {
            if (buf[0] == 'f' && buf[1] == 'i' && buf[2] == 'n' && buf[3] == 'a' && buf[4] == 'l' && buf[5] == 'l' && buf[6] == 'y') {
                token_t *token = malloc(sizeof *token);
                token->type = TOKEN_FINALLY;
                token->value = NULL;
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
        if (current_char(lexer) != '_' && !isalnum(current_char(lexer))) {
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
token_t *lex_number(lexer_t *lexer) {
    if (current_char(lexer) != '.' &&  !isdigit(current_char(lexer))) {
        return NULL;
    }

    char is_init = 0;
    double value = 0;
    while (isdigit(current_char(lexer))) {
        value = value * 10 + (current_char(lexer) - '0');
        advance_char(lexer);
        is_init = 1;
    }
    if (current_char(lexer) == '.') {
        advance_char(lexer); //skip .
        if (isdigit(current_char(lexer))) {
            double fraction = 1;
            while (isdigit(current_char(lexer))) {
                fraction /= 10;
                value += (current_char(lexer) - '0') * fraction;
                advance_char(lexer);
            }
        }
        else {
            retreat_char(lexer);
            if (!is_init) {
                return NULL;
            }
        }
    }

    token_t *token = malloc(sizeof *token);
    double *ptr = malloc(sizeof *ptr);
    *ptr = value;
    token->type = TOKEN_NUMBER;
    token->value = ptr;
    return token;
}
token_t *lex_comment(lexer_t *lexer) {
    return NULL;
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
                    token->value = NULL;
                    return token;
                }
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_ASSIGN_PLUS;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_PLUS;
                    token->value = NULL;
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
                    token->value = NULL;
                    return token;
                }
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_ASSIGN_MINUS;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_MINUS;
                    token->value = NULL;
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
                    token->type = TOKEN_ASSIGN_MULTIPLY;
                    token->value = NULL;
                    return token;
                }
                case '*': {
                    advance_char(lexer);
                    token->type = TOKEN_POWER;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_MULTIPLY;
                    token->value = NULL;
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
                    token->type = TOKEN_ASSIGN_DIVIDE;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_DIVIDE;
                    token->value = NULL;
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
                    token->type = TOKEN_ASSIGN_MODULUS;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_MODULUS;
                    token->value = NULL;
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
                            token->value = NULL;
                            return token;
                        }
                        default: {
                            token->type = TOKEN_RANGE;
                            token->value = NULL;
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_DOT;
                    token->value = NULL;
                    return token;
                }
            }
        }
        case '(': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_PARENTHESIS_LEFT;
            token->value = NULL;
            return token;
        }
        case ')': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_PARENTHESIS_RIGHT;
            token->value = NULL;
            return token;
        }
        case '[': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SQUARE_BRACKET_LEFT;
            token->value = NULL;
            return token;
        }
        case ']': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SQUARE_BRACKET_RIGHT;
            token->value = NULL;
            return token;
        }
        case '{': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_CURLY_BRACKET_LEFT;
            token->value = NULL;
            return token;
        }
        case '}': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_CURLY_BRACKET_RIGHT;
            token->value = NULL;
            return token;
        }
        case '<': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            switch (current_char(lexer)) {
                case '=': {
                    advance_char(lexer);
                    token->type = TOKEN_LESS_OR_EQUAL;
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_LESS; // == TOKEN_ANGEL_BRACKET_LEFT
                    token->value = NULL;
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
                    token->value = NULL;
                    return token;
                }
                default: {
                    token->type = TOKEN_GREATER; // == TOKEN_ANGEL_BRACKET_RIGHT
                    token->value = NULL;
                    return token;
                }
            }
        }
        case ':': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_COLON;
            token->value = NULL;
            return token;
        }
        case ';': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_SEMICOLON;
            token->value = NULL;
            return token;
        }
        case '|': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_OR;
            token->value = NULL;
            return token;
        }
        case '&': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_AND;
            token->value = NULL;
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
                            token->value = NULL;
                            return token;
                        }
                        default: {
                            token->type = TOKEN_EQ_NOT;
                            token->value = NULL;
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_NOT;
                    token->value = NULL;
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
                            token->value = NULL;
                            return token;
                        }
                        default: {
                            token->type = TOKEN_EQ;
                            token->value = NULL;
                            return token;
                        }
                    }
                }
                default: {
                    token->type = TOKEN_ASSIGN;
                    token->value = NULL;
                    return token;
                }
            }
        }
        case ' ': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_WHITESPACE;
            token->value = NULL;
            return token;
        }
        case '\t': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_TAB;
            token->value = NULL;
            return token;
        }
        case '\n': {
            lexer->seek++;
            lexer->column = 0;
            lexer->row++;

            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_NEWLINE;
            token->value = NULL;
            return token;
        }
        case '\r': {
            //custom advance
            lexer->seek++;
            lexer->column = 0;
            lexer->row++;

            if (current_char(lexer) == '\n') {
                lexer->seek++;
            }

            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_NEWLINE;
            token->value = NULL;
            return token;
        }
        case '\0': {
            advance_char(lexer);
            token_t *token = malloc(sizeof *token);
            token->type = TOKEN_EOF;
            token->value = NULL;
            return token;
        }
        default: {
            return NULL;
        }
    }
}

char current_char(const lexer_t *lexer) {
    return lexer->chars[lexer->seek];
}
void advance_char(lexer_t *lexer) {
    lexer->column++;
    lexer->seek++;
}
void retreat_char(lexer_t *lexer) {
    lexer->column--;
    lexer->seek--;
}