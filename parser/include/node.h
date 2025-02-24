#pragma once

typedef struct node {
    /// Type of node
    int type;
    /// Data of node
    void *data;
} node_t;

struct node_file {
    struct node *nodes;
};

struct node_proj {
    struct node *files;
};

struct node_var {
    char* name;
    char* type;
};

struct node_val {
    char* name;
    char* type;
};

struct node_func {
    char* name;
    char* return_type;

    struct node* args;
    struct node* body;
};


struct node_param {
    char* name;
    char* type;
};









struct node_unary {
    struct node* operand0;
};
struct node_binary {
    struct node* operand0;
    struct node* operand1;
};
struct node_ternary {
    struct node* operand0;
    struct node* operand1;
    struct node* operand2;
};

struct node_identifier {
    char* value;
};
struct node_string {
    char* value;
};
struct node_number {
    double value;
};
struct node_boolean {
    char value;
};
struct node_comment {
    char* value;
};


//service
#define NODE_TYPE_EOF                                                                                                   (0)
#define NODE_TYPE_FILE                                                                                                 (-1)
#define NODE_TYPE_PROJ                                                                                                 (-1)

//literal
///a
#define NODE_TYPE_IDENTIFIER                                                                                           (-1)
///REGEX:
#define NODE_TYPE_NUMBER                                                                                               (-1)
//REGEX
#define NODE_TYPE_STRING                                                                                               (-1)
#define NODE_TYPE_BOOLEAN                                                                                              (-1)

//declaration
///NODE: '{' NODE* '}' | ';'
#define NODE_TYPE_BODY                                                                                                (-1)

#define NODE_TYPE_TYPE                                                                                                 (-1)
#define NODE_TYPE_ENUM                                                                                                 (-1)
///NODE: func ID FUNC_ARGS ('>' ID)? BODY
#define NODE_TYPE_FUNC                                                                                                 (-1)
///NODE: var ID (':' ID)?
#define NODE_TYPE_VAR                                                                                             (99)
///NODE: val ID (':' ID)?
#define NODE_TYPE_VAL                                                                                            (100)

//branch
#define NODE_TYPE_IF
#define NODE_TYPE_ELIF
#define NODE_TYPE_ELSE
#define NODE_TYPE_SWITCH
#define NODE_TYPE_CASE

//loop
#define NODE_TYPE_FOR
#define NODE_TYPE_WHILE
#define NODE_TYPE_UNTIL

//jump
#define NODE_TYPE_NEXT
#define NODE_TYPE_BACK
#define NODE_TYPE_STOP
#define NODE_TYPE_REPEAT
#define NODE_TYPE_RETURN
#define NODE_TYPE_EJECT
#define NODE_TYPE_YIELD

//arithmetic
#define NODE_TYPE_ADDITION                                                                                             (-1)
#define NODE_TYPE_SUBTRACTION                                                                                          (-1)
#define NODE_TYPE_MULTIPLICATION                                                                                       (-1)
#define NODE_TYPE_DIVISION                                                                                             (-1)
#define NODE_TYPE_MODULO                                                                                               (-1)
#define NODE_TYPE_POWER                                                                                                (-1)
#define NODE_TYPE_INCREMENT                                                                                            (-1)
#define NODE_TYPE_DECREMENT                                                                                            (-1)

//relation
#define NODE_TYPE_EQUAL                                                                                                (-1)
#define NODE_TYPE_NOT_EQUAL                                                                                            (-1)
#define NODE_TYPE_LESS                                                                                                 (-1)
#define NODE_TYPE_LESS_OR_EQUAL                                                                                        (-1)
#define NODE_TYPE_GREATER                                                                                              (-1)
#define NODE_TYPE_GREATER_OR_EQUAL                                                                                     (-1)

//equality
/// TOKEN: eq | ==
#define NODE_TYPE_EQ                                                                                                  (620)
/// TOKEN: !=
#define NODE_TYPE_EQ_NOT                                                                                              (630)
/// TOKEN: is | ===
#define NODE_TYPE_IS                                                                                                  (640)
/// TOKEN: !==
#define NODE_TYPE_IS_NOT

//logical
#define NODE_TYPE_OR                                                                                                   (-1)
#define NODE_TYPE_AND                                                                                                  (-1)
#define NODE_TYPE_NOT

//statement


//assign
/// TOKEN: node = assign
#define NODE_TYPE_ASSIGN                                                                                              (232)
/// TOKEN: node += assign
#define NODE_TYPE_ASSIGN_PLUS                                                                                         (233)
/// TOKEN: node -= assign
#define NODE_TYPE_ASSIGN_MINUS                                                                                        (234)
/// TOKEN: node *= assign
#define NODE_TYPE_ASSIGN_MULTIPLY                                                                                     (235)
/// TOKEN: node /= assign
#define NODE_TYPE_ASSIGN_DIVIDE                                                                                       (236)
/// TOKEN: node %= assign
#define NODE_TYPE_ASSIGN_MODULUS                                                                                      (237)

