#include "codegen.h"

void codegen(const std::vector<node*> statements) {
	for (auto statement : statements) {
		if (statement->getType()==0)
			statement->expr->codegen();
		else if (statement->getType()==1)
			statement->func->codegen();
	}

	TheModule->print(errs(), nullptr);
}

int main() {
	TheModule = make_unique<Module>("my cool jit", TheContext);

	binary_expression* lhs = new binary_expression('-', new integer_const(3), new integer_const(4));
	integer_const* rhs = new integer_const(4);
	auto *bin_exp = new binary_expression('+', lhs, rhs);
	auto *ret_exp = new return_statement(new integer_const(5));

	integer_const* arg = new integer_const(2);
	std::vector<expression*> args = {arg};

	function_exp *add_two_funct = new function_exp("fib", {0}, {"in"}, 0, {ret_exp, bin_exp});
	
	function_call *fc = new function_call("fib", args);
	function_exp *main = new function_exp("main", {}, {}, -1, {fc});

	std::vector<node*> statements = {};

	statements.push_back(new node(add_two_funct));
	statements.push_back(new node(main));

	codegen(statements);

	

	return 0;
}