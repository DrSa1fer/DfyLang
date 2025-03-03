#pragma once

enum IRInstructionType {
    IR_INST_NOP,

    IR_INST_ADD,
    IR_INST_SUB,
    IR_INST_MUL,
    IR_INST_DIV,
    IR_INST_MOD,

    IR_INST_JMP,
    IR_INST_JEQ,
    IR_INST_JNE,
    IR_INST_JG,
    IR_INST_JGT,
    IR_INST_JL,
    IR_INST_JLE,
};

typedef struct IRInstruction {
    enum IRInstructionType type;
    char *operand0;
    char *operand1;
    char *result;
} IRInstruction_t;