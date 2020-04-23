#include <stdio.h> 
#include <stdlib.h>
#include "yy.tab.h"

extern int yylex();
extern char* yytext;

struct token_str{
	int token_type;
	char* content;
	struct token_str* next_token;
};

int main(void) {
	struct token_str* current_token = malloc(sizeof(current_token));
	struct token_str head;
	int head_defined = 0;
	
	int token = yylex();
	
	if (!token) {
		return 0;
	}
	
	for(;;) {
		current_token->token_type = token;
		current_token->content = strdup(yytext);
	    current_token->next_token = NULL;
		
		token = yylex();
		
		if (!token) {
			break;
		}
		
		current_token->next_token = malloc(sizeof(current_token));
		
		if (!head_defined) {
			head = *current_token;
			head_defined = 1;
		}
		
		current_token = current_token->next_token;
	}
	
	current_token = &head;
	
	int i = 1;
	do {
		int token_type = current_token->token_type;
		char* token_content = current_token->content;
		
		printf("Token no. %d of type %d with value %s\n", i, token_type, token_content);

		current_token = current_token->next_token;
		i++;
	} while(current_token);
	return 0;
}

/*

switch(token) {
	case INTEGER:
		printf("Integer %s", yytext);
		printf("\n");
		break;
	case UNKNOWN:
		printf("Unknown token %s", yytext);
		printf("\n");
		break;		
	default:
		printf("Unexpected text: %s", yytext);
		printf("\n");
}

*/
