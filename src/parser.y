%code requires{
#include "ast.hpp"
	
#include <cassert>
#include <vector>

extern const ASTNode *root; // Definition of variable (to match declaration earlier)

int yylex(void);
void yyerror(const char *);

}


%union{
	const ASTNode *node;
	Declarator *declarator_node;
	Declaration *declaration_node;
	Statement *statement_node;
	Expression *expression_node;
	std::string *string;
	int int_num;




	std::vector<Expression 	*>* argument_list_vector;
	std::vector<Statement 	*>* statement_list_vector;
	std::vector<Declaration *>* declaration_list_vector;
	std::vector<Declarator  *>* declarator_list_vector;

}

%token INT
%token VOID
%token RETURN
%token IF
%token ELSE
%token WHILE
%token FOR


%token IDENTIFIER CONSTANT LITERAL
%token INC_OP

%token EQ_OP NE_OP LT_OP GT_OP LE_OP GE_OP




%type <node> translation_unit global_declaration function_definition
%type <declarator_node>	declarator init_declarator 
%type <declarator_list_vector> init_declarator_list


%type <declaration_node> declaration parameter_declaration 
%type <declaration_list_vector> declaration_list parameter_list


%type<expression_node> base_expression postfix_expression mult_expression add_expression 
%type<expression_node> bw_shift_expression  compare_expression  equality_expression
%type<expression_node> bitwise_expression logical_expression ternary_expression 
%type<expression_node> assign_expression  expression 

%type<argument_list_vector> argument_list





%type <statement_node> jump_statement statement compound_statement expr_statement condition_statement iteration_statement
%type <statement_list_vector> statement_list



%type <string> IDENTIFIER LITERAL INT RETURN 
%type <string> type_specifier 


%type <int_num> CONSTANT

%start ROOT

%%



ROOT : translation_unit	{ root = $1; }

translation_unit	: 	global_declaration					  { $$ = $1; }
					| 	translation_unit global_declaration   { $$ = new TranslationUnit($1,$2); }

global_declaration	: 	function_definition						{ $$ = $1; }
					| 	declaration 							{ $$ = $1; }

function_definition	: 	type_specifier IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new FunctionDefinition(*$1,*$2,$4,$6); }

declarator			: 	IDENTIFIER 						{ $$ = new Declarator(*$1);				}

init_declarator		: 	declarator 						{ $$ = $1;								}
					| 	declarator '=' expression 		{ $$ = new Declarator($1->getId(),$3);	}

init_declarator_list: 	init_declarator 							{ $$ = new std::vector<Declarator*>(1,$1);	}
					|	init_declarator_list ',' init_declarator	{ $1->push_back($3);  $$ = $1;				}


declaration 		:	type_specifier ';'							{ $$ = new Declaration(*$1);	}
					| 	type_specifier init_declarator_list ';' 	{ $$ = new Declaration(*$1,$2);	}
								

declaration_list	: 	declaration 						{ $$ = new std::vector<Declaration*>(1,$1); }
					| 	declaration_list declaration  		{ $1->push_back($2);	$$ = $1; 			}


parameter_declaration:	type_specifier declarator 	{ $$ = new Declaration(*$1,new std::vector<Declarator*>(1,$2)); }		


parameter_list		:	parameter_declaration				     { $$ = new std::vector<Declaration*>(1,$1);	}
					|	parameter_list ',' parameter_declaration { $1->push_back($3); $$ = $1;					}
					|											 { $$ = NULL;}


/*---------------------------------------------------------------------------------------------------------------------*/
/* Expressions */


base_expression		: 	CONSTANT			{ $$ = new Constant($1);   		}
					| 	IDENTIFIER		 	{ $$ = new Identifier(*$1); 	}	
					| 	LITERAL			 	{ $$ = new StringLiteral(*$1); 	}	
					| 	'(' expression ')'	{ $$ = $2;						}
					

postfix_expression	:	base_expression	
					|	base_expression	INC_OP					{ $$ = new PostIncrementExpression($1); 	}
					|	base_expression	'(' ')'					{ $$ = new FunctionCallExpression($1) ;		}
					|	base_expression '(' argument_list ')'	{ $$ = new FunctionCallExpression($1,$3);	}
					/* to implement more postfix expressions i.e. arrays */



mult_expression		:	postfix_expression				 	{ $$ = $1; }
					| 	mult_expression '*' postfix_expression { $$ = new MultExpression($1,$3);	}
					| 	mult_expression '/' postfix_expression { $$ = new DivExpression($1,$3);	}
					/* to implement modulo expressions */

add_expression		: 	mult_expression					  	{ $$ = $1; }
					| 	add_expression '+' mult_expression	{ $$ = new AddExpression($1,$3);	}
					| 	add_expression '-' mult_expression  { $$ = new SubExpression($1,$3);	}

bw_shift_expression	:	add_expression

compare_expression	: 	bw_shift_expression
					|	compare_expression LT_OP bw_shift_expression	{ $$ = new LessThanExpression($1,$3); 		}
					|	compare_expression LE_OP bw_shift_expression	{ $$ = new LessThanEqExpression($1,$3); 	}
					|	compare_expression GT_OP bw_shift_expression	{ $$ = new MoreThanExpression($1,$3); 		}
					|	compare_expression GE_OP bw_shift_expression	{ $$ = new MoreThanEqExpression($1,$3); 	}

equality_expression	: 	compare_expression
					|	equality_expression EQ_OP compare_expression	{ $$ = new EqualityExpression($1,$3); 		}
					|	equality_expression NE_OP compare_expression	{ $$ = new NotEqualityExpression($1,$3); 	}

bitwise_expression	: 	equality_expression /* to expand into bitwise AND,XOR,OR for precedence */

logical_expression	: 	bitwise_expression /* to expand into logical  AND,OR for precedence */

ternary_expression 	: 	logical_expression /* to implement conidtional expression */

assign_expression	: 	ternary_expression 
					|	postfix_expression '=' assign_expression { $$ = new DirectAssignmentExpression($1,$3); }	

expression 			:	assign_expression

argument_list		: 	expression 						{ $$ = new std::vector<Expression*>(1,$1);	}
					| 	argument_list ',' expression 	{ $1->push_back($3); $$ = $1;		 		}


/*---------------------------------------------------------------------------------------------------------------------*/
/* Statements */

statement 			: 	jump_statement			{ $$ = $1; }
					| 	compound_statement		{ $$ = $1; }
					| 	expr_statement			{ $$ = $1; }
					| 	condition_statement		{ $$ = $1; }
					|	iteration_statement		{ $$ = $1; }


statement_list 		: 	statement 				{ $$ = new std::vector<Statement*>(1,$1);	}
					| 	statement_list statement{ $1->push_back($2); $$ = $1; 				}


compound_statement  : 	'{' '}'										{ $$ = new CompoundStatement();	  		}
					| 	'{' statement_list   					'}'	{ $$ = new CompoundStatement(NULL,$2);	}
					| 	'{' declaration_list 					'}'	{ $$ = new CompoundStatement($2,NULL);	}
					| 	'{' declaration_list statement_list 	'}'	{ $$ = new CompoundStatement($2,$3);	}
					

				
jump_statement		: 	RETURN ';'				{ $$ = new JumpStatement(); 	}
					| 	RETURN expression ';'	{ $$ = new JumpStatement($2);	}


expr_statement		: 	expression ';'			{ $$ = new ExprStatement($1);	}


condition_statement :	IF '(' expression ')' statement 				{ $$ = new ConditionIfStatement($3,$5); 		}	
					|	IF '(' expression ')' statement ELSE statement 	{ $$ = new ConditionIfElseStatement($3,$5,$7); 	}


iteration_statement	:	WHILE '(' expression ')' statement 									{ $$ = new WhileStatement($3,$5);	}
					|	FOR   '(' expression ';' expression ';' expression ')' statement	{ $$ = new ForStatement($3,$5,$7,$9);	}

/*---------------------------------------------------------------------------------------------------------------------*/

type_specifier		:	INT 		{ $$ = new std::string("int"); }
					|	VOID		{ $$ = new std::string("void");} 			


%%

const ASTNode *root; // Definition of variable (to match declaration earlier)



const ASTNode* parse(){
	root = 0;
	yyparse();
	return root;
}