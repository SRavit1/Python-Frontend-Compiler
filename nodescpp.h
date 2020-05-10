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

namespace {	
	class expression {
		std::string name;
		public:
			explicit expression(llvm::StringRef name) : name(name) {}
			virtual ~expression() = default;
			virtual Value *codegen() = 0;
			virtual const char* getType() = 0;
			virtual const char *getName() { return name.c_str(); }
			virtual void setName(llvm::StringRef newName) { name = newName; }
	};

	class binary_expression : public expression {
		static constexpr const char* ExprType = "binary expression";
		std::string Op;

		expression* LHS;
		expression* RHS;
		public:
			binary_expression(llvm::StringRef name, llvm::StringRef Op, expression* LHS, expression* RHS) : expression(name), Op(Op), LHS(LHS), RHS(RHS) {}
			~binary_expression() { delete LHS; delete RHS; }
			
			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class variable : public expression {
		static constexpr const char* ExprType = "variable";

		public:
			variable(llvm::StringRef name) : expression(name) {}
			
			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	//TODO: figure out how to store unnamed variable into IR (e.g. "%3 = i32 52")
	//TODO: for integer, float, string, bool const, add "char* name" field
	class integer_const : public expression {
		int val;
		static constexpr const char* ExprType = "integer expression";
		
		public:
			integer_const(llvm::StringRef name, int val) : expression(name), val(val) {}
			
			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class float_const : public expression {
		float val;
		static constexpr const char* ExprType = "float expression";
		
		public:
			float_const(llvm::StringRef name, float val) : expression(name), val(val) {}

			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class string_const : public expression {
		std::string val;
		static constexpr const char* ExprType = "string expression";

		public:
			string_const(llvm::StringRef name, std::string val) : expression(name), val(val) {}
			
			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class bool_const : public expression {
		bool val;
		static constexpr const char* ExprType = "bool expression";

		public:
			bool_const(llvm::StringRef name, bool val) : expression(name), val(val) {}

			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class function_call : public expression {
		std::vector<expression*> args;
		static constexpr const char* ExprType = "function call";
		//TODO: Add variable for name of function that is called (not name of variable assigned to function call)
		std::string function_name;

		public:
			function_call(llvm::StringRef name, std::string function_name) 
				: expression(name), args(args), function_name(function_name) {}
			
			Value *codegen() override;
			virtual const char *getType() { return ExprType; }

			void addExpression(expression* exp) { args.push_back(exp); }
	};

	class return_statement : public expression {
		expression* return_val;
		static constexpr const char* ExprType = "return statement";

		public:
			return_statement(llvm::StringRef name, expression* return_val) : expression(name), return_val(std::move(return_val)) {}
			~return_statement() { delete return_val; }

			Value *codegen() override;
			virtual const char *getType() { return ExprType; }
	};

	class function_exp : public expression {
		std::vector<int> arg_types;
		std::vector<std::string> arg_names;
		int ret_type;
		std::vector<expression*>* body;
		static constexpr const char* ExprType = "function expression";

		public:
			function_exp(llvm::StringRef name, std::vector<int> arg_types, std::vector<std::string> arg_names, int ret_type, std::vector<expression*>* body)
				 : expression(name), arg_types(std::move(arg_types)), arg_names(std::move(arg_names)), ret_type(ret_type), body(std::move(body)) {}

			Function *codegen();
			virtual const char *getType() { return ExprType; }
	};
}