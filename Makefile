all : main.h codegen.h checker.h optimizer.h nodescpp.h lex.yy.c parser.tab.c parser.tab.h
	g++ -g -O0 parser.tab.c lex.yy.c -ll -ly `llvm-config --cxxflags --ldflags --system-libs --libs core`

lex.yy.c : lex.l
	lex lex.l

parser.tab.c : parser.y
	bison --debug -d parser.y

parser.tab.h : parser.y
	bison --debug -d parser.y

clean : 
	rm lex.yy.* parser.tab.* a.out
