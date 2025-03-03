#define LEXER_LOG 1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser/include/tokenizer.h"
#include "parser/include/parser.h"

void main(void) {
    char *src = malloc(sizeof *src * 1024);

    FILE *file = fopen("/home/danil/Projects/DfyLang/_dfy/samples/main.dfy", "r");
    if (file == NULL) {
        printf("File not found\n");
        return;
    }
    fread(src, sizeof(char), 1024, file);
    fclose(file);


    Tokenizer_t lexer  = (Tokenizer_t) { .chars = src, .seek = 0 };
    Token_t *tokens = Tokenize(&lexer);

    struct ASTNode *ptr = malloc(sizeof(struct ASTNode) * 10);
    free(ptr);


    Parser_t parser;
    parser.seek = 0;
    parser.tokens = tokens;

    ASTNode_t *node = Parse(&parser);

    free(src);
    // free(tokens);
    // free(node);
}