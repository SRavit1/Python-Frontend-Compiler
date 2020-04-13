# Python-Frontend-Compiler
Compiler to convert Python source code to LLVM IR.

# Goals
The motivation behind this project is to create a frontend for a subset of the Python language. This would enable conversion of Python files to executable for any architecture supported by LLVM. To learn more, please visit my blog: <https://siliconvalley.basisindependent.com/author/ravits/>. Throughout this project, I have also kept a record of my notes and documentation for this compiler, which you can find at the following link: <https://docs.google.com/document/d/1-tF2z3V0TJ14ryqjZQjaMmmGkJGZyR_3b_XSsQzlpjY/edit?usp=sharing>.

# Supported Features of Python
The print command, function declarations/calls, if/elif/else control structures, while loops, and basic primitive expressions are supported.

# Installation and Usage
Coming soon!!

# Steps Overview
There are three main phases of this project, as documented below.
1. Lexer: Identifying Tokens
1. Parser: Converting tokens to AST
1. AST Codegen

I will keep these explanations concise on this README. For more detail, please take a look at my documentation.
## Lexer
Function of this phase is to break down the source code into individual tokens. This requires defining the supported tokens to be identified, and their associated regular expression in the file lex.l. The lex command is used to create the lexer lex.yy.c

## Parser
Function of this phase is to provide semantic analysis on the tokens to create a parse tree. The pattern of tokens for statements/expressions is specified in the file parser.y. yacc parser generator is used to create the parser files y.tab.c and y.tab.h. Node definitions for the AST are provided in nodes.h and the output of this phase is the AST. AST print utils present in ast\_utils.h.

## AST Codegen
Function of this phase is to build the LLVM IR file from the AST. The LLVM C++ API is used to insert LLVM statements into the IR from the AST.
