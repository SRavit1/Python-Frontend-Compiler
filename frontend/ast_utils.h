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
					_print_indent("constant integer", indent_level);
				}
				break;
			case 2: //integer variable
				break;
			case 3: //integer function
				break;
			case 4: //arithmetic expression
				{
					_print_indent("arithmetic expression", indent_level);
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
			_print_indent("expression", indent_level);
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
			_print_indent("print statement", indent_level);
			print_expression(&(statement_i->statement_val.print_statement_s.print_content), indent_level+1);
			}
			break;
		default:
			printf("Error");
	}
}

/*main method that iterates and prints all statements in the program*/
void print_ast(struct statement_link head) {
	struct statement_link* sl_i = &head;
	while (sl_i) {
		struct statement statement_i = sl_i->current_statement;
		
		print_statement(&statement_i, 0);
		sl_i = sl_i->next;
	}
}

void _print_indent(char* text, int indents) {
	char indented[50], to_print[50];

	strcpy(indented,  "");
	strcpy(to_print, text);

	for (int i = 0; i < indents; i++)
		strcat(indented, "\t");
	
	strcat(indented, strcat(to_print, "\n"));
	printf(indented);
}
