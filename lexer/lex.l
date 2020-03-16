%{
#include "tokens.h"
%}
%option nounput yylineno

/*https://docs.python.org/3/reference/lexical_analysis.html*/
/*@ is both a delimiter and operator, right now grouped with delimiter*/
/*TODO: for backslash, should \ be written or \\?*/
/*TODO: add keywords*/
/*TODO: faulty rules on following
19,20,22,23,36,38,39,40,44,47,48,51,52,53,54,56,58,59,60,61,62,63,64,66,69,70,73,75,76,82
*/

%%

\n								return NEWL;
\t								return TAB;

[a-zA-Z_][a-zA-Z_0-9]*			return IDENTIFIER;

(-? | +?)[1-9][0-9]*			return INTEGER;
(-? | +?)[0-9]*\.[0-9]*			return FLOAT;

(-? | +?)[1-9][0-9]*j			return IMG_INTEGER;
(-? | +?)[0-9]*\.[0-9]*j		return IMG_FLOAT;

\(								return OPEN_PAREN;
\)								return CLOSE_PAREN;
\[								return OPEN_BRACKET;
\]								return CLOSE_BRAKCET;
{								return OPEN_BRACKET;
,								return COMMA;
:								return COLON;
.								return PERIOD;
;								return SEMICOLON;
@								return AT;
=								return EQUAL;
->								return ANNOTATION;
+=								return ADD_ASSIGN;
-=								return SUB_ASSIGN;
*=								return MUL_ASSIGN;
/=								return DIV_ASSIGN;
//=								return FLOOR_DIV_ASSIGN;
\%=								return MOD_ASSIGN;
@=								return AT_ASSIGN;
&=								return AND_ASSIGN;
|=								return OR_ASSIGN;
^=								return XOR_ASSIGN;
>>=								return BRS_ASSIGN;
<<=								return BLS_ASSIGN;
**=								return POW_ASSIGN;


'								return SINGLE_QUOT;
\"								return DOUBLE_QUOT;
#								return HASH;
\								return BACKSLASH;

=								return EQUAL;

+								return ADD;
-								return SUB;
*								return STAR;
**								return POW;
/								return DIV;
//								return FLOOR_DIV;
\%								return MOD;

<<								return BLS;
>>								return BRS;
&								return AND;
|								return OR;
^								return XOR;
~								return BIN_COMP;
:=								return ASSGN_EXP;
<								return LESS_THAN;
>								return GREATER_THAN;
<=								return LESS_EQUAL_TO;
>=								return GREATER_EQUAL_TO;
==								return EQUAL_TO;
!=								return NOT_EQUAL_TO;

.								return UNKNOWN;

%%

int yywrap(void) {
	return 1;
}
