cmake_minimum_required(VERSION 3.30)
project(dfy_lang C)

set(CMAKE_C_STANDARD 17)

add_executable(dfy_lang
    main.c
        parser/include/token.h
        parser/include/tokenizer.h
        parser/tokenizer.c

        parser/include/ast.h
        parser/parser.c
        parser/analyzer.c

        compiler/include/ir_instruction.h
        compiler/include/ir_generator.h
        compiler/include/ir_optimizer.h
        compiler/ir_generator.c
        compiler/ir_optimizer.c
        parser/include/parser.h
        parser/include/analyzer.h
)