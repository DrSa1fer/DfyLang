#pragma once
#include "ir_instruction.h"
#include "../../parser/include/ast.h"

typedef struct IRGenerator {
    AST_t *ast;

} IRGenerator_t;


IRInstruction_t *GenerateIR(IRGenerator_t *generator);