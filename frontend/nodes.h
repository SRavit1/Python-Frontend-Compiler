		struct int_var {
			char* var_name;
		};

		struct int_function {
			char* funct_name;
		};

		struct add_int {
			struct int_exp* operand1;
			struct int_exp* operand2;
		};

		struct sub_int {
			struct int_exp* operand1;
			struct int_exp* operand2;
		};

		struct mul_int {
			struct int_exp* operand1;
			struct int_exp* operand2;
		};

		struct div_int {
			struct int_exp* operand1;
			struct int_exp* operand2;
		};

		struct pow_int {
			struct int_exp* operand1;
			struct int_exp* operand2;
		};
	
	struct int_exp {
		int type;
		union {
			int int_val; //1
			struct int_var int_var_val; //2
			struct int_function int_function_val; //3
			struct add_int add_int_val; //4
			struct sub_int sub_int_val;
			struct mul_int mul_int_val;
			struct div_int div_int_val;
			struct pow_int pow_int_val;
		} int_exp_val;
	};

		struct float_var {
			char* var_name;
		};

		struct float_function {
			char* funct_name;
		};

		struct add_float {
			struct float_exp* operand1;
			struct float_exp* operand2;
		};

		struct sub_float {
			struct float_exp* operand1;
			struct float_exp* operand2;
		};

		struct mul_float {
			struct float_exp* operand1;
			struct float_exp* operand2;
		};

		struct div_float {
			struct float_exp* operand1;
			struct float_exp* operand2;
		};

		struct pow_float {
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
			struct add_float add_float_val;
			struct sub_float sub_float_val;
			struct mul_float mul_float_val;
			struct div_float div_float_val;
			struct pow_float pow_float_val;
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

		struct and_bool {
			struct bool_exp* operand1;
			struct bool_exp* operand2;
		};

		struct or_bool {
			struct bool_exp* operand1;
			struct bool_exp* operand2;
		};

		struct xor_bool {
			struct bool_exp* operand1;
			struct bool_exp* operand2;
		};

		struct nor_bool {
			struct bool_exp* operand1;
			struct bool_exp* operand2;
		};

		struct not_bool {
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
			struct and_bool and_bool_val;
			struct or_bool or_bool_val;
			struct xor_bool xor_bool_val;
			struct nor_bool nor_bool_val;
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
		int print_content;
	};
	
	struct assignment {
		char* var_name;
		struct expression value;
	};

	struct function_decl {
		char* function_name;
		struct expression param; //really should be a list of expressions
		struct expression ret; //really should be a list of expressions
		struct code_block* body;
	};
	
	struct if_statement {
		struct bool_exp condition;
		struct code_block* body;
	};
	
	struct elif_statement {
		struct _conditions_chain prev_conditions; //previous if/elif statement conditions
		struct bool_exp condition;
		struct code_block* body;
	};
	
	struct else_statement {
		struct _conditions_chain prev_conditions; //previous if/elif statement conditions
		struct code_block* body;
	};
	
	struct while_loop {
		struct bool_exp condition;
		struct code_block* body;
	};

struct statement {
	int type;
	union {
		struct print_statement print_statement_s; //1
		struct assignment assgn_s;
		struct function_decl function_decl_s;
		struct if_statement if_statement_s;
		struct elif_statement elif_statement_s;
		struct else_statement else_statement_s;
		struct while_loop while_loop_s;
	} statement_val;
};

struct statement_link {
	struct statement current_statement;
	int defined;

	struct statement_link* next;
};
