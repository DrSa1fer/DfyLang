cmake_minimum_required(VERSION 3.30)
project(dfy_lang C)

set(CMAKE_C_STANDARD 17)

add_executable(dfy_lang
    main.c parser/lexer.c
        parser/parser.c
        parser/include/node.h
        parser/include/table.h)
