#include <ctype.h>

#include "stdlib.h"
#include "stdio.h"
#include "parser/lexer.h"
// int lex(const char* src);
// int llex(const char* src) {
// 	int n = 0;
//
// 	while(*src) {
// 		if (isdigit(*src)) {
// 			printf("digit: [");
// 			while (*src && isdigit(*src)) {
// 				putchar(*src++);
// 			}
// 			printf("]\n");
// 		}
// 		else if (isalpha(*src) || isdigit(*src) || *src == '_') {
// 			printf("id: [");
// 			while (*src && (isalpha(*src) || isdigit(*src) || *src == '_')) {
// 				putchar(*src++);
// 			}
// 			printf("]\n");
// 		}
// 		else if (*src == '"') {
// 			printf("string: [");
// 			src++; //skip "
// 			while (*src && *src != '"') {
// 				if (*src != '\\') {
// 					putchar(*src++);
// 					continue;
// 				}
//
// 				switch (*++src) {
// 					case '\"':
// 						putchar('\"');
// 					break;
//
// 					case '\\':
// 						putchar('\\');
// 					break;
//
// 					default:
// 						printf("[unkw:%c]", *src);
// 						break;
// 				}
// 				src++;
// 			}
// 			src++; //skip "
// 			printf("]\n");
// 		}
// 		else {
// 			switch (*src) {
// 				case '+': {
// 					if (*(src+1) == '+') {
// 						printf("increment: [++]\n");
// 						src++;
// 					}
// 					else {
//
// 						printf("plus: [+]\n");
// 					}
// 					break;
// 				}
// 				case '-': {
// 					if (*(src+1) == '-') {
// 						printf("decrement: [--]\n");
// 						src++;
// 					}
// 					else {
// 						printf("minus: [-]\n");
// 					}
// 					break;
// 				}
// 				case '*': {
//
// 					printf("asterisk: [+]\n");
// 					break;
// 				}
// 				case '/': {
// 					printf("slash: [+]\n");
// 					break;
// 				}
// 				case ' ': {
// 					printf("space: [ ]\n");
// 					break;
// 				}
// 				default: {
// 					printf("[unkw:%c]\n", *src);
// 					break;
// 				}
// 			}
//
// 			src++;
// 		}
// 		n++;
// 	}
//
// 	return n;
// }

