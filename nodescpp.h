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
		char* name;
		public:
			virtual ~expression() = default;
			virtual Value *codegen() = 0;
			virtual std::string getType() = 0;
			void setName(char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class binary_expression : public expression {
		char Op;
		expression* LHS;
		expression* RHS;
		char* name;
		public:
			binary_expression(char Op, expression* LHS, expression* RHS) : Op(Op), LHS(LHS), RHS(RHS), name(0) {}
			binary_expression(char Op, expression* LHS, expression* RHS, char* name) : Op(Op), LHS(LHS), RHS(RHS), name(name) {}
			Value *codegen() override;
			virtual std::string getType() { return "binary_expression"; };
			void setName(char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class variable : public expression {
		char* name;
		public:
			variable(char* name) : name(name) {}
			Value *codegen() override;
			virtual std::string getType() { return "variable"; };
			void setName(char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	//TODO: figure out how to store unnamed variable into IR (e.g. "%3 = i32 52")
	//TODO: for integer, float, string, bool const, add "char* name" field
	class integer_const : public expression {
		int val;
		const char* name;
		public:
			integer_const(int val) : val(val) {
				std::string newName = "";
				name = newName.c_str();
			}
			Value *codegen() override;
			virtual std::string getType() { return "integer_const"; };
			void setName(const char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class float_const : public expression {
		float val;
		const char* name;
		public:
			float_const(float val) : val(val) {
				std::string newName = "";
				name = newName.c_str();
			}
			Value *codegen() override;
			virtual std::string getType() { return "float_const"; };
			void setName(const char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class string_const : public expression {
		std::string val;
		const char* name;
		public:
			string_const(std::string val) : val(val) {
				std::string newName = "";
				name = newName.c_str();
			}
			Value *codegen() override;
			virtual std::string getType() { return "string_const"; };
			void setName(const char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class bool_const : public expression {
		bool val;
		const char* name;
		public:
			bool_const(bool val) : val(val) {
				std::string newName = "";
				name = newName.c_str();
			}
			Value *codegen() override;
			virtual std::string getType() { return "bool_const"; };
			void setName(const char* newName) { name = newName; }
			const char *getName() { return name; }
	};

	class function_call : public expression {
		std::string function_name;
		std::vector<expression*> args;
		char* name;
		public:
			function_call(std::string function_name, std::vector<expression*> args) : function_name(function_name), args(args), name(0) {}
			function_call(std::string function_name, std::vector<expression*> args, char* call_name) : function_name(function_name), args(args), name(call_name) {}
			Value *codegen() override;
			virtual std::string getType() { return "function_call"; };
			char *getName() { return name; }
	};

	class return_statement : public expression {
		expression* return_val;
		public:
			return_statement(expression* return_val) : return_val(std::move(return_val)) {}
			Value *codegen() override;
			virtual std::string getType() { return "return_statement"; };
	};

	class function_exp {
		const char* function_name;
		std::vector<int> arg_types;
		std::vector<std::string> arg_names;
		int ret_type;
		std::vector<expression*> body;
		public:
			function_exp(const char* function_name, std::vector<int> arg_types, std::vector<std::string> arg_names, int ret_type, std::vector<expression*> body)
				 : function_name(function_name), arg_types(std::move(arg_types)), arg_names(std::move(arg_names)), ret_type(ret_type), body(std::move(body)) {}
			Function *codegen();
			const char *getName() { return function_name; }
	};
}