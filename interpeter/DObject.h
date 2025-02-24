#ifndef DOBJECT_H
#define DOBJECT_H

#include <stdio.h>

struct d_object;      typedef struct d_object      d_object_t;


struct d_type;        typedef struct d_type        d_type_t;
struct d_func;        typedef struct d_func        d_func_t;
struct d_var;         typedef struct d_var         d_var_t;
struct d_val;         typedef struct d_val         d_val_t;

struct d_object {
    union {
        struct {
            size_t size;
            void *data;
        };
    };
    size_t func_count;
    size_t val_count;
    size_t var_count;

    struct d_func *funcs;
    struct d_val  *vals;
    struct d_var  *vars;
};
struct d_type {
    struct d_type *parent;

    size_t func_count;
    size_t val_count;
    size_t var_count;

    struct d_func *funcs;
    struct d_val  *vals;
    struct d_var  *vars;
};
struct d_var {
    struct d_object *value;
};
struct d_val {
    struct d_object *const value;
};

struct d_func {

};

#endif //DOBJECT_H