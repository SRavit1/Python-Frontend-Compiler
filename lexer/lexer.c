#include <stdio.h>
#include "tokens.h"

extern int yylex();
extern char* yytext;

//TODO: Add rest of the tokens to here

int main(void) {
	int token = yylex();
	switch(token) {
		case UNKNOWN:
			printf("Unknown");
			break;
		case INTEGER:
			printf("Integer %s", yytext);
			break;
		case FLOAT:
			printf("Float %s", yytext);
			break;
		default:
			printf("shouldn't even get here");
	}
	return 0;
}
