#pragma once
#include <stdlib.h>

struct table;
typedef struct table table_t;

struct table_var {
    char *name;
    char *type;
    void *value;
};
struct table_val {
    char *name;
    char *type;
    void *value;
};
struct table_func {
};
struct table_type {

};

/// Nametable
struct table {
    struct table_func *funcs;
    size_t funcs_size;

    struct table_type *types;
    size_t types_size;

    struct table_var  *vars;
    size_t vars_size;

    struct table_val  *vals;
    size_t vals_size;

    struct table *parent;
};

struct table_type *find_type (struct table *table, const char *name);
struct table_func *find_func (struct table *table, const char *name);
struct table_var  *find_var  (struct table *table, const char *name);
struct table_val  *find_val  (struct table *table, const char *name);

int append_type (struct table *table, struct table_type *item);
int append_func (struct table *table, struct table_func *item);
int append_var  (struct table *table, struct table_var  *item);
int append_val  (struct table *table, struct table_val  *item);

void push_scope (struct table *table);
void pop_scope  (struct table *table);
