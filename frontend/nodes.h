		struct int_var {
			char* var_name;
		};

		struct int_function {
			char* funct_name;
		};

		struct int_arith {
			int type; //add 1, sub 2, mul 3, pow 4, div 5, floor div 6, mod 7
			struct int_exp* operand1;
			struct int_exp* operand2;
		};
	
	struct int_exp {
		int type;
		union {
			int int_val;
			struct int_var int_var_val;
			struct int_function int_function_val;
			struct int_arith int_arith_val;
		} int_exp_val;
	};

		struct float_var {
			char* var_name;
		};

		struct float_function {
			char* funct_name;
		};

		struct float_arith {
			int type; //add 1, sub 2, mul 3, pow 4, div 5, floor div 6, mod 7
			struct float_exp* operand1;
			struct float_exp* operand2;
		};

	struct float_exp {
		int type;
		union {
			float float_val;
			struct float_var float_var_val;
			struct float_function float_function_val;
			struct int_exp int_exp_val;
			struct float_arith float_arith_val;
		} float_exp_val;
	};	

		struct string_var {
			char* var_name;
		};

		struct string_function {
			char* funct_name;
		};

		struct concat_str {
			struct string_exp* operand1;
			struct string_exp* operand2;
		};

	struct string_exp {
		int type;
		union {
			char* string_val;
			struct string_var string_var_val;
			struct string_function string_function_val;
			struct concat_str concat_str_val;
		} string_exp_val;
	};
		struct bool_var {
			char* var_name;
		};

		struct bool_function {
			char* funct_name;
		};

		struct bool_ops_two {
			int type; //and 1, or 2, xor 3, nor 4
			struct bool_exp* operand1;
			struct bool_exp* operand2;
		};

		struct bool_ops_one {
			int type; //not 1
			struct bool_exp* operand;
		};
		
		struct equal {
			struct expression* operand1;
			struct expression* operand2;
		};

	struct bool_exp {
		int type;
		union {
			int bool_val;
			struct bool_var bool_var_val;
			struct bool_function bool_function_val;
			struct bool_ops_two bool_ops_two_val;
			struct bool_ops_one bool_ops_one_val;
		} bool_exp_val;
	};

struct expression {
	int type;
	union {
		struct int_exp int_exp_val;
		struct float_exp float_exp_val;
		struct string_exp string_exp_val;
		struct bool_exp bool_exp_val;
	} expression_val;
};

struct _conditions_chain { //not a node
	struct bool_exp current_condition;
	struct _conditions_chain* next_conditions;
};

	struct print_statement {
		struct expression print_content;
	};
	
	struct assignment {
		char* var_name;
		struct expression value;
	};

	struct function_decl {
		char* function_name;
		struct expression param; //really should be a list of expressions
		struct expression ret; //really should be a list of expressions
		struct statement_link* body;
	};
	
	struct if_statement {
		struct bool_exp condition;
		struct statement_link* body;
	};
	
	struct elif_statement {
		struct _conditions_chain prev_conditions; //previous if/elif statement conditions
		struct bool_exp condition;
		struct statement_link* body;
	};
	
	struct else_statement {
		struct _conditions_chain prev_conditions; //previous if/elif statement conditions
		struct statement_link* body;
	};
	
	struct while_loop {
		struct bool_exp condition;
		struct statement_link* body;
	};

struct statement {
	int type;
	union {
		struct print_statement print_statement_s;
		struct assignment assign_s;
		struct function_decl function_decl_s;
		struct if_statement if_statement_s;
		struct elif_statement elif_statement_s;
		struct else_statement else_statement_s;
		struct while_loop while_loop_s;
	} statement_val;
};
