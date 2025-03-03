#pragma once

#include "ast.h"

typedef struct Analyzer {
    AST_t *ast;
    Scope_t *scope;
} Analyzer_t;

int Analyze(Analyzer_t *analyzer);