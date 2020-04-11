%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>

#include "nodes.h"
#include "ast_utils.h"
%}

%union {
	int int_val;
	float float_val;
	struct int_exp* int_expression;
	struct expression* exp;
}

%start statement_list

%token <int_val> INTEGER
%token <float_val> FLOAT

%token ADD
%token SUB
%token MUL
%token POW
%token DIV
%token FLOOR_DIV
%token MOD

%token OPEN_PAREN
%token CLOSE_PAREN
%token NEWL

%token PRINT

%type <int_expression> int_expression
%type <exp> expression

%%
statement_list: statement NEWL | statement_list statement NEWL;

statement :
	PRINT OPEN_PAREN expression CLOSE_PAREN
		{
			struct print_statement* print_struct = malloc(sizeof(*print_struct));
			print_struct->print_content = *$3;

			struct statement* curr_statement = malloc(sizeof(*curr_statement));
			curr_statement->type = 1;
			curr_statement->statement_val.print_statement_s = *print_struct;
			
			add (curr_statement);
		}
	;

expression : 
	OPEN_PAREN expression CLOSE_PAREN
		{
			$$ = $2;
		}
	| int_expression
		{
			$$ = malloc(sizeof(*$$));
			$$->type = 1;
			$$->expression_val.int_exp_val = *$1;
		}
	;

//TODO: Figure out operator precedence
int_expression :
	OPEN_PAREN int_expression CLOSE_PAREN
		{
			$$ = $2;
		}
	| INTEGER
		{
			$$ = malloc(sizeof(*$$));
			$$->type = 1;		
			$$->int_exp_val.int_val = $1;
		}
	| int_expression ADD int_expression
		{
			struct int_arith* add_struct = malloc(sizeof(*add_struct));
			add_struct->type = 1;
			add_struct->operand1 = $1;
			add_struct->operand2 = $3;
			
			$$ = malloc(sizeof(*$$));
			$$->type = 4;
			$$->int_exp_val.int_arith_val = *add_struct;
			//TODO: is ai being copied?
			//If so, this shouldn't be happening as it takes memory
		}
	;
%%


int main (void) {
	int result = yyparse ();
	print_ast(head);
	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
