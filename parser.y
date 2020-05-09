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
std::vector<expression*> current_block = {};
%}

%union {
	int int_val;
	float float_val;
	bool bool_val;

	char* string_val;
	class expression* exp; //Problem: Object slicing occurs when assigning derived class to base class
	class function_exp* fexp;
	std::vector<expression*>* expl;
	std::vector<std::string>* strl;
}

%start statement_list

%token DEF
%token RET

%token EQUAL
%token <string_val> NEWL

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

%%

statement_list: top_statement NEWL | statement_list top_statement NEWL;

top_statement:
	statement { main_block.push_back($1); }
	| function_declaration { program.push_back($1); }
	;

statement:
	assignment {
		$$ = $1;
	}
	| function_call { $$ = $1; }
	| return { $$ = $1; }
	/*TODO: should not be called at top level*/
	/*TODO: Add while loop, control statement_list*/
	;

assignment:
	IDENTIFIER EQUAL expression
		{
			$$ = $3;
		}
	;

function_call:
	IDENTIFIER "(" args_list ")"
		{
      function_call *FC = new function_call("fun", $1);
      for (auto Exp : *$3)
        FC->addExpression(Exp);
			$$ = FC;
		}
	;
args_list:
	expression
		{
			std::vector<expression*> *expl = new std::vector<expression*>();
			expl->push_back($1);
			$$ = expl;
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
			$$ = new return_statement("ret", $2);
		}
	;

function_declaration:
	DEF IDENTIFIER "(" param_list ")" ":" "\n" "{" body "}"
		{
			//TODO: Don't know ret type at this point
			char* function_name = $2;
			std::vector<int> arg_types($4->size());
			std::vector<std::string> arg_names = *$4;
			int ret_type = 0;
			std::vector<expression*> body = *$9;
			
			$$ = new function_exp("func", function_name, arg_types, arg_names, ret_type, body);
		}
	;

param_list:
	param_list "," IDENTIFIER
		{
			$$ = $1;
			$$->push_back($3);
		}
	| IDENTIFIER
		{
			std::vector<std::string> *ident = new std::vector<std::string>();
			ident->push_back($1);
			$$ = ident;
		}

	;	

body:
	statement "\n" body
		{
			current_block.push_back($1);
		}
	| statement "\n"
		{
			current_block.push_back($1);
		}

	;

expression: 
	"(" expression ")" { $$ = $2; }
	| binary_expression { $$ = $1; }
	| variable { $$ = $1; }
	| function_call { $$ = $1; }
	| INT_TOKEN {
		$$ = new integer_const("int", $1);
	}
	| FLOAT_TOKEN {
		$$ = new float_const("float", $1);
	}
	| STRING_TOKEN {
		$$ = new string_const("str", $1);
	}
	| BOOL_TOKEN {
		$$ = new bool_const("bool", $1);
	}
	;

binary_expression:
	expression OP expression
		{
			$$ = new binary_expression("binexp", $2, $1, $3);
		}
	;

variable:
	IDENTIFIER
		{
			$$ = new variable($1);
		}
	;
%%

int main (void) {
#ifdef YYDEBUG
  yydebug = 1;
#endif
	std::string main_name = "main";

	int result = yyparse ();
	
	for (expression* statement : main_block) {
		std::cout << "Statement " << statement->getType() << "\n";
  }
  std::vector<int> argT;
  std::vector<std::string> argN;
  argT.push_back(0);
  argN.emplace_back("var");

  // You need to make sure you are moving main_block after parsing. main_block
  // is invalidated after std::move.
  function_exp main("fun", main_name.c_str(), argT, argN, 0, main_block);
	program.push_back(&main);

  compile(program);

  return result;
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
