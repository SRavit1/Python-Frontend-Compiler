//linked-list organization of statements
struct statement_link {
	struct statement current_statement;
	struct statement_link* next;
};

//declaring the dynamic statement_link body
struct statement_link* body;
struct statement_link head;

//flag variable set to 1 upon head being defined
int head_defined = 0;

/*for adding statements to the statement_link*/
void add (struct statement* curr_statement) {
	if (!head_defined) {
		body = malloc(sizeof(*body));
	}	
	
	body->current_statement = *curr_statement;
	body->next = malloc(sizeof(*(body->next)));

	if (!head_defined) {
		head = *body;
		head_defined = 1;
	}
	
	body = body->next;
}
//----------------------------------------------------------------------

		/*method that prints add int expression*/
		void print_int_arith(struct int_arith* int_arith_i, int indent_level) {
			print_int_exp(int_arith_i->operand1, indent_level);
			print_int_exp(int_arith_i->operand2, indent_level);
		}

	/*method that prints integer expressions*/
	void print_int_exp(struct int_exp* int_exp_i, int indent_level) {
		int type = int_exp_i->type;
		switch (type) {
			case 1: //integer constant
				{
					_print_indent(indent_level);
					printf("constant integer of value %d\n", int_exp_i->int_exp_val.int_val);
				}
				break;
			case 2: //integer variable
				{
					_print_indent(indent_level);
					printf("integer variable of name %s\n", int_exp_i->int_exp_val.int_var_val.var_name);
				}
				break;
			case 3: //integer function
				break;
			case 4: //arithmetic expression
				{
					_print_indent(indent_level);
					printf("arithmetic expression of type %d\n", int_exp_i->int_exp_val.int_arith_val.type);
					print_int_arith(&(int_exp_i->int_exp_val.int_arith_val), indent_level+1);
				}
				break;
		}
	}

/*master expression printing function that calls methods that print each expression*/
void print_expression(struct expression* expression_i, int indent_level) {
	int type = expression_i->type;
	switch (type) {
		case 1: //integer expression
			{
			_print_indent(indent_level);
			printf("expression\n");
			print_int_exp(&(expression_i->expression_val.int_exp_val), indent_level + 1);
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	}
}

/*master statement printing function that calls methods that print specialized statements*/
void print_statement(struct statement* statement_i, int indent_level) {
	int type = statement_i->type;
	switch (type) {
		case 0: //No statement
			return; 
			break;	
		case 1: //Print statement
			{
			_print_indent(indent_level);
			printf("print statement\n");
			print_expression(&(statement_i->statement_val.print_statement_s.print_content), indent_level+1);
			}
			break;
		case 2: //Assign statement
			{
			_print_indent(indent_level);
			printf("assign statement\n");

			_print_indent(indent_level+1);
			printf("variable name is %s\n", statement_i->statement_val.assign_s.var_name);

			print_expression(&(statement_i->statement_val.assign_s.value), indent_level+1);
			}
			break;		
		default:
			printf("Error");
	}
}

/*main method that iterates and prints all statements in the program*/
void print_ast(struct statement_link head) {
	struct statement_link* sl_i = &head;
	
	int statement_no = 1;
	while (sl_i && sl_i->current_statement.type) {
		struct statement statement_i = sl_i->current_statement;
		
		printf("Statement %d: ", statement_no);
		print_statement(&statement_i, 0);
		
		sl_i = sl_i->next;
		statement_no++;
	}
}

void _print_indent(int indents) {
	char indented[50];

	strcpy(indented,  "");

	for (int i = 0; i < indents; i++)
		strcat(indented, "\t");

	printf(indented);
}
