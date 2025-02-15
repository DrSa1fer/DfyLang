#include <stdlib.h>
#include <stdio.h>

#include "../grammar.h"
#include "parser.h"

#include <string.h>

token_t current_token(const parser_t *parser);
void advance_token(parser_t *parser);
void retreat_token(parser_t *parser);
void advance_whitespace(parser_t *parser);
void retreat_whitespace(parser_t *parser);


node_t *parse_primary                               (parser_t *parser);
node_t *parse_subexpr                               (parser_t *parser);
node_t *parse_identifier                            (parser_t *parser);
node_t *parse_number                                (parser_t *parser);
node_t *parse_string                                (parser_t *parser);
node_t *parse_boolean                               (parser_t *parser);
node_t *parse_call_func                             (parser_t *parser);

node_t *parse_postfix                               (parser_t *parser);
node_t *parse_increment                             (parser_t *parser);
node_t *parse_decrement                             (parser_t *parser);

node_t *parse_prefix                                (parser_t *parser);
node_t *parse_increment                             (parser_t *parser);
node_t *parse_decrement                             (parser_t *parser);
node_t *parse_minus                                 (parser_t *parser);
node_t *parse_not                                   (parser_t *parser);

node_t *parse_exponential                           (parser_t *parser);
node_t *parse_power                                 (parser_t *parser);
node_t *parse_root                                  (parser_t *parser);

node_t *parse_multiplicative                        (parser_t *parser);
node_t *parse_multiplication                        (parser_t *parser);
node_t *parse_division                              (parser_t *parser);
node_t *parse_modulo                                (parser_t *parser);

node_t *parse_additive                              (parser_t *parser);
node_t *parse_addition                              (parser_t *parser);
node_t *parse_subtraction                           (parser_t *parser);

node_t *parse_relational                            (parser_t *parser);
node_t *parse_less                                  (parser_t *parser);
node_t *parse_greater                               (parser_t *parser);
node_t *parse_less_or_equal                         (parser_t *parser);
node_t *parse_greater_or_equal                      (parser_t *parser);

node_t *parse_equality                              (parser_t *parser);
node_t *parse_eq                                    (parser_t *parser);
node_t *parse_is                                    (parser_t *parser);
node_t *parse_eq_not                                (parser_t *parser);
node_t *parse_is_not                                (parser_t *parser);

node_t *parse_or                                    (parser_t *parser);
node_t *parse_and                                   (parser_t *parser);

node_t *parse_statement                             (parser_t *parser);
node_t *parse_compound                              (parser_t *parser);
node_t *parse_switch                                (parser_t *parser);
    node_t *parse_case                              (parser_t *parser);

node_t *parse_if                                    (parser_t *parser);
    node_t *parse_elif                              (parser_t *parser);
    node_t *parse_else                              (parser_t *parser);

node_t *parse_while                                 (parser_t *parser);
node_t *parse_until                                 (parser_t *parser);
node_t *parse_for                                   (parser_t *parser);
    node_t *parse_repeat                            (parser_t *parser);
    node_t *parse_next                              (parser_t *parser);
    node_t *parse_back                              (parser_t *parser);
    node_t *parse_stop                              (parser_t *parser);
    node_t *parse_return                            (parser_t *parser);
    node_t *parse_eject                             (parser_t *parser);
    node_t *parse_yield                             (parser_t *parser);

node_t *parse_try                                   (parser_t *parser);
    node_t *parse_catch                             (parser_t *parser);
    node_t *parse_finally                           (parser_t *parser);

node_t *parse_declaration                           (parser_t *parser);
node_t *parse_var                                   (parser_t *parser);
node_t *parse_val                                   (parser_t *parser);
node_t *parse_type                                  (parser_t *parser);
node_t *parse_enum                                  (parser_t *parser);
node_t *parse_space                                 (parser_t *parser);
node_t *parse_func                                  (parser_t *parser);

node_t *parse_assignment                            (parser_t *parser);
node_t *parse_assign                                (parser_t *parser);
node_t *parse_assign_plus                           (parser_t *parser);
node_t *parse_assign_minus                          (parser_t *parser);
node_t *parse_assign_multiply                       (parser_t *parser);
node_t *parse_assign_divide                         (parser_t *parser);
node_t *parse_assign_modulus                        (parser_t *parser);

//
int main(void) {
    parser_t *parser = malloc(sizeof *parser);
    lexer_t *lexer = malloc(sizeof *lexer);
    lexer->chars = strdup("var     i   :   ;10=0 = .1 = 11 = 1 = 1 *= 0+=10%");
    lexer->column = 0;
    lexer->row = 0;
    lexer->seek = 0;

    parser->lexer = lexer;
    parser->seek = 0;

    parse(parser);


    free(lexer->chars);
    free(lexer);
    free(parser->tokens);
    free(parser);
}
// node_t *parse_sub(parser_t *parser) {
//     if (current_token(parser).type != TOKEN_PARENTHESIS_LEFT) {
//         return NULL;
//     }
//     advance_token(parser); // skip (
//
//     node_t *node = parse(parser);
//     if (current_token(parser).type != TOKEN_PARENTHESIS_RIGHT) {
//         PARSER_PANIC(parser, "Expected ')' after left parentheses");
//         return NULL;
//     }
//     advance_token(parser); // skip ) :3
//
//     return node;
// }

node_t *parse_primary(parser_t *parser) {
    node_t *node = NULL;
    if ((node = parse_identifier(parser))           != NULL) { return node; }
    if ((node = parse_boolean(parser))              != NULL) { return node; }
    if ((node = parse_number(parser))               != NULL) { return node; }
    if ((node = parse_string(parser))               != NULL) { return node; }
    return NULL;parse_s
}
node_t *parse_postfix(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_increment(parser))            != NULL) { return node; }
    // if ((node = parse_decrement(parser))            != NULL) { return node; }
    return parse_primary(parser);
}
node_t *parse_prefix(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_increment(parser))            != NULL) { return node; }
    // if ((node = parse_decrement(parser))            != NULL) { return node; }
    // if ((node = parse_not(parser))                  != NULL) { return node; }
    // if ((node = parse_minus(parser))                != NULL) { return node; }
    return parse_postfix(parser);
}
node_t *parse_exponential(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_power(parser))                != NULL) { return node; }
    // if ((node = parse_root(parser))                 != NULL) { return node; }
    return parse_postfix(parser);
}
node_t *parse_multiplicative(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_multiplication(parser))       != NULL) { return node; }
    // if ((node = parse_division(parser))             != NULL) { return node; }
    return parse_exponential(parser);
}
node_t *parse_additive(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_subtraction(parser))          != NULL) { return node; }
    // if ((node = parse_addition(parser))             != NULL) { return node; }
    return parse_multiplicative(parser);
}
node_t *parse_relation(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_greater_or_equal(parser))     != NULL) { return node; }
    // if ((node = parse_less_or_equal(parser))        != NULL) { return node; }
    // if ((node = parse_greater(parser))              != NULL) { return node; }
    // if ((node = parse_less(parser))                 != NULL) { return node; }
    return parse_additive(parser);
}
node_t *parse_equality(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_is_not(parser))               != NULL) { return node; }
    // if ((node = parse_eq_not(parser))               != NULL) { return node; }
    // if ((node = parse_eq(parser))                   != NULL) { return node; }
    // if ((node = parse_is(parser))                   != NULL) { return node; }
    return parse_relation(parser);
}
node_t *parse_and(parser_t *parser) {
    node_t *operand0 = parse_equality(parser);

    advance_whitespace(parser);
    if(current_token(parser).type != TOKEN_AND) {
        return operand0;
    }
    advance_token(parser);
    advance_whitespace(parser);
    node_t *operand1 = parse_and(parser);

    if (operand1 == NULL) {
        PARSER_PANIC(parser, "Expected rvalue after 'AND' operator");
        return NULL;
    }
    struct node_binary *op = malloc(sizeof *op);
    op->operand0 = operand0;
    op->operand1 = operand1;

    struct node *node = malloc(sizeof *node);
    node->type = NODE_AND;
    node->value = op;

    return node;
}
node_t *parse_or(parser_t *parser) {
    node_t *operand0 = parse_and(parser);

    advance_whitespace(parser);
    if(current_token(parser).type != TOKEN_OR) {
        return operand0;
    }
    advance_token(parser);
    advance_whitespace(parser);
    node_t *operand1 = parse_or(parser);

    if (operand1 == NULL) {
        PARSER_PANIC(parser, "Expected rvalue after 'OR' operator");
        return NULL;
    }
    struct node_binary *op = malloc(sizeof *op);
    op->operand0 = operand0;
    op->operand1 = operand1;

    struct node *node = malloc(sizeof *node);
    node->type = NODE_OR;
    node->value = op;

    return node;
}



node_t *parse_assignment(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_assign(parser))               != NULL) { return node; }
    // if ((node = parse_assign_plus(parser))          != NULL) { return node; }
    // if ((node = parse_assign_minus(parser))         != NULL) { return node; }
    // if ((node = parse_assign_multiply(parser))      != NULL) { return node; }
    // if ((node = parse_assign_divide(parser))        != NULL) { return node; }
    // if ((node = parse_assign_modulus(parser))       != NULL) { return node; }
    return NULL;
}
node_t *parse_statement(parser_t *parser) {
    node_t *node = NULL;
    // if ((node = parse_switch(parser))               != NULL) { return node; }
    // if ((node = parse_if(parser))                   != NULL) { return node; }
    // if ((node = parse_while(parser))                != NULL) { return node; }
    // if ((node = parse_until(parser))                != NULL) { return node; }
    // if ((node = parse_for(parser))                  != NULL) { return node; }
    // if ((node = parse_try(parser))                  != NULL) { return node; }
    // if ((node = parse_space(parser))                != NULL) { return node; }
    // if ((node = parse_type(parser))                 != NULL) { return node; }
    // if ((node = parse_enum(parser))                 != NULL) { return node; }
    // if ((node = parse_func(parser))                 != NULL) { return node; }
    if ((node = parse_var(parser))                  != NULL) { return node; }
    if ((node = parse_val(parser))                  != NULL) { return node; }
    return parse_assignment(parser);
}

node_t *parse(parser_t *parser) {
    token_t *tokens = lex(parser->lexer);
    if (tokens == NULL) {
        PARSER_PANIC(parser, "Null tokens");
        free(parser->tokens);
        return NULL;
    }
    parser->tokens = tokens;
    return parse_statement(parser);
}


node_t *parse_identifier(parser_t *parser) {
    if (current_token(parser).type != TOKEN_IDENTIFIER) {
        return NULL;
    }
    struct node_identifier *op = malloc(sizeof *op);
    op->value = current_token(parser).value;
    node_t *node = malloc(sizeof *node);
    node->type = NODE_IDENTIFIER;
    node->value = op;
    advance_token(parser);
    return node;
}
node_t *parse_number(parser_t *parser) {
    if (current_token(parser).type != TOKEN_NUMBER) {
        return NULL;
    }
    struct node_number *op = malloc(sizeof *op);
    op->value = *(double *) current_token(parser).value;
    node_t *node = malloc(sizeof *node);
    node->type = NODE_NUMBER;
    node->value = op;
    advance_token(parser);
    return node;
}
node_t *parse_string(parser_t *parser) {
    if (current_token(parser).type != TOKEN_STRING) {
        return NULL;
    }
    struct node_string *op = malloc(sizeof *op);
    op->value = current_token(parser).value;
    node_t *node = malloc(sizeof *node);
    node->type = NODE_STRING;
    node->value = op;
    advance_token(parser);
    return node;
}
node_t *parse_boolean(parser_t *parser) {
    if (current_token(parser).type != TOKEN_BOOLEAN) {
        return NULL;
    }
    struct node_boolean *op = malloc(sizeof *op);
    op->value = *(char *)current_token(parser).value;
    node_t *node = malloc(sizeof *node);
    node->type = NODE_BOOLEAN;
    node->value = op;
    advance_token(parser);
    return node;
}

node_t *parse_compound(parser_t *parser) {
    // if (current_token(parser)->type) {}
    return NULL;
}
node_t *parse_if(parser_t *parser) {
    if (current_token(parser).type != TOKEN_IF) {

    }
    return NULL;
}
node_t *parse_switch(parser_t *parser) {
    if (current_token(parser).type != TOKEN_SWITCH) {
    }

    return NULL;
}
node_t *parse_while(parser_t *parser) {
    if (current_token(parser).type != TOKEN_WHILE) {

    }
    return NULL;
}
node_t *parse_until(parser_t *parser) {
    if (current_token(parser).type != TOKEN_UNTIL) {

    }
    return NULL;
}
node_t *parse_for(parser_t *parser) {
    if (current_token(parser).type != TOKEN_FOR) {
    }
    return NULL;
}

node_t *parse_var(parser_t *parser) {
    advance_whitespace(parser);
    if (current_token(parser).type != TOKEN_VAR) {
        return NULL;
    }
    advance_token(parser);
    advance_whitespace(parser);
    if (current_token(parser).type != TOKEN_IDENTIFIER) {
        PARSER_PANIC(parser, "Expected identifier");
        return NULL;
    }
    advance_token(parser);
    advance_whitespace(parser);

    if (current_token(parser).type == TOKEN_COLON) {
        advance_token(parser);
        advance_whitespace(parser);

        if (current_token(parser).type != TOKEN_IDENTIFIER) {
            PARSER_PANIC(parser, "Expected type");
            return NULL;
        }
        advance_token(parser);
    }

    advance_whitespace(parser);

    printf("VAR");
    //todo

    return NULL;
}
node_t *parse_val(parser_t *parser) {
    advance_whitespace(parser);
    if (current_token(parser).type != TOKEN_VAL) {
        return NULL;
    }
    advance_token(parser);
    advance_whitespace(parser);
    if (current_token(parser).type != TOKEN_IDENTIFIER) {
        PARSER_PANIC(parser, "Expected identifier");
        return NULL;
    }
    advance_token(parser);
    advance_whitespace(parser);

    if (current_token(parser).type == TOKEN_COLON) {
        advance_token(parser);
        advance_whitespace(parser);

        if (current_token(parser).type != TOKEN_IDENTIFIER) {
            PARSER_PANIC(parser, "Expected type");
            return NULL;
        }
        advance_token(parser);
    }

    advance_whitespace(parser);

    //todo

    return NULL;
}






token_t current_token(const parser_t *parser) {
    return parser->tokens[parser->seek];
}
void advance_token(parser_t *parser) {
    parser->seek++;
}
void retreat_token(parser_t *parser) {
    parser->seek--;
}
void advance_whitespace(parser_t *parser) {
    while (current_token(parser).type == TOKEN_WHITESPACE
        || current_token(parser).type == TOKEN_NEWLINE
        || current_token(parser).type == TOKEN_TAB) {
        advance_token(parser);
    }
}
void retreat_whitespace(parser_t *parser) {
    while (current_token(parser).type == TOKEN_WHITESPACE
        || current_token(parser).type == TOKEN_NEWLINE
        || current_token(parser).type == TOKEN_TAB) {
        retreat_token(parser);
    }
}