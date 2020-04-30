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
	char* string;
}

%start statement_list

%token <int_val> INTEGER
%token <float_val> FLOAT

%token EQUAL

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

%token <string> IDENTIFIER

%type <int_expression> int_expression
%type <exp> expression
%type <int_val> OP

%%
statement_list: statement NEWL | statement_list statement NEWL;

statement :
	PRINT OPEN_PAREN expression CLOSE_PAREN
		{
			struct print_statement* print_struct = malloc(sizeof(*print_struct));
			print_struct->print_content = *$3;

			struct statement* print_statement = malloc(sizeof(*print_statement));
			print_statement->type = 1;
			print_statement->statement_val.print_statement_s = *print_struct;
			
			add (print_statement);
		}
	| IDENTIFIER EQUAL expression
		{
			struct assignment* assign_struct = malloc(sizeof(*assign_struct));
			assign_struct->var_name = strdup($1);
			assign_struct->value = *$3;
			
			struct statement* assign_statement = malloc(sizeof(*assign_statement));
			assign_statement->type = 2;
			assign_statement->statement_val.assign_s = *assign_struct;

			add (assign_statement);
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
	| IDENTIFIER
		{
			$$ = malloc(sizeof(*$$));
			$$->type = 2;

			struct int_var* int_var_struct = malloc(sizeof(*int_var_struct));
			int_var_struct->var_name = $1;

			$$->int_exp_val.int_var_val = *int_var_struct;
		}	
	| int_expression OP int_expression
		{
			$$ = malloc(sizeof(*$$));
			$$->type = 4;

			struct int_arith* add_struct = malloc(sizeof(*add_struct));
			add_struct->type = $2;
			add_struct->operand1 = $1;
			add_struct->operand2 = $3;

			$$->int_exp_val.int_arith_val = *add_struct;
			//TODO: is ai being copied?
			//If so, this shouldn't be happening as it takes memory
		}
	| int_expression SUB int_expression
	;

OP :
	ADD				{$$=1;}
	| SUB			{$$=2;}
	| MUL			{$$=3;}
	| POW			{$$=4;}
	| DIV			{$$=5;}
	| FLOOR_DIV		{$$=6;}
	| MOD			{$$=7;}
%%


int main (void) {
	int result = yyparse ();
	print_ast(head);
	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
