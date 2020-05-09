#include "checker.h"
#include "optimizer.h"
#include "codegen.h"

#include <stdio.h>

std::vector<function_exp*> getStatements() {
    /*TheModule = make_unique<Module>("Program", TheContext);

	integer_const ic1(5);
	auto* ret_exp = new return_statement(ic1);
	std::string foo_str = "foo"
	function_exp* foo = new function_exp(foo_str.c_str(), {0}, {"in"}, 0, {*ret_exp});
	
	integer_const ic2(2);
	function_call* fc = new function_call("foo", {ic2});
	function_exp* main = new function_exp("main", {}, {}, -1, {*fc});*/

	std::vector<function_exp*> statements = {};

	//statements.push_back(foo);
	//statements.push_back(main);

    return statements;
}

int compile (std::vector<function_exp> program) {
	if(checkProgram(program))
        perror("Program sanity check");
    std::vector<function_exp> optimized = optimizeProgram(program);
	codegen(program);
	
	return 1;
}