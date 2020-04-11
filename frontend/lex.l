%{
#include "y.tab.h"
void yyerror (char *s);
int yylex();
%}

/*https://docs.python.org/3/reference/lexical_analysis.html*/
/*TODO: add keywords*/

%%

(-+)?[1-9][0-9]*				{yylval.int_val = atoi(yytext); 
								return INTEGER;}
(-+)?[0-9]*\.[0-9]+ |
(-+)?[0-9]+\.[0-9]*				{yylval.float_val = atof(yytext);
								return FLOAT;}

\+								return ADD;
\-								return SUB;
\*								return MUL;
\**								return POW;
\/								return DIV;
\/\/							return FLOOR_DIV;
\%								return MOD;

\(								return OPEN_PAREN;
\)								return CLOSE_PAREN;

\n								return NEWL;

print							return PRINT;

.								;
%%

int yywrap (void) {return 1;}

/*
\n								return NEWL;
\t								return TAB;

[a-zA-Z_][a-zA-Z_0-9]*			return IDENTIFIER;

(-+)?[1-9][0-9]*				return INTEGER;
(-+)?[0-9]*\.[0-9]+ |
(-+)?[0-9]+\.[0-9]*				return FLOAT;

\(								return OPEN_PAREN;
\)								return CLOSE_PAREN;
\[								return OPEN_BRACKET;
\]								return CLOSE_BRAKCET;
\{								return OPEN_BRACE;
\}								return CLOSE_BRACE;

,								return COMMA;
:								return COLON;
\.								return PERIOD;
;								return SEMICOLON;
=								return EQUAL;

\+=								return ADD_ASSIGN;
-=								return SUB_ASSIGN;
\*=								return MUL_ASSIGN;
\*\*=							return POW_ASSIGN;
\/=								return DIV_ASSIGN;
\/\/=							return FLOOR_DIV_ASSIGN;
\%=								return MOD_ASSIGN;

'								return SINGLE_QUOT;
\"								return DOUBLE_QUOT;
#								return HASH;
\\								return BACKSLASH;

\<								return LESS_THAN;
>								return GREATER_THAN;
\<=								return LESS_EQUAL_TO;
>=								return GREATER_EQUAL_TO;
==								return EQUAL_TO;
!=								return NOT_EQUAL_TO;
*/


