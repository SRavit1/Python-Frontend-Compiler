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

namespace {	
	class expression {
		public:
		  virtual ~expression() = default;
		  virtual Value *codegen() = 0;
	};

	class binary_expression : public expression {
		char Op;
		expression* LHS;
		expression* RHS;
		std::string name;

		public:
			binary_expression(char Op, expression* LHS, expression* RHS) : Op(Op), LHS(LHS), RHS(RHS), name("") {}
			binary_expression(char Op, expression* LHS, expression* RHS, const std::string name) : Op(Op), LHS(LHS), RHS(RHS), name(name) {}
			Value *codegen() override;
	};

	class variable : public expression {
		std::string name;

		public:
			variable(const std::string name) : name(name) {}
			Value *codegen() override;
	};

	//TODO: figure out how to store unnamed variable into IR (e.g. "%3 = i32 52")
	//TODO: for integer, float, string, bool const, add "std::string name" field
	class integer_const : public expression {
		int val;
		public:
			integer_const(int val) : val(val) {}
			Value *codegen() override;
	};

	class float_const : public expression {
		float val;
		public:
			float_const(float val) : val(val) {}
			Value *codegen() override;
	};

	class string_const : public expression {
		std::string val;
		public:
			string_const(std::string val) : val(val) {}
			Value *codegen() override;
	};

	class bool_const : public expression {
		bool val;
		public:
			bool_const(bool val) : val(val) {}
			Value *codegen() override;
	};

	class function_call : public expression {
		std::string function_name;
		std::vector<expression*> args;
		public:
			function_call(std::string function_name, std::vector<expression*> args) : function_name(function_name), args(args) {}
			Value *codegen() override;
	};

	class return_statement : public expression {
		expression* return_val;
		public:
			return_statement(expression* return_val) : return_val(std::move(return_val)) {}
			Value *codegen() override;
	};

	class function_exp {
		std::string function_name;
		std::vector<int> arg_types;
		std::vector<std::string> arg_names;
		int ret_type;
		std::vector<expression*> body;
		public:
			function_exp(const std::string function_name, std::vector<int> arg_types, std::vector<std::string> arg_names, int ret_type, std::vector<expression*> body)
				 : function_name(function_name), arg_types(std::move(arg_types)), arg_names(std::move(arg_names)), ret_type(ret_type), body(std::move(body)) {}
			Function *codegen();
	};

	class node {
		int type;
		public:
			expression* expr;
			function_exp* func;
			virtual ~node() = default;
			node(expression* expr) : type(0), expr(expr), func(NULL) {}
			node(function_exp* func) : type(1), expr(NULL), func(func) {}
			int getType() { return type; }
	};
}