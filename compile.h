#include "checker.h"
#include "optimizer.h"
#include "codegen.h"

#include <stdio.h>

int compile (std::vector<function_exp*>* program) {
	//if(checkProgram(program))
    //    perror("Program sanity check");
    //std::vector<function_exp> optimized = optimizeProgram(program);
	codegen(program);
	
	return 0;
}