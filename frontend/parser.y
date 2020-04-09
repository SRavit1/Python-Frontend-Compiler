%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"

struct statement_link* body;
struct statement_link head;

int head_defined = 0;

void add (struct statement* ptr) {
	body->current_statement = *ptr;
	
	body = body->next;
	body = malloc(sizeof(body));
}

void check_head() {
	if (!head_defined) {
		head = *body;
		//TODO: Is head being copied?
		head_defined = 1;
	}
}

%}

%union {
	int int_val;
	float float_val;
	struct int_exp* int_expression;
}
%start statement
%token <int_val> INTEGER
%token <float_val> FLOAT
%token ADD
%token PRINT
%token OPEN_PAREN
%token CLOSE_PAREN
%type <int_expression> int_expression

%%
statement :
	PRINT OPEN_PAREN int_expression CLOSE_PAREN	
		{
			struct print_statement* print_struct = malloc(sizeof(*print_struct));
			print_struct->print_content = ($3->int_exp_val).int_val;

			struct statement* curr_statement = malloc(sizeof(*curr_statement));
			curr_statement->type = 1;
			curr_statement->statement_val.print_statement_s = *print_struct;
			
			//add (curr_statement);
			//check_head();

			if (!head_defined) {
				body = malloc(sizeof(*body));
			}	
			
			body->current_statement = *curr_statement;
			body->defined = 1;

			if (!head_defined) {
				head = *body;
				head_defined = 1;
			}

			body->next = malloc(sizeof(*body));
			body = body->next;
			body->defined = 0;

			if ($3->type==1) {printf("Simple integer %d", $3->int_exp_val.int_val);}
			if ($3->type==4) {printf("Add expression");}
		}
	;

int_expression :
	INTEGER
		{
			$$ = malloc(sizeof(*$$));
			$$->type = 1;		
			$$->int_exp_val.int_val = $1;
		}
	| int_expression ADD int_expression
		{
			struct add_int* ai = malloc(sizeof(*ai));
			ai->operand1 = $1;
			ai->operand2 = $3;
			
			$$ = malloc(sizeof(*$$));
			$$->type = 4;
			$$->int_exp_val.add_int_val = *ai;
			//TODO: is ai being copied?
			//If so, this shouldn't be happening as it takes memory
		}
	;
%%

void print_ast() {
	struct statement_link* sl_i = &head;
	while (sl_i && sl_i->defined) {
		struct statement statement_i = head.current_statement;
		printf("new statement");
		sl_i = sl_i->next;
	}
}

int main (void) {
	/* init symbol table */
	int result = yyparse ();
	print_ast();
	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 
