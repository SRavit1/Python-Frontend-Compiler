%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "compile.h"
#include "nodescpp.h"

int indent_level;

std::vector<function_exp*> program = {};
std::vector<expression*> main_block = {};
%}

//Note: Pointer must be used in union because sizeof(obj) is unknown
%union {
	int int_val;
	float float_val;
	bool bool_val;

	char* string_val;
	class expression* exp;
	
	class function_exp* fexp;
	std::vector<expression*>* expl;
	std::vector<std::string>* strl;
}

%start statement_list

%token DEF
%token RET

%token EQUAL

%token OPEN_PAREN
%token CLOSE_PAREN
%token COMMA
%token COLON
%token TAB

%token <string_val> IDENTIFIER
%token <int_val> INT_TOKEN
%token <float_val> FLOAT_TOKEN
%token <string_val> STRING_TOKEN
%token <int_val> BOOL_TOKEN
%token <string_val> OP

%type <exp> statement
%type <fexp> function_declaration

%type <exp> assignment
%type <exp> function_call
%type <exp> return

%type <expl> args_list
%type <strl> param_list
%type <expl> body

%type <exp> expression
%type <exp> binary_expression
%type <exp> variable

//TODO: Precedence is not working
/*
%left '-' '+'
%left '*' '/'
*/
%%
statement_list: top_statement | statement_list top_statement;

top_statement:
	statement { main_block.push_back($1); }
	| function_declaration { program.push_back($1); }
	| "\n" top_statement {}
	;

statement:
	assignment { $$ = $1; }
	| function_call { $$ = $1; }
	| return { $$ = $1; }
	/*TODO: should not be called at top level*/
	/*TODO: Add while loop, control statement_list*/
	;

assignment:
	IDENTIFIER EQUAL expression { $$ = $3; $$->setName($1); }
	;

function_call:
	IDENTIFIER OPEN_PAREN args_list CLOSE_PAREN
		{
			function_call* fc = new function_call("fun", $1);
			for (auto arg: *$3)
				fc->addExpression(arg);
			$$ = fc;
		}
	;
args_list:
	expression
		{
			std::vector<expression*>* expl = new std::vector<expression*>();
			expl->push_back($1);
			$$ = expl;
		}
	| args_list COMMA expression
		{			
			$1->push_back($3);
			$$=$1;
		}
	;
return:
	RET expression
		{
			$$ = new return_statement("ret", $2);
		}
	;

function_declaration:
	DEF IDENTIFIER OPEN_PAREN CLOSE_PAREN COLON body
		{
			//TODO: Don't know ret type at this point
			std::vector<int> arg_types;
			std::vector<std::string> arg_names;
			
			$$ = new function_exp ($2, arg_types, arg_names, 0, $6);
		}
	| DEF IDENTIFIER OPEN_PAREN param_list CLOSE_PAREN COLON body
		{
			//TODO: Don't know ret type at this point -- see above
			std::vector<int> arg_types($4->size());
			
			$$ = new function_exp ($2, arg_types, *$4, 0, $7);
		}
	;

param_list:
	IDENTIFIER
		{
			std::vector<std::string>* ident = new std::vector<std::string>;
			ident->push_back($1);
			$$ = ident;
		}
	| param_list COMMA IDENTIFIER
		{
			$$ = $1;
			$$->push_back($3);
		}
	;	

body:
	TAB statement
		{
			$$ = new std::vector<expression*>;
			$$->push_back($2);
		}
	| body TAB statement
		{
			$$ = $1;
			$$->push_back($3);
		}
	;

expression: 
	OPEN_PAREN expression CLOSE_PAREN { $$ = $2; }
	| binary_expression { $$ = $1; }
	| variable { $$ = $1; }
	| function_call { $$ = $1; }
	| INT_TOKEN {
		$$ = new integer_const("", $1);
	}
	| FLOAT_TOKEN {
		$$ = new float_const("", $1);
	}
	| STRING_TOKEN {
		$$ = new string_const("", $1);
	}
	| BOOL_TOKEN {
		$$ = new bool_const("", $1);
	}
	;

binary_expression:
	expression OP expression
		{
			$$ = new binary_expression("", $2, $1, $3);
		}
	;

variable:
	IDENTIFIER
		{
			variable* var = new variable($1);
			$$ = var;
		}
	;
%%

int main (void) {
	#ifndef YYDEBUG
	yydebug = 1;
	#endif
	
	int result = yyparse();

	//Add return to main_block if not present
	//Add return types for function args
	//Add return type for functions
	function_exp main("main", {}, {}, 0, &main_block);
	program.push_back(&main);

	compile(program);

	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}