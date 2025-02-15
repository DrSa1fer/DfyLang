#pragma once

struct node;
typedef struct node node_t;

struct node {
    int type;
    void* value;
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