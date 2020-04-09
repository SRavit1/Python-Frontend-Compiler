all : y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -ly -ll

debug : y.tab.c lex.yy.c
	gcc -g y.tab.c lex.yy.c -ly -ll

y.tab.c : lex.yy.c parser.y
	yacc -d parser.y

lex : tokenizer.c lex.yy.c
	gcc tokenizer.c lex.yy.c

lex-debug : lexer.c lex.yy.c
	gcc -g lexer.c lex.yy.c

lex.yy.c : lex.l
	lex lex.l

clean :
	rm lex.yy.c a.out y.tab.*
