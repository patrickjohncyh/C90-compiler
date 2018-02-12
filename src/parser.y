%code requires{
#include "ast.hpp"
	
#include <cassert>

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
}

%token INT
%token RETURN
%token IDENTIFIER CONSTANT LITERAL
%token INC_OP




%type <node> translation_unit global_declaration function_definition
%type <declarator_node>	declarator init_declarator init_declarator_list

%type <declaration_node> declaration declaration_list parameter_declaration parameter_list 

%type<expression_node> expression base_expression mult_expression add_expression assignment_expression argument_list


%type <statement_node> jump_statement statement statement_list compound_statement expr_statement

%type <string> IDENTIFIER LITERAL INT RETURN 
%type <string> type_specifier 


%type <int_num> CONSTANT

%start ROOT

%%

ROOT : translation_unit	{ root = $1; }

translation_unit	: global_declaration					{ $$ = $1; }
					| translation_unit global_declaration   { $$ = new TranslationUnit($1,$2); }

global_declaration	: function_definition					{ $$ = $1; }
					| declaration 							{ $$ = $1; }

function_definition	: type_specifier IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new FunctionDefinition(*$1,*$2,$4,$6); }

declarator			: IDENTIFIER 						{ $$ = new Declarator(*$1);		}


init_declarator		: declarator 						{ $$ = $1;								}
					| declarator '=' expression 		{ $$ = new Declarator($1->getId(),$3);	}

init_declarator_list: init_declarator 							{ $$ = $1; 				}
					| init_declarator ',' init_declarator_list	{ $1->next = $3; $$ =$1;}


declaration 		: type_specifier ';'						{ $$ = new Declaration(*$1);	}
					| type_specifier init_declarator_list ';' 	{ $$ = new Declaration(*$1,$2);	}
								

declaration_list	: declaration 						{ $$ = $1; }
					| declaration declaration_list 		{ $1->next = $2; $$ = $1; }


parameter_declaration	: type_specifier declarator 	{ $$ = new Declaration(*$1,$2); }		


parameter_list		:	 										 { $$ = NULL; 				}
					|	parameter_declaration				     { $$ = $1; 				}
					|	parameter_declaration ',' parameter_list { $1->next = $3; $$ = $1;	}


argument_list		: expression 						{ $$ = $1; 					}
					| expression ',' argument_list  	{ static_cast<AssignmentExpression*>($1)->next = $3; $$ = $1; 	}

assignment_expression	: IDENTIFIER '=' expression { $$ = new AssignmentExpression(*$1,$3); }	


expression			: add_expression			{ $$ = $1; }
					| assignment_expression		{ $$ = $1; }


add_expression		: mult_expression					  { $$ = $1; }
					| add_expression '+' mult_expression  { $$ = new AddExpression($1,$3);	}
					| add_expression '-' mult_expression  { $$ = new SubExpression($1,$3);	}


mult_expression		: base_expression					  { $$ = $1; }
					| mult_expression '*' base_expression { $$ = new MultExpression($1,$3);	}
					| mult_expression '/' base_expression { $$ = new DivExpression($1,$3);	}


base_expression		: CONSTANT			 { $$ = new Constant($1);    			}
					| IDENTIFIER		 { $$ = new Identifier(*$1); 			}	
					| LITERAL			 { $$ = new StringLiteral(*$1); 		}	
					| '(' expression ')' { $$ = $2;								}
					| IDENTIFIER '(' ')' { $$ = new FunctionCallExpression(*$1);}
					| IDENTIFIER '(' argument_list ')' { $$ = new FunctionCallExpression(*$1,$3);}








statement 			: jump_statement			{ $$ = $1; }
					| compound_statement		{ $$ = $1; }
					| expr_statement			{ $$ = $1; }


statement_list 		: statement 				{ $$ = $1; }
					| statement statement_list 	{ $1->next = $2; $$ = $1; }


compound_statement  : '{' '}'									{ $$ = new CompoundStatement();	  		}
					| '{' statement_list   					'}'	{ $$ = new CompoundStatement(NULL,$2);	}
					| '{' declaration_list 					'}'	{ $$ = new CompoundStatement($2,NULL);	}
					| '{' declaration_list statement_list 	'}'	{ $$ = new CompoundStatement($2,$3);	}
					
				
				
jump_statement		: RETURN ';'			{ $$ = new JumpStatement("return"); 	}
					| RETURN expression ';'	{ $$ = new JumpStatement("return",$2);	}


expr_statement		: expression ';'	{ $$ = new ExprStatement($1);		}



type_specifier		 : INT 		  { $$ = new std::string("int"); }


%%

const ASTNode *root; // Definition of variable (to match declaration earlier)



const ASTNode* parse(){
	root = 0;
	yyparse();
	return root;
}