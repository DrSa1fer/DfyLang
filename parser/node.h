#pragma once

struct node;
typedef struct node node_t;

struct node {
    int type;
    void* value;
};

struct node_number {
    double value;
};

struct node_unary_operation {
    struct node* operand0;
};
struct node_binary_operation {
    struct node* operand0;
    struct node* operand1;
};
struct node_ternary_operation {
    struct node* operand0;
    struct node* operand1;
    struct node* operand2;
};

struct node_identifier {
  char* identifier;
};

struct node_var {

};
struct node_var_dec {
    char* name;
};

struct node_assign {
    struct node* target;
    struct node* value;
};