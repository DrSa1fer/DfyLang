#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/parser.h"

///Alloc new node and set value
static struct node *new_node(int type, void *data);
/// Return current token
static token_t current(const parser_t *parser);
/// Advance current token
static void advance(parser_t *parser);
/// Retreat current token
static void retreat(parser_t *parser);
/// Skip trivia tokens
static void skip_trivia(parser_t *parser);

node_t *parse_file(parser_t *parser);

node_t *parse_func(parser_t *parser);
node_t *parse_var(parser_t *parser);
node_t *parse_val(parser_t *parser);

node_t *parse_primary(parser_t *parser);
node_t *parse_literal(parser_t *parser);
node_t *parse_statement(parser_t *parser);


node_t *parse(parser_t *parser) {
    return parse_file(parser);
}

node_t *parse_file(parser_t *parser) {
    size_t size = 100;
    size_t seek = 0;
    node_t *buf = malloc(sizeof *buf * size);

    while(1) {
        node_t *node = parse_statement(parser);

        if (node == NULL) {
            PARSER_ERROR("Invalid token '%d'", current(parser).type);
            advance(parser);
            free(buf);
            return NULL;
        }

        if (seek >= size) {
            void *tmp = realloc(buf, sizeof(*buf) * (size *= 2));
            if (tmp == NULL) {
                PARSER_ERROR("Out of memory");
                free(buf);
                free(node);
                return NULL;
            }
            buf = tmp;
        }
        buf[seek++] = *node;

        if (node->type == NODE_TYPE_EOF) {
            node_t *ptr = malloc(sizeof *ptr * seek);
            if (ptr == NULL) {
                PARSER_ERROR("Out of memory");
                free(buf);
                free(node);
                return NULL;
            }

            for (size_t i = 0; i < seek; i++) {
                ptr[i] = buf[i];
            }

            free(buf);
            free(node);
            return new_node(0, ptr);
        }

        free(node);
    }
}

node_t *parse_statement(parser_t *parser) {
    node_t *node = NULL;

    if ((node = parse_func (parser)) != NULL)  { return node; }
    if ((node = parse_var  (parser)) != NULL)  { return node; }
    if ((node = parse_val  (parser)) != NULL)  { return node; }

    return parse_primary(parser);
}

node_t *parse_var(parser_t *parser) {
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_VAR) {
        return NULL;
    }

    char *name = NULL;
    char *type = NULL;

    advance(parser);
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
        PARSER_ERROR("Expected name of var");
        return NULL;
    }

    name = (char *) current(parser).data;
    PARSER_DEBUG("VAR NAME = {%s}", name);
    advance(parser);
    skip_trivia(parser);

    if (current(parser).type == TOKEN_TYPE_COLON) {
        advance(parser);
        skip_trivia(parser);
        if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
            PARSER_ERROR("Expected type of var");
            return NULL;
        }
        type = (char *) current(parser).data;
        PARSER_DEBUG("VAR TYPE = {%s}", type);
        advance(parser);
    }

    struct node_var *var = malloc(sizeof *var);
    var->name = name;
    var->type = type;
    return new_node(NODE_TYPE_VAR, var);
}
node_t *parse_val(parser_t *parser) {
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_VAL) {
        return NULL;
    }

    char *name = NULL;
    char *type = NULL;

    advance(parser);
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
        PARSER_ERROR("Expected name of val");
        return NULL;
    }

    name = current(parser).data;
    PARSER_DEBUG("VAL NAME = {%s}", name);
    advance(parser);
    skip_trivia(parser);

    if (current(parser).type == TOKEN_TYPE_COLON) {
        advance(parser);
        skip_trivia(parser);
        if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
            PARSER_ERROR("Expected type of val");
            return NULL;
        }
        type = current(parser).data;
        PARSER_DEBUG("VAL TYPE = {%s}", type);
        advance(parser);
    }

    struct node_var *var = malloc(sizeof *var);
    var->name = name;
    var->type = type;
    return new_node(NODE_TYPE_VAL, var);
}
node_t *parse_func(parser_t *parser) {
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_FUNC) {
        return NULL;
    }

    char *name = NULL;
    char *return_type = NULL;

    advance(parser);
    skip_trivia(parser);
    if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
        PARSER_ERROR("Expected name of func");
        return NULL;
    }

    name = current(parser).data;
    PARSER_DEBUG("FUNC NAME = {%s}", name);
    advance(parser);
    skip_trivia(parser);

    if (current(parser).type == TOKEN_TYPE_PARENTHESIS_LEFT) {
        PARSER_ERROR("Expected parenthesis left");
        return NULL;
    }

    if (current(parser).type == TOKEN_TYPE_ANGEL_BRACKET_RIGHT) {
        advance(parser);
        skip_trivia(parser);
        if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
            PARSER_ERROR("Expected return type of func");
            return NULL;
        }
        return_type = current(parser).data;
        PARSER_DEBUG("FUNC RETURN TYPE = {%s}", return_type);
        advance(parser);
    }

    struct node_func *func = malloc(sizeof *func);
    func->name = name;
    func->return_type = return_type;
    func->body = NULL;
    func->args = NULL;

    return new_node(NODE_TYPE_FUNC, func);
}
node_t *parse_primary(parser_t *parser) {
    skip_trivia(parser);
    switch (current(parser).type) {
        case TOKEN_TYPE_EOF: {
            advance(parser);
            PARSER_DEBUG("EOF")
            return new_node(NODE_TYPE_EOF, NULL);
        }
        default: {
            return parse_literal(parser);
        }
    }
}
node_t *parse_literal(parser_t *parser) {
    skip_trivia(parser);
    switch (current(parser).type) {
        case TOKEN_TYPE_LITERAL_IDENTIFIER: {
            const token_t token = current(parser);
            advance(parser);
            return new_node(NODE_TYPE_IDENTIFIER, token.data);
        }
        case TOKEN_TYPE_LITERAL_NUMBER: {
            const token_t token = current(parser);
            advance(parser);
            return new_node(NODE_TYPE_NUMBER, token.data);
        }
        case TOKEN_TYPE_LITERAL_STRING: {
            const token_t token = current(parser);
            advance(parser);
            return new_node(NODE_TYPE_STRING, token.data);
        }
        default: {
            return NULL;
        }
    }
}

token_t current(const parser_t *parser) {
    return parser->tokens[parser->seek];
}
void advance(parser_t *parser) {
    parser->seek++;
}
void retreat(parser_t *parser) {
    parser->seek--;
}
void skip_trivia(parser_t *parser) {
    while (current(parser).type == TOKEN_TYPE_TRIVIA_COMMENT
        || current(parser).type == TOKEN_TYPE_TRIVIA_NEWLINE
        || current(parser).type == TOKEN_TYPE_TRIVIA_SPACE
        || current(parser).type == TOKEN_TYPE_TRIVIA_TAB) { advance(parser); }
}

struct node *new_node(const int type, void *data) {
    struct node *node = malloc(sizeof(struct node));
    node->type = type;
    node->data = data;
    return node;
}






// node_t *parse_boolean                               (parser_t *parser);
//
// /// EXPRESSION: a.b
// node_t *parse_access                                (parser_t *parser);
// /// EXPRESSION: INVOKE = a(ARG, ...) | a ARG, ...
// node_t *parse_invoke                                (parser_t *parser);
// /// EXPRESSION: ARG = a : b | a
// node_t *parse_invoke_arg                            (parser_t *parser);
// /// EXPRESSION: a[b]
// node_t *parse_index                                 (parser_t *parser);

// node_t *parse_subexpr                               (parser_t *parser);
//
// node_t *parse_postfix                               (parser_t *parser);
// node_t *parse_increment                             (parser_t *parser);
// node_t *parse_decrement                             (parser_t *parser);
//
// node_t *parse_prefix                                (parser_t *parser);
// node_t *parse_increment                             (parser_t *parser);
// node_t *parse_decrement                             (parser_t *parser);
// node_t *parse_minus                                 (parser_t *parser);
// node_t *parse_not                                   (parser_t *parser);
//
// node_t *parse_exponential                           (parser_t *parser);
// node_t *parse_power                                 (parser_t *parser);
// node_t *parse_root                                  (parser_t *parser);
//
// node_t *parse_multiplicative                        (parser_t *parser);
// node_t *parse_multiplication                        (parser_t *parser);
// node_t *parse_division                              (parser_t *parser);
// node_t *parse_modulo                                (parser_t *parser);
//
// node_t *parse_additive                              (parser_t *parser);
// node_t *parse_addition                              (parser_t *parser);
// node_t *parse_subtraction                           (parser_t *parser);
//
// node_t *parse_relational                            (parser_t *parser);
// node_t *parse_less                                  (parser_t *parser);
// node_t *parse_greater                               (parser_t *parser);
// node_t *parse_less_or_equal                         (parser_t *parser);
// node_t *parse_greater_or_equal                      (parser_t *parser);
//
// node_t *parse_equality                              (parser_t *parser);
// node_t *parse_eq                                    (parser_t *parser);
// node_t *parse_is                                    (parser_t *parser);
// node_t *parse_eq_not                                (parser_t *parser);
// node_t *parse_is_not                                (parser_t *parser);
//
// node_t *parse_or                                    (parser_t *parser);
// node_t *parse_and                                   (parser_t *parser);
//
// node_t *parse_statement                             (parser_t *parser);
// node_t *parse_compound                              (parser_t *parser);
// node_t *parse_switch                                (parser_t *parser);
//     node_t *parse_case                              (parser_t *parser);
//
// node_t *parse_if                                    (parser_t *parser);
//     node_t *parse_elif                              (parser_t *parser);
//     node_t *parse_else                              (parser_t *parser);
//
// node_t *parse_while                                 (parser_t *parser);
// node_t *parse_until                                 (parser_t *parser);
// node_t *parse_for                                   (parser_t *parser);
//     node_t *parse_repeat                            (parser_t *parser);
//     node_t *parse_next                              (parser_t *parser);
//     node_t *parse_back                              (parser_t *parser);
//     node_t *parse_stop                              (parser_t *parser);
//     node_t *parse_return                            (parser_t *parser);
//     node_t *parse_eject                             (parser_t *parser);
//     node_t *parse_yield                             (parser_t *parser);
//
// node_t *parse_try                                   (parser_t *parser);
//     node_t *parse_catch                             (parser_t *parser);
//     node_t *parse_finally                           (parser_t *parser);
//
// node_t *parse_declaration                           (parser_t *parser);
// node_t *parse_var                                   (parser_t *parser);
// node_t *parse_val                                   (parser_t *parser);
// node_t *parse_type                                  (parser_t *parser);
// node_t *parse_enum                                  (parser_t *parser);
// node_t *parse_space                                 (parser_t *parser);
// node_t *parse_func                                  (parser_t *parser);
//
// node_t *parse_assignment                            (parser_t *parser);
// node_t *parse_assign                                (parser_t *parser);
// node_t *parse_assign_plus                           (parser_t *parser);
// node_t *parse_assign_minus                          (parser_t *parser);
// node_t *parse_assign_multiply                       (parser_t *parser);
// node_t *parse_assign_divide                         (parser_t *parser);
// node_t *parse_assign_modulus                        (parser_t *parser);
//
// // node_t *parse_sub(parser_t *parser) {
// //     if (current(parser).type != TOKEN_TYPE_PARENTHESIS_LEFT) {
// //         return NULL;
// //     }
// //     advance(parser); // skip (
// //
// //     node_t *node = parse(parser);
// //     if (current(parser).type != TOKEN_TYPE_PARENTHESIS_RIGHT) {
// //         PARSER_ERROR(parser, "Expected ')' after left parentheses");
// //         return NULL;
// //     }
// //     advance(parser); // skip ) :3
// //
// //     return node;
// // }
//
//
// node_t *parse_postfix(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_increment(parser))            != NULL) { return node; }
//     // if ((node = parse_decrement(parser))            != NULL) { return node; }
//     return parse_primary(parser);
// }
// node_t *parse_prefix(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_increment(parser))            != NULL) { return node; }
//     // if ((node = parse_decrement(parser))            != NULL) { return node; }
//     // if ((node = parse_not(parser))                  != NULL) { return node; }
//     // if ((node = parse_minus(parser))                != NULL) { return node; }
//     return parse_postfix(parser);
// }
// node_t *parse_exponential(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_power(parser))                != NULL) { return node; }
//     // if ((node = parse_root(parser))                 != NULL) { return node; }
//     return parse_postfix(parser);
// }
// node_t *parse_multiplicative(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_multiplication(parser))       != NULL) { return node; }
//     // if ((node = parse_division(parser))             != NULL) { return node; }
//     return parse_exponential(parser);
// }
// node_t *parse_additive(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_subtraction(parser))          != NULL) { return node; }
//     // if ((node = parse_addition(parser))             != NULL) { return node; }
//     return parse_multiplicative(parser);
// }
// node_t *parse_relation(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_greater_or_equal(parser))     != NULL) { return node; }
//     // if ((node = parse_less_or_equal(parser))        != NULL) { return node; }
//     // if ((node = parse_greater(parser))              != NULL) { return node; }
//     // if ((node = parse_less(parser))                 != NULL) { return node; }
//     return parse_additive(parser);
// }
// node_t *parse_equality(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_is_not(parser))               != NULL) { return node; }
//     // if ((node = parse_eq_not(parser))               != NULL) { return node; }
//     // if ((node = parse_eq(parser))                   != NULL) { return node; }
//     // if ((node = parse_is(parser))                   != NULL) { return node; }
//     return parse_relation(parser);
// }
// node_t *parse_and(parser_t *parser) {
//     node_t *operand0 = parse_equality(parser);
//
//     advance_whitespace(parser);
//     if(current(parser).type != TOKEN_TYPE_AND) {
//         return operand0;
//     }
//     advance(parser);
//     advance_whitespace(parser);
//     node_t *operand1 = parse_and(parser);
//
//     if (operand1 == NULL) {
//         PARSER_ERROR(parser, "Expected rvalue after 'AND' operator");
//         return NULL;
//     }
//     struct node_binary *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_AND;
//     node->value = op;
//
//     return node;
// }
// node_t *parse_or(parser_t *parser) {
//     node_t *operand0 = parse_and(parser);
//
//     advance_whitespace(parser);
//     if(current(parser).type != TOKEN_TYPE_OR) {
//         return operand0;
//     }
//     advance(parser);
//     advance_whitespace(parser);
//     node_t *operand1 = parse_or(parser);
//
//     if (operand1 == NULL) {
//         PARSER_ERROR(parser, "Expected rvalue after 'OR' operator");
//         return NULL;
//     }
//     struct node_binary *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_OR;
//     node->value = op;
//
//     return node;
// }
//
//
//
// node_t *parse_assignment(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_assign(parser))               != NULL) { return node; }
//     // if ((node = parse_assign_plus(parser))          != NULL) { return node; }
//     // if ((node = parse_assign_minus(parser))         != NULL) { return node; }
//     // if ((node = parse_assign_multiply(parser))      != NULL) { return node; }
//     // if ((node = parse_assign_divide(parser))        != NULL) { return node; }
//     // if ((node = parse_assign_modulus(parser))       != NULL) { return node; }
//     return NULL;
// }
// node_t *parse_statement(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_switch(parser))               != NULL) { return node; }
//     // if ((node = parse_if(parser))                   != NULL) { return node; }
//     // if ((node = parse_while(parser))                != NULL) { return node; }
//     // if ((node = parse_until(parser))                != NULL) { return node; }
//     // if ((node = parse_for(parser))                  != NULL) { return node; }
//     // if ((node = parse_try(parser))                  != NULL) { return node; }
//     // if ((node = parse_space(parser))                != NULL) { return node; }
//     // if ((node = parse_type(parser))                 != NULL) { return node; }
//     // if ((node = parse_enum(parser))                 != NULL) { return node; }
//     // if ((node = parse_func(parser))                 != NULL) { return node; }
//     if ((node = parse_var(parser))                  != NULL) { return node; }
//     if ((node = parse_val(parser))                  != NULL) { return node; }
//     return parse_assignment(parser);
// }
//
// node_t *parse(parser_t *parser) {
//     token_t *tokens = lex(parser->lexer);
//     if (tokens == NULL) {
//         PARSER_ERROR(parser, "Null tokens");
//         free(parser->tokens);
//         return NULL;
//     }
//     parser->tokens = tokens;
//     return parse_statement(parser);
// }
//
//
// node_t *parse_identifier(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
//         return NULL;
//     }
//     struct node_identifier *op = malloc(sizeof *op);
//     op->value = current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_IDENTIFIER;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_number(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_LITERAL_NUMBER) {
//         return NULL;
//     }
//     struct node_number *op = malloc(sizeof *op);
//     op->value = *(double *) current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_NUMBER;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_string(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_LITERAL_STRING) {
//         return NULL;
//     }
//     struct node_string *op = malloc(sizeof *op);
//     op->value = current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_STRING;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_boolean(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_BOOLEAN) {
//         return NULL;
//     }
//     struct node_boolean *op = malloc(sizeof *op);
//     op->value = *(char *)current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_TYPE_BOOLEAN;
//     node->value = op;
//     advance(parser);
//     return node;
// }
//
// node_t *parse_compound(parser_t *parser) {
//     // if (current(parser)->type) {}
//     return NULL;
// }
// node_t *parse_if(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_IF) {
//
//     }
//     return NULL;
// }
// node_t *parse_switch(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_SWITCH) {
//     }
//
//     return NULL;
// }
// node_t *parse_while(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_WHILE) {
//
//     }
//     return NULL;
// }
// node_t *parse_until(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_UNTIL) {
//
//     }
//     return NULL;
// }
// node_t *parse_for(parser_t *parser) {
//     if (current(parser).type != TOKEN_TYPE_FOR) {
//     }
//     return NULL;
// }
//

// node_t *parse_val(parser_t *parser) {
//     advance_whitespace(parser);
//     if (current(parser).type != TOKEN_TYPE_VAL) {
//         return NULL;
//     }
//     advance(parser);
//     advance_whitespace(parser);
//     if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
//         PARSER_ERROR(parser, "Expected identifier");
//         return NULL;
//     }
//     advance(parser);
//     advance_whitespace(parser);
//
//     if (current(parser).type == TOKEN_TYPE_COLON) {
//         advance(parser);
//         advance_whitespace(parser);
//
//         if (current(parser).type != TOKEN_TYPE_LITERAL_IDENTIFIER) {
//             PARSER_ERROR(parser, "Expected type");
//             return NULL;
//         }
//         advance(parser);
//     }
//
//     advance_whitespace(parser);
//
//     //todo
//
//     return NULL;
// }
//
//
//
//
//