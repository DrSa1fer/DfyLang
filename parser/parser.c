// #include <stdlib.h>
// #include <stdio.h>
//
// #include "grammatic.h"
// #include "parser.h"
// #include "node.h"
//
// void advance_token(parser_t *parser);
// token_t *current_token(const parser_t *parser);
//
// int main(void) {
//     char *src = "10=0+1-1*0+10";
//     lexer_t lexer = {
//         .column = 0,
//         .row = 0,
//         .seek = 0,
//         .text = src
//     };
//
//     parser_t parser = {
//         .lexer = &lexer,
//         .token = NULL,
//     };
//
//     advance_token(&parser); // move to first token
//     const node_t *node = parse(&parser);
//     while (node != NULL) {
//         node = parse(&parser);
//     }
// }
//
// node_t *parse_block(parser_t *parser) {
//     // if (current_token(parser)->type) {}
//     return NULL;
// }
//
// node_t *parse_if(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_IF) {
//
//     }
//     return NULL;
// }
// node_t *parse_switch(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_SWITCH) {
//     }
//
//     return NULL;
// }
//
// node_t *parse_while(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_WHILE) {
//
//     }
//     return NULL;
// }
// node_t *parse_until(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_UNTIL) {
//
//     }
//     return NULL;
// }
// node_t *parse_for(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_FOR) {
//
//     }
//     return NULL;
// }
//
// node_t *parse_variable(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_VAR) {
//     }
//     return NULL;
// }
// node_t *parse_valiable(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_VAL) {
//     }
//     return NULL;
// }
// node_t *parse_function(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_FUNC) {
//     }
//     return NULL;
// }
// node_t *parse_type(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_TYPE) {
//     }
//     return NULL;
// }
// node_t *parse_enum(parser_t *parser) {
//     if (current_token(parser)->type != TOKEN_ENUM) {
//     }
//     return NULL;
// }
//
// node_t *parse_number(parser_t *parser) {
//     const token_t *token = current_token(parser);
//     if (token->type != TOKEN_NUMBER) {
//         printf("Expected a number, got %s\n", token->value);
//         return NULL;
//     }
//     advance_token(parser);
//
//     node_t *number = malloc(sizeof *number);
//     number->type = NODE_NUMBER;
//     number->value = token->value;
//     return number;
// }
//
// node_t *parse_order(parser_t *parser) {
//     if (current_token(parser)-> type != TOKEN_LEFT_PARENTHESES) {
//         return NULL;
//     }
//     advance_token(parser); // skip (
//
//     node_t *node = parse(parser);
//     if (current_token(parser)-> type != TOKEN_RIGHT_PARENTHESES) {
//         fprintf(stderr, "Expected ')' after left parentheses\n");
//     }
//     else {
//         advance_token(parser); // skip )
//     }
//
//     return node;
// }
// node_t *parse_primary(parser_t *parser) {
//     node_t *operand0 = parse_order(parser);
//     if(current_token(parser)->type != TOKEN_NUMBER &&
//         current_token(parser)->type != TOKEN_STRING &&
//         current_token(parser)->type != TOKEN_IDENTIFIER) {
//         return operand0;
//     }
//     printf(current_token(parser)-> value);
//     return NULL;
// }
// node_t *parse_postfix(parser_t *parser) {
//     node_t *operand0 = parse_primary(parser);
//     return operand0;
// }
// node_t *parse_prefix(parser_t *parser) {
//     node_t *operand0 = parse_postfix(parser);
//     return operand0;
// }
// node_t *parse_multiplicative(parser_t *parser) {
//     node_t *operand0 = parse_prefix(parser);
//     if(current_token(parser)->type != TOKEN_MULTIPLY &&
//         current_token(parser)->type != TOKEN_MODULUS) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_multiplicative(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_MULTIPLY: {
//             node->type = NODE_MULTIPLY;
//             return node;
//         }
//         case TOKEN_MODULUS: {
//             node->type = NODE_MODULO;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_additive(parser_t *parser) {
//     node_t *operand0 = parse_multiplicative(parser);
//     if(current_token(parser)->type != TOKEN_PLUS &&
//         current_token(parser)->type != TOKEN_MINUS) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_additive(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_PLUS: {
//             node->type = NODE_PLUS;
//             return node;
//         }
//         case TOKEN_MINUS: {
//             node->type = NODE_MINUS;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_shift(parser_t *parser) {
//     node_t *operand0 = parse_additive(parser);
//     return operand0;
// }
// node_t *parse_relation(parser_t *parser) {
//     node_t *operand0 = parse_shift(parser);
//
//     if(current_token(parser)->type != TOKEN_LESS &&
//         current_token(parser)->type != TOKEN_GREATER &&
//         current_token(parser)->type != TOKEN_LESS_OR_EQUAL &&
//         current_token(parser)->type != TOKEN_GREATER_OR_EQUAL) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_relation(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_LESS: {
//             node->type = NODE_LESS;
//             return node;
//         }
//         case TOKEN_GREATER: {
//             node->type = NODE_GREATER;
//             return node;
//         }
//         case TOKEN_LESS_OR_EQUAL: {
//             node->type = NODE_LESS_OR_EQUAL;
//             return node;
//         }
//         case TOKEN_GREATER_OR_EQUAL: {
//             node->type = NODE_GREATER_OR_EQUAL;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_equality(parser_t *parser) {
//     node_t *operand0 = parse_relation(parser);
//
//     if(current_token(parser)->type != TOKEN_EQUAL &&
//         current_token(parser)->type != TOKEN_EQUAL_NOT) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_relation(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_EQUAL: {
//             node->type = NODE_EQUAL;
//             return node;
//         }
//         case TOKEN_EQUAL_NOT: {
//             node->type = NODE_NOT_EQUAL;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_logical_and(parser_t *parser) {
//     node_t *operand0 = parse_equality(parser);
//
//     if(current_token(parser)->type != TOKEN_AND) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_logical_and(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_AND: {
//             node->type = NODE_AND;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_logical_or(parser_t *parser) {
//     node_t *operand0 = parse_logical_and(parser);
//
//     if(current_token(parser)->type != TOKEN_OR) {
//         return operand0;
//     }
//
//     advance_token(parser);
//     node_t *operand1 = parse_logical_or(parser);
//
//     struct node_binary_operation *op = malloc(sizeof *op);
//     op->operand0 = operand0;
//     op->operand1 = operand1;
//
//     struct node *node = malloc(sizeof *node);
//     node->value = op;
//
//     switch (current_token(parser)->type) {
//         case TOKEN_OR: {
//             node->type = NODE_OR;
//             return node;
//         }
//         default: {
//             fprintf(stderr, "Await rvalue after operator");
//             return NULL;
//         }
//     }
// }
// node_t *parse_assignment(parser_t *parser) {
//     node_t *operand0 = parse_equality(parser);
//
//     if (current_token(parser)->type == TOKEN_ASSIGN) {
//         advance_token(parser);
//         node_t *operand1 = parse_assignment(parser);
//
//         struct node_assign *assign = malloc(sizeof *assign);
//         assign->target = operand0;
//         assign->value = operand1;
//
//         struct node *node = malloc(sizeof *node);
//         node->type = NODE_ASSIGN;
//         node->value = assign;
//
//         return node;
//     }
//     return operand0;
// }
// node_t *parse(parser_t *parser) {
//     return parse_assignment(parser);
// }
//
// void advance_token(parser_t *parser) {
//     parser->token = lex(parser->lexer);
// }
// token_t *current_token(const parser_t *parser) {
//   return parser->token;
// }