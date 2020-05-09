#include "checker.h"
#include "optimizer.h"
#include "codegen.h"

#include <stdio.h>

std::vector<function_exp*> getStatements() {
  /*
    TheModule = make_unique<Module>("Program", TheContext);

	auto *ret_exp = new return_statement(new integer_const(5));
	function_exp *foo = new function_exp("foo", {0}, {"in"}, 0, {ret_exp});
	
	function_call *fc = new function_call("foo", {new integer_const(2)});
	function_exp *main = new function_exp("main", {}, {}, -1, {fc});

	std::vector<function_exp*> statements = {};

	statements.push_back(foo);
	statements.push_back(main);

    return statements;
    */
  return std::vector<function_exp*>();
}

int compile (std::vector<function_exp*> program) {
	//if(checkProgram(program))
   //     perror("Program sanity check");
   // std::vector<function_exp*> optimized = optimizeProgram(program);
	codegen(program);
	
	return 0;
}
