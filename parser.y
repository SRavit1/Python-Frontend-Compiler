%{
void yyerror (char *s);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "main.h"
#include "nodescpp.h"

std::vector<function_exp*> program = {};
std::vector<expression*> main_block = {};
std::vector<expression*>* current_block = {};
%}

%union {
	int int_val;
	float float_val;
	bool bool_val;
	char char_val;

	std::string* string_val;
	class expression* exp; //Problem: Object slicing occurs when assigning derived class to base class
	class function_exp* fexp;
	std::vector<expression*>* expl;
	std::vector<std::string>* strl;
}

%start statement_list

%token DEF
%token RET

%token EQUAL
%token NEWL

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
%type <exp> integer
%type <exp> float
%type <exp> string
%type <exp> bool

%%
statement_list: top_statement NEWL | statement_list top_statement NEWL;

top_statement:
	statement { main_block.push_back($1); }
	| function_declaration { program.push_back($1); }
	;

statement:
	assignment { $$ = $1; }
	| function_call { $$ = $1; }
	| return { $$ = $1; }
	/*TODO: should not be called at top level*/
	/*TODO: Add while loop, control statement_list*/
	;

assignment:
	IDENTIFIER EQUAL expression
		{
			$$ = $3;
			std::cout<<"Assignment pointer " << $$ << std::endl;

			std::cout<< $$->getName() << std::flush;
			//$$->setName("something");
		}
	;

function_call:
	IDENTIFIER "(" args_list ")"
		{
			$$ = new function_call(*$1, *$3);
		}
	;
args_list:
	expression
		{
			std::vector<expression*> expl;
			expl.push_back($1);
			$$ = &expl;
		}
	| args_list "," expression
		{
			$$=$1;
			$$->push_back($3);
		}
	;
return:
	RET expression
		{
			$$ = new return_statement($2);
		}
	;

function_declaration:
	DEF IDENTIFIER "(" param_list ")" ":" "\n" "{" body "}"
		{
			//TODO: Don't know ret type at this point
			std::string function_name = *$2;
			std::vector<int> arg_types($4->size());
			std::vector<std::string> arg_names = *$4;
			int ret_type = 0;
			std::vector<expression*> body = *$9;
			$$ = new function_exp(function_name, arg_types, arg_names, ret_type, body);
		}
	;

param_list:
	param_list "," IDENTIFIER
		{
			$$ = $1;
			$$->push_back(*$3);
		}
	| IDENTIFIER
		{
			std::vector<std::string> ident;
			ident.push_back(*$1);
			$$ = &ident;
		}

	;	

body:
	statement "\n" body
		{
			current_block->push_back($1);
		}
	| statement "\n"
		{
			current_block->push_back($1);
		}

	;

expression: 
	"(" expression ")" { $$ = $2; }
	| binary_expression { $$ = $1; }
	| variable { $$ = $1; }
	| function_call { $$ = $1; }
	| integer { $$ = $1; 
	std::cout<< "Pointer to exp " << $$ << std::endl << std::flush;
	std::cout<< "Name of exp " << $$->getName() << std::endl << std::flush; }
	| float { $$ = $1; }
	| string { $$ = $1; }
	| bool { $$ = $1; }
	;

binary_expression:
	expression OP expression
		{
			$$ = new binary_expression($2, $1, $3);
		}
	;

variable:
	IDENTIFIER
		{
			$$ = new variable(*$1);
		}
	;
integer:
	INT_TOKEN
		{
			//integer_const* int_tok = new integer_const($1);
			integer_const int_tok($1);
			$$ = &int_tok;

			$$->setName("int_name");
		}
	;
float:
	FLOAT_TOKEN
		{
			$$ = new float_const($1);
		}
	;
string:
	STRING_TOKEN
		{
			$$ = new string_const(*$1);
		}
	;
bool:
	BOOL_TOKEN
		{
			$$ = new bool_const($1);
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
	function_exp* main = new function_exp("main", {}, {}, 0, main_block);

	current_block = &main_block;
	program.push_back(main);

	int result = yyparse ();
	
	compile(program);

	return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
