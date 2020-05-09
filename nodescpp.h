#pragma once
//TODO: Figure out which includes are needed
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ADT/StringRef.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

//const?
//purpose of declaring name again in each derived class
//does problem of object slicing occur when 
//	a particular derived class is stored in expression?
//what is a namespace?

	class expression {
      std::string name;
		public:
      explicit expression(llvm::StringRef name) : name(name) {}
			virtual ~expression() = default;

			virtual Value *codegen() = 0;
			virtual const char *getType() = 0;
			virtual const char *getName() { return name.c_str(); }
			virtual void setName(llvm::StringRef newName) { name = newName; }
	};

	class binary_expression : public expression {
    static constexpr const char *ExpType = "binary_expression";
    std::string Op;
		expression* LHS;
		expression* RHS;

		public:
			binary_expression(llvm::StringRef name, llvm::StringRef Op, expression* LHS, expression* RHS) : expression(name), Op(Op), LHS(LHS), RHS(RHS) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class variable : public expression {
    static constexpr const char *ExpType = "variable";
		public:
			variable(llvm::StringRef name) : expression(name) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	//TODO: figure out how to store unnamed variable into IR (e.g. "%3 = i32 52")
	//TODO: for integer, float, string, bool const, add "char* name" field
	class integer_const : public expression {
    static constexpr const char *ExpType = "integer_const";
		int val;
		public:
			integer_const(llvm::StringRef name, int val) : expression(name), val(val) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class float_const : public expression {
    static constexpr const char *ExpType = "float_const";
		float val;
		public:
			float_const(llvm::StringRef name, int val) : expression(name), val(val) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class string_const : public expression {
    static constexpr const char *ExpType = "string_const";
		std::string val;
		public:
			string_const(llvm::StringRef name, llvm::StringRef val) : expression(name), val(val) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class bool_const : public expression {
    static constexpr const char *ExpType = "bool_const";
		bool val;
		public:
			bool_const(llvm::StringRef name, bool val) : expression(name), val(val) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class function_call : public expression {
    static constexpr const char *ExpType = "function_call";
		std::string function_name;
		std::vector<expression*> args;
		public:
			function_call(llvm::StringRef name, llvm::StringRef function_name) : expression(name), function_name(function_name) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
      void addExpression(expression *exp) { args.push_back(exp); }
	};

	class return_statement : public expression {
    static constexpr const char *ExpType = "return_statement";
		expression* return_val;
		public:
			return_statement(llvm::StringRef name, expression* return_val) : expression(name), return_val(return_val) {}
			Value *codegen() override;
			virtual const char *getType() { return ExpType; };
	};

	class function_exp : public expression {
    static constexpr const char *ExpType = "function_exp";
    std::string function_name;
		std::vector<int> arg_types;
		std::vector<std::string> arg_names;
		int ret_type;
		std::vector<expression*> body;
		public:
                 function_exp(llvm::StringRef name, llvm::StringRef function_name,
                              std::vector<int> arg_types,
                              std::vector<std::string> arg_names, int ret_type,
                              std::vector<expression *> body)
                     : expression(name),
                       function_name(function_name),
                       arg_types(arg_types),
                       arg_names(arg_names),
                       ret_type(ret_type),
                       body(body) {}
                 Function *codegen();
                 virtual const char *getType() { return ExpType; };
	};
