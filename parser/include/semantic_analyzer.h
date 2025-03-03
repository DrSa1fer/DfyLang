#pragma once

#include "ast_node.h"

typedef struct Analyzer {
    AST_t *ast;
} Analyzer_t;

int analyze(Analyzer_t *analyzer);