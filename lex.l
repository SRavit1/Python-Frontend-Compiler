%{
#include <stdlib.h>
#include <string>
#include <vector>
#include "parser.tab.h"
void yyerror (char *s);
int yylex();
%}

/*https://docs.python.org/3/reference/lexical_analysis.html*/
/*TODO: add keywords*/

%%
\"\"\"[a-z]*\"\"\"				;


(-+)?[1-9][0-9]*				{yylval.int_val = atoi(yytext); 
								return INT_TOKEN;}
(-+)?[0-9]*\.[0-9]+ |
(-+)?[0-9]+\.[0-9]*				{yylval.float_val = atof(yytext);
								return FLOAT_TOKEN;}

True							{yylval.bool_val = 1; return BOOL_TOKEN;}
False							{yylval.bool_val = 0; return BOOL_TOKEN;}

\"[a-zA-Z_0-9]*\"				{yylval.string_val = strdup(yytext); return STRING_TOKEN;}

"="								return EQUAL;
"\n"							return NEWL;

def								return DEF;
return							return RET;

[a-zA-Z_][a-zA-Z_0-9]*			{
								yylval.string_val = strdup(yytext);
								return IDENTIFIER;}
%%

int yywrap (void) {return 1;}