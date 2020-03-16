all : lexer.c lex.yy.c
	gcc lexer.c lex.yy.c

lex.yy.c : lex.l
	lex lex.l

clean :
	rm lex.yy.c
