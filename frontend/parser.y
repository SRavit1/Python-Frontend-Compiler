%{
void yyerror (char *s);
int yylex();
#include <stdio.h>
#include "nodes.h"

struct statement_link* curr_statement;
curr_statement = malloc(sizeof(curr_statement));
struct statement_link head;

int head_defined = 0;

void add (struct statement_link* ptr, union statement statement) {
	ptr = malloc(sizeof(ptr));
	ptr->current_statement = statement;
	ptr = ptr->next_statements;
};
%}

%union {
	int int_val;
	float float_val;
	struct int_exp* int_exp_val;
}
%start statement
%token <int_val> INTEGER
%token <float_val> FLOAT
%token ADD
%token PRINT
%token OPEN_PAREN
%token CLOSE_PAREN
%type <int_exp_val> int_expression

%%
statement :
	PRINT OPEN_PAREN int_expression CLOSE_PAREN
		{
			int type = $3->type;
			if (type==0)			
				printf("Simple integer %d", $3->int_exp_val.int_val);
			if (type==3)
				printf("Add expression");
		}
	;

int_expression :
	INTEGER
		{
			struct int_exp ie = malloc(sizeof(ie));
			ie.int_const = $1;
			
			$$ = malloc(sizeof($$));
			$$->type = 0;		
			$$->int_exp_val.int_val = ie;
		}
	| int_expression ADD int_expression
		{
			struct add_int ai = malloc(sizeof(ai));
			ai.operand1 = $1;
			ai.operand2 = $3;
			
			$$ = malloc(sizeof($$));
			$$->type = 3;
			$$->int_exp_val.add_int_val = ai;
		}
	;
%%

int main (void) {
	/* init symbol table */
	return yyparse ();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

