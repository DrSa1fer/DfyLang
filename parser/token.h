#pragma once

struct token;
typedef struct token token_t;

struct token {
    int type;
    void *value;
};

struct token_number {};