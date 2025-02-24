#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser/include/lexer.h"
#include "parser/include/parser.h"

void main(void) {
    char *src = malloc(sizeof *src * 1024);

    FILE *file = fopen("/home/danil/Projects/dfy_lang/_dfy/samples/main.dfy", "r");
    if (file == NULL) {
        printf("File not found\n");
        return;
    }
    fread(src, sizeof(char), 1024, file);
    fclose(file);




    lexer_t lexer = (lexer_t) { .chars = src, .seek = 0, .column = 0, .row = 0 };
    token_t *tokens = lex(&lexer);
    parser_t parser = (parser_t) { .seek = 0, .tokens = tokens };
    node_t *node = parse(&parser);

    free(src);
    free(tokens);
    free(node);
}