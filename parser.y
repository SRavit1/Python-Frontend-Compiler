%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "main.h"
#include "nodescpp.h"

std::vector<function_exp>* program = new std::vector<function_exp>;
std::vector<expression>* main_block = new std::vector<expression>;
std::vector<expression>* current_block = new std::vector<expression>;
%}

%union {
	int int_val;
	float float_val;
	bool bool_val;
	char char_val;

	char* string_val;
	class expression* exp;
	//Pointer must be used in union because sizeof(exp) is unknown
	//TODO: Look into smart pointers as better alternative
	//Problem: Object slicing occurs when assigning derived class to base class
	class function_exp* fexp;
	std::vector<expression>* expl;
	std::vector<std::string>* strl;
}

%start statement_list

%token DEF
%token RET

%token EQUAL
%token NEWL

%token OPEN_PAREN
%token CLOSE_PAREN
%token COMMA
%token COLON

%token <string_val> IDENTIFIER
%token <int_val> INT_TOKEN
%token <float_val> FLOAT_TOKEN
%token <string_val> STRING_TOKEN
%token <int_val> BOOL_TOKEN

%type <char_val> OP

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

%%

statement_list: top_statement NEWL | statement_list top_statement NEWL;

top_statement:
	statement { main_block->push_back(*$1); }
	| function_declaration { program->push_back(*$1); }
	;

statement:
	assignment { $$ = $1; }
	| function_call { $$ = $1; }
	| return { $$ = $1; }
	/*TODO: should not be called at top level*/
	/*TODO: Add while loop, control statement_list*/
	;

assignment:
	IDENTIFIER EQUAL expression { $$ = $3; }
	;

function_call:
	IDENTIFIER OPEN_PAREN args_list CLOSE_PAREN
		{
			std::string identifier_str($1);
			function_call* curr_fc = new function_call(identifier_str, *$3);

			$$ = curr_fc;
		}
	;
args_list:
	expression
		{
			std::vector<expression> expl;
			expl.push_back(*$1);
			$$ = &expl;
		}
	| args_list COMMA expression
		{
			$$=$1;
			$$->push_back(*$3);
		}
	;
return:
	RET expression
		{
			return_statement curr_rs($2);
			$$ = &curr_rs;
		}
	;

function_declaration:
	DEF IDENTIFIER OPEN_PAREN param_list CLOSE_PAREN COLON NEWL "{" body "}"
		{
			//TODO: Don't know ret type at this point
			char* function_name = $2;
			std::vector<int> arg_types($4->size());
			std::vector<std::string> arg_names = *$4;
			int ret_type = 0;
			std::vector<expression> body = *$9;
			
			function_exp curr_fe(function_name, arg_types, arg_names, ret_type, body);
			$$ = &curr_fe;
		}
	;

param_list:
	param_list COMMA IDENTIFIER
		{
			$$ = $1;
			$$->push_back($3);
		}
	| IDENTIFIER
		{
			std::vector<std::string>* ident = new std::vector<std::string>;
			ident->push_back($1);
			$$ = ident;
		}

	;	

body:
	statement NEWL body
		{
			current_block->push_back(*$1);
		}
	| statement NEWL
		{
			current_block->push_back(*$1);
		}
	;

expression: 
	OPEN_PAREN expression CLOSE_PAREN { $$ = $2; }
	| binary_expression { $$ = $1; }
	| variable { $$ = $1; }
	| function_call { $$ = $1; }
	| INT_TOKEN {
		integer_const* int_tok = new integer_const($1);
		$$ = int_tok;

		std::string newName = "happy";
		char newNameC[5] = {'h', 'a', 'p', 'p', 'y'};
		$$->setName(newNameC);
	}
	| FLOAT_TOKEN {
		float_const* float_tok = new float_const($1);
		$$ = float_tok;
	}
	| STRING_TOKEN {
		string_const* string_tok = new string_const($1);
		$$ = string_tok;
	}
	| BOOL_TOKEN {
		bool_const* bool_tok = new bool_const($1);
		$$ = bool_tok;
	}
	;

binary_expression:
	expression OP expression
		{
			binary_expression* be = new binary_expression($2, $1, $3);
			$$ = be;
		}
	;

variable:
	IDENTIFIER
		{
			variable* var = new variable($1);
			$$ = var;
		}
	;
OP:
	'+'				{$$ = '+';}
	| '-'			{$$ = '-';}
	| '*'			{$$ = '*';}
	| '/'			{$$ = '/';}
	;
%%

int main (void) {
	std::string main_name = "main";
	function_exp main(main_name.c_str(), {}, {}, 0, *main_block);

	current_block = main_block;
	program->push_back(main);

	int result = yyparse ();
	
	for (expression statement : *main_block) {
		std::cout << "Statement " << statement.getType() << "\n";
	}

	compile(*program);

	delete program;
	delete main_block;
	delete current_block;

	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}