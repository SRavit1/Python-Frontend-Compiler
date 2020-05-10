#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "compile.h"
#include "nodescpp.h"


int main (void) {
	std::vector<expression*> square_body = {};

    variable* input = new variable("num");
	square_body.push_back(new binary_expression("squared", "*", input, input));
	square_body.push_back(new return_statement("", new variable("squared")));

	function_exp* square = new function_exp("square", {0}, {"num"}, 0, &square_body);

    std::vector<function_exp*> program = {};

    function_call* call_square = new function_call("call_square", "square");
	call_square->addExpression(new integer_const("num", 3));
	std::vector<expression*> main_body = {call_square};
	
    function_exp* main = new function_exp("main", {}, {}, 0, &main_body);

    program.push_back(square);
    program.push_back(main);
	
    compile(program);

	return 0;
}