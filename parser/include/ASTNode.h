#pragma once

#include "common.h"

typedef struct ASTNode {
    /// Type of node
    int type;
    /// Data of node
    union {
        literal_data_t literal;

        void* custom;

        struct { struct ASTNode* operand0; } unary;

        struct { struct ASTNode* operand0;
                 struct ASTNode* operand1; } binary;

        struct { struct ASTNode* operand0;
                 struct ASTNode* operand1;
                 struct ASTNode* operand2; } ternary;
    } data;

    /// Start position of node
    position_t position_start;
    /// End position of node
    position_t position_end;
} node_t;

struct node_file {
    struct ASTNode *nodes;
};

struct node_proj {
    struct ASTNode *files;
};

struct node_dec_var {
    char* name;
    char* type;
};

struct node_dec_val {
    char* name;
    char* type;
};

struct node_dec_func {
    char* name;
    char* return_type;

    struct ASTNode* args;
    struct ASTNode* body;
};


struct node_param {
    char* name;
    char* type;
};


//service
#define NODE_SRV_EOF                                                                                                   (0)
#define NODE_SRV_FILE                                                                                                 (-1)
#define NODE_SRV_PROJ                                                                                                 (-1)

//literal
/// NODE: IDENTIFIER
#define NODE_LIT_IDENTIFIER                                                                                           (-1)
/// NODE: NUMBER
#define NODE_LIT_NUMBER                                                                                               (-1)
/// NODE: STRING
#define NODE_LIT_STRING                                                                                               (-1)

//declaration
///NODE: '{' NODE* '}' | ';'
#define NODE_BODY                                                                                                (-1)
/// NODE: type IDENTIFIER (':' IDENTIFIER)? BODY
#define NODE_DEC_TYPE                                                                                                 (-1)
/// NODE: enum IDENTIFIER BODY
#define NODE_DEC_ENUM                                                                                                 (-1)
///NODE: func ID FUNC_ARGS ('>' ID)? BODY
#define NODE_DEC_FUNC                                                                                                 (-1)
///NODE: var ID (':' ID)?
#define NODE_DEC_VAR                                                                                             (99)
///NODE: val ID (':' ID)?
#define NODE_DEC_VAL                                                                                            (100)

//branch
#define NODE_STM_IF
#define NODE_STM_ELIF
#define NODE_STM_ELSE
#define NODE_STM_SWITCH
#define NODE_STM_CASE

//loop
#define NODE_STM_FOR
#define NODE_STM_WHILE
#define NODE_STM_UNTIL

//jump
#define NODE_JMP_NEXT
#define NODE_JMP_BACK
#define NODE_JMP_STOP
#define NODE_JMP_REPEAT
#define NODE_JMP_RETURN
#define NODE_JMP_EJECT
#define NODE_JMP_YIELD

//arithmetic
#define NODE_BIN_ADDITION                                                                                             (-1)
#define NODE_BIN_SUBTRACTION                                                                                          (-1)
#define NODE_BIN_MULTIPLICATION                                                                                       (-1)
#define NODE_BIN_DIVISION                                                                                             (-1)
#define NODE_BIN_MODULO                                                                                               (-1)
#define NODE_BIN_EXPONENTIATION                                                                                       (-1)
#define NODE_UNY_INCREMENT                                                                                            (-1)
#define NODE_UNY_DECREMENT                                                                                            (-1)

//relation
#define NODE_BIN_EQUAL                                                                                                (-1)
#define NODE_BIN_EQUAL_NOT                                                                                            (-1)
#define NODE_BIN_LESS                                                                                                 (-1)
#define NODE_BIN_LESS_OR_EQUAL                                                                                        (-1)
#define NODE_BIN_GREATER                                                                                              (-1)
#define NODE_BIN_GREATER_OR_EQUAL                                                                                     (-1)

//equality
/// TOKEN: eq | ==
#define NODE_BIN_EQ                                                                                                  (620)
/// TOKEN: !=
#define NODE_BIN_EQ_NOT                                                                                              (630)
/// TOKEN: is | ===
#define NODE_BIN_IS                                                                                                  (640)
/// TOKEN: !==
#define NODE_BIN_IS_NOT

//logical
#define NODE_BIN_OR                                                                                                   (-1)
#define NODE_BIN_AND                                                                                                  (-1)
#define NODE_UNY_NOT




//assign
/// TOKEN: node = assign
#define NODE_BIN_ASSIGN                                                                                              (232)
/// TOKEN: node += assign
#define NODE_BIN_ASSIGN_PLUS                                                                                         (233)
/// TOKEN: node -= assign
#define NODE_BIN_ASSIGN_MINUS                                                                                        (234)
/// TOKEN: node *= assign
#define NODE_BIN_ASSIGN_MULTIPLY                                                                                     (235)
/// TOKEN: node /= assign
#define NODE_BIN_ASSIGN_DIVIDE                                                                                       (236)
/// TOKEN: node %= assign
#define NODE_BIN_ASSIGN_MODULUS                                                                                      (237)