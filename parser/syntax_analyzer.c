#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/parser.h"
#include "include/ast_node.h"
// #include "include/gc.h"

/// Return current token
static Lexeme_t current(const Parser_t *parser);
/// Advance current token
static void advance(Parser_t *parser);
/// Retreat current token
static void retreat(Parser_t *parser);
/// Advance trivia tokens
static void advance_trivia(Parser_t *parser);
/// Retreat trivia tokens
static void retreat_trivia(Parser_t *parser);

static ASTNode_t *parse_file(Parser_t *parser);

static ASTNode_t *parse_declaration                           (Parser_t *parser);
static ASTNode_t *parse_func_dec(Parser_t *parser);
static ASTNode_t *parse_var_dec(Parser_t *parser);
static ASTNode_t *parse_val_dec(Parser_t *parser);

static ASTNode_t *parse_literal                               (Parser_t *parser);
static ASTNode_t *parse_primary                               (Parser_t *parser);
static ASTNode_t *parse_exponentation                         (Parser_t *parser);
static ASTNode_t *parse_multiplication                        (Parser_t *parser);
static ASTNode_t *parse_addition                              (Parser_t *parser);
static ASTNode_t *parse_statement                             (Parser_t *parser);
static ASTNode_t *parse_assignment                            (Parser_t *parser);

AST_t *parse(Parser_t *parser) {
    return parse_file(parser);
}
ASTNode_t *parse_file(Parser_t *parser) {
    size_t size = 1;
    size_t seek = 0;
    ASTNode_t *buf = malloc(sizeof(*buf) * size);

    if (buf == NULL) {
        PARSER_ERROR("malloc failed");
        return NULL;
    }

    while(current(parser).type != LexemeTypeEOF) {
        struct ASTNode *node = parse_assignment(parser);

        if (node == NULL) {
            PARSER_ERROR("Allocation failed");
            free(buf);
            free(node);
            return NULL;
        }

        while (seek >= size) {
            ASTNode_t *tmp = realloc(buf, sizeof(*buf) * (size = size * 2 + 1));
            if (tmp == NULL) {
                PARSER_ERROR("Out of memory");
                free(buf);
                free(node);
                return NULL;
            }
            buf = tmp;
        }

        buf[seek++] = *node;
        free(node);
    }

    struct ASTNode *ptr = malloc(sizeof(struct ASTNode) * seek);

    if (ptr == NULL) {
        PARSER_ERROR("Out of memory");
        free(buf);
        return NULL;
    }

    for (size_t i = 0; i < seek; i++) {
        ptr[i] = buf[i];
    }

    ptr[seek].type = NODE_SRV_EOF;

    free(buf);
    return ptr;
}


ASTNode_t *parse_assignment                            (Parser_t *parser) {
    ASTNode_t* const (*next)(Parser_t *parser) = parse_declaration;

    ASTNode_t *operand0 = next(parser);
    advance_trivia(parser);
    switch (current(parser).type) {
        case LexemeTypeAssign: {
            advance(parser); // skip =
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        case LexemeTypeAddAssign: {
            advance(parser); // skip +=
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN_PLUS;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        case LexemeTypeSubAssign: {
            advance(parser); // skip -=
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN_MINUS;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        case LexemeTypeMulAssign: {
            advance(parser); // skip *=
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN_MULTIPLY;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        case LexemeTypeDivAssign: {
            advance(parser); // skip /=
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN_DIVIDE;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        case LexemeTypeModAssign: {
            advance(parser); // skip %=
            advance_trivia(parser);
            ASTNode_t *operand1 = parse_assignment(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_ASSIGN_MODULUS;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        default: {
            return operand0;
        }
    }
}

ASTNode_t *parse_declaration                           (Parser_t *parser) {
    ASTNode_t* const (*next)(Parser_t *parser) = parse_addition;
    ASTNode_t *node = NULL;

    if ((node = parse_func_dec (parser)) != NULL)  { return node; }
    if ((node = parse_var_dec  (parser)) != NULL)  { return node; }
    if ((node = parse_val_dec  (parser)) != NULL)  { return node; }

    return next(parser);
}

ASTNode_t *parse_addition                              (Parser_t *parser) {
    ASTNode_t* const (*next)(Parser_t *parser) = parse_multiplication;

    ASTNode_t *operand0 = next(parser);
    while (current(parser).type != LexemeTypeEOF) {
        advance_trivia(parser);
        switch(current(parser).type) {
            case LexemeTypePlus: {
                advance(parser); //skip +
                ASTNode_t *operand1 = next(parser);

                ASTNode_t *node = malloc(sizeof *node);
                node->type = NODE_BIN_ADDITION;
                node->operand0 = operand0;
                node->operand1 = operand1;

                operand0 = node;
                break;
            }
            case LexemeTypeMinus: {
                advance(parser); //skip -
                ASTNode_t *operand1 = next(parser);

                ASTNode_t *node = malloc(sizeof *node);
                node->type = NODE_BIN_SUBTRACTION;
                node->operand0 = operand0;
                node->operand1 = operand1;

                operand0 = node;
                break;
            }
            default: {
                return operand0;
            }
        }
    }
}
ASTNode_t *parse_multiplication                        (Parser_t *parser) {
    ASTNode_t* const (*next)(Parser_t *parser) = parse_exponentation;

    ASTNode_t *operand0 = next(parser);
    while (current(parser).type != LexemeTypeEOF) {
        advance_trivia(parser);
        switch(current(parser).type) {
            case LexemeTypeMultiply: {
                advance(parser); //skip *
                ASTNode_t *operand1 = next(parser);

                ASTNode_t *node = malloc(sizeof *node);
                node->type = NODE_BIN_MULTIPLICATION;
                node->operand0 = operand0;
                node->operand1 = operand1;

                operand0 = node;
                break;
            }
            case LexemeTypeDivide: {
                advance(parser); //skip /
                ASTNode_t *operand1 = next(parser);

                ASTNode_t *node = malloc(sizeof *node);
                node->type = NODE_BIN_DIVISION;
                node->operand0 = operand0;
                node->operand1 = operand1;

                operand0 = node;
                break;
            }
            case LexemeTypeModulo: {
                advance(parser); // skip %
                ASTNode_t *operand1 = next(parser);

                ASTNode_t *node = malloc(sizeof *node);
                node->type = NODE_BIN_MODULO;
                node->operand0 = operand0;
                node->operand1 = operand1;

                operand0 = node;
                break;
            }
            default: {
                return operand0;
            }
        }
    }
}
ASTNode_t *parse_exponentation                         (Parser_t *parser)  {
    ASTNode_t* const (*next)(Parser_t *parser) = parse_primary;

    ASTNode_t *operand0 = next(parser);
    advance_trivia(parser);
    switch (current(parser).type) {
        case LexemeTypeExponent: {
            advance(parser); // skip **
            ASTNode_t *operand1 = parse_exponentation(parser);

            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_BIN_EXPONENTIATION;
            node->operand0 = operand0;
            node->operand1 = operand1;

            return node;
        }
        default: {
            return operand0;
        }
    }
}
ASTNode_t *parse_primary                               (Parser_t *parser) {
    advance_trivia(parser);
    return parse_literal(parser);
}
ASTNode_t *parse_literal                               (Parser_t *parser) {
    advance_trivia(parser);
    switch (current(parser).type) {
        case LexemeTypeIdentifier: {
            const Lexeme_t token = current(parser);
            advance(parser);
            PARSER_DEBUG("LITERAL: %s", token.data.identifier);
            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_LIT_IDENTIFIER;
            node->identifier_lit = token.data.identifier;
            return node;
        }
        case LexemeTypeNumber: {
            const Lexeme_t token = current(parser);
            PARSER_DEBUG("LITERAL: %f", token.data.dbl_number);
            advance(parser);
            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_LIT_NUMBER;
            node->decimal_lit = token.data.dbl_number;
            return node;
        }
        case LexemeTypeString: {
            const Lexeme_t token = current(parser);
            PARSER_DEBUG("LITERAL: %s", token.data.string);
            advance(parser);
            ASTNode_t *node = malloc(sizeof *node);
            node->type = NODE_LIT_STRING;
            node->string_lit = token.data.string;
            return node;
        }
        default: {
            return NULL;
        }
    }
}


//declarations

ASTNode_t *parse_var_dec                                   (Parser_t *parser) {
    advance_trivia(parser);
    if (current(parser).type != LexemeTypeVar) {
        return NULL;
    }

    char *name = NULL;
    char *type = NULL;

    advance(parser);
    advance_trivia(parser);
    if (current(parser).type != LexemeTypeIdentifier) {
        PARSER_ERROR("Expected name of var");
        return NULL;
    }

    name = current(parser).data.identifier;
    PARSER_DEBUG("VAR NAME = {%s}", name);
    advance(parser);
    advance_trivia(parser);

    if (current(parser).type == LexemeTypeColon) {
        advance(parser);
        advance_trivia(parser);
        if (current(parser).type != LexemeTypeIdentifier) {
            PARSER_ERROR("Expected type of var");
            return NULL;
        }
        type = current(parser).data.identifier;
        PARSER_DEBUG("VAR TYPE = {%s}", type);
        advance(parser);
    }

    ASTNode_t *node = malloc(sizeof *node);
    node->type = NODE_DEC_VAR;

    node->var_dec = malloc(sizeof *node->var_dec);
    node->var_dec->name = name;
    node->var_dec->type = type;

    return node;
}
ASTNode_t *parse_val_dec                                   (Parser_t *parser) {
    advance_trivia(parser);
    if (current(parser).type != LexemeTypeVal) {
        return NULL;
    }

    char *name = NULL;
    char *type = NULL;

    advance(parser); advance_trivia(parser);

    if (current(parser).type != LexemeTypeIdentifier) {
        PARSER_ERROR("Expected name of val");
        return NULL;
    }

    name = current(parser).data.identifier;
    PARSER_DEBUG("VAL NAME = {%s}", name);
    advance(parser);
    advance_trivia(parser);

    if (current(parser).type == LexemeTypeColon) {
        advance(parser);
        advance_trivia(parser);
        if (current(parser).type != LexemeTypeIdentifier) {
            PARSER_ERROR("Expected type of val");
            return NULL;
        }
        type = current(parser).data.identifier;
        PARSER_DEBUG("VAL TYPE = {%s}", type);
        advance(parser);
    }

    ASTNode_t *node = malloc(sizeof *node);
    node->type = NODE_DEC_VAL;

    node->val_dec = malloc(sizeof *node->val_dec);
    node->val_dec->name = name;
    node->val_dec->type = type;

    return node;
}
ASTNode_t *parse_func_dec                                  (Parser_t *parser) {
    advance_trivia(parser);
    if (current(parser).type != LexemeTypeFunc) {
        return NULL;
    }

    char *name = NULL;


    advance(parser);
    advance_trivia(parser);
    if (current(parser).type != LexemeTypeIdentifier) {
        PARSER_ERROR("Expected name of func");
        return NULL;
    }

    name = current(parser).data.identifier;
    PARSER_DEBUG("FUNC NAME = {%s}", name);
    advance(parser);
    advance_trivia(parser);

    ASTNode_t *args = NULL;
    if (current(parser).type == LexemeTypeOpenParen) {
        PARSER_ERROR("Expected parenthesis left");
        return NULL;
    }

    char *return_type = NULL;
    advance_trivia(parser);
    if (current(parser).type == LexemeTypeOpenAngle) {
        advance(parser);
        advance_trivia(parser);
        if (current(parser).type != LexemeTypeIdentifier) {
            PARSER_ERROR("Expected return type of func");
            return NULL;
        }
        return_type = current(parser).data.identifier;
        PARSER_DEBUG("FUNC RETURN TYPE = {%s}", return_type);
        advance(parser);
    }

    ASTNode_t *body = NULL;
    advance_trivia(parser);
    switch (current(parser).type) {
        case LexemeTypeSemicolon: {
            advance(parser);
            body = NULL;
            break;
        }
        case LexemeTypeOpenCurly: {
            while (current(parser).type != LexemeTypeCloseCurly)
                advance(parser);
            advance(parser);
            break;
        }
        default: {
            PARSER_ERROR("INVALID BODY DEFINITION");
        }
    }

    ASTNode_t *node = malloc(sizeof *node);
    node->type = NODE_DEC_FUNC;

    node->func_dec = malloc(sizeof (*node->func_dec));
    node->func_dec->name = name;
    node->func_dec->return_type = return_type;
    node->func_dec->args = args;
    node->func_dec->body = body;

    return node;
}

Lexeme_t current(const Parser_t *parser) {
    return parser->tokens[parser->seek];
}
void advance(Parser_t *parser) {
    parser->seek++;
    parser->position.x = current(parser).start.x;
    parser->position.y = current(parser).start.y;
}
void retreat(Parser_t *parser) {
    parser->seek--;
    parser->position.x = current(parser).start.x;
    parser->position.y = current(parser).start.y;
}
void advance_trivia(Parser_t *parser) {
    while (current(parser).type == LexemeTypeComment
        || current(parser).type == LexemeTypeNewLineTrivia
        || current(parser).type == LexemeTypeSpaceTrivia
        || current(parser).type == LexemeTypeTabTrivia) { advance(parser); }
}
void retreat_trivia(Parser_t *parser) {
    while (current(parser).type == LexemeTypeComment
        || current(parser).type == LexemeTypeNewLineTrivia
        || current(parser).type == LexemeTypeSpaceTrivia
        || current(parser).type == LexemeTypeTabTrivia) { retreat(parser); }
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
// //     if (current(parser).type != LEXEME_OPR_PARENTHESIS_LEFT) {
// //         return NULL;
// //     }
// //     advance(parser); // skip (
// //
// //     node_t *node = parse(parser);
// //     if (current(parser).type != LEXEME_OPR_PARENTHESIS_RIGHT) {
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
// node_t *parse_multiplication(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_multiplication(parser))       != NULL) { return node; }
//     // if ((node = parse_division(parser))             != NULL) { return node; }
//     return parse_exponential(parser);
// }
// node_t *parse_addition(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_subtraction(parser))          != NULL) { return node; }
//     // if ((node = parse_addition(parser))             != NULL) { return node; }
//     return parse_multiplication(parser);
// }
// node_t *parse_relation(parser_t *parser) {
//     node_t *node = NULL;
//     // if ((node = parse_greater_or_equal(parser))     != NULL) { return node; }
//     // if ((node = parse_less_or_equal(parser))        != NULL) { return node; }
//     // if ((node = parse_greater(parser))              != NULL) { return node; }
//     // if ((node = parse_less(parser))                 != NULL) { return node; }
//     return parse_addition(parser);
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
//     if(current(parser).type != LEXEME_KOR_AND) {
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
//     node->type = NODE_BIN_AND;
//     node->value = op;
//
//     return node;
// }
// node_t *parse_or(parser_t *parser) {
//     node_t *operand0 = parse_and(parser);
//
//     advance_whitespace(parser);
//     if(current(parser).type != LEXEME_KOR_OR) {
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
//     node->type = NODE_BIN_OR;
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
//     if (current(parser).type != LexemeTypeIdentifier) {
//         return NULL;
//     }
//     struct node_identifier *op = malloc(sizeof *op);
//     op->value = current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_LIT_IDENTIFIER;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_number(parser_t *parser) {
//     if (current(parser).type != LEXEME_LIT_NUMBER) {
//         return NULL;
//     }
//     struct node_number *op = malloc(sizeof *op);
//     op->value = *(double *) current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_LIT_NUMBER;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_string(parser_t *parser) {
//     if (current(parser).type != LEXEME_LIT_STRING) {
//         return NULL;
//     }
//     struct node_string *op = malloc(sizeof *op);
//     op->value = current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_LIT_STRING;
//     node->value = op;
//     advance(parser);
//     return node;
// }
// node_t *parse_boolean(parser_t *parser) {
//     if (current(parser).type != LEXEME_BOOLEAN) {
//         return NULL;
//     }
//     struct node_boolean *op = malloc(sizeof *op);
//     op->value = *(char *)current(parser).value;
//     node_t *node = malloc(sizeof *node);
//     node->type = NODE_BOOLEAN;
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
//     if (current(parser).type != LEXEME_KWD_IF) {
//
//     }
//     return NULL;
// }
// node_t *parse_switch(parser_t *parser) {
//     if (current(parser).type != LEXEME_KWD_SWITCH) {
//     }
//
//     return NULL;
// }
// node_t *parse_while(parser_t *parser) {
//     if (current(parser).type != LEXEME_KWD_WHILE) {
//
//     }
//     return NULL;
// }
// node_t *parse_until(parser_t *parser) {
//     if (current(parser).type != LEXEME_KWD_UNTIL) {
//
//     }
//     return NULL;
// }
// node_t *parse_for(parser_t *parser) {
//     if (current(parser).type != LEXEME_KWD_FOR) {
//     }
//     return NULL;
// }