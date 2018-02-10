%code requires{
#include "ast.hpp"
	
#include <cassert>

extern const ASTNode *root; // Definition of variable (to match declaration earlier)



int yylex(void);
void yyerror(const char *);

}


%union{
	const ASTNode *node;
	Declaration *declaration_node;
	Statement *statement_node;
	Expression *expression_node;
	std::string *string;
	int int_num;
}

%token INT
%token RETURN
%token IDENTIFIER CONSTANT LITERAL




%type <node> translation_unit global_declaration function_definition
%type <declaration_node> declaration parameter_declaration parameter_list

%type<expression_node> expression base_expression mult_expression add_expression

%type <statement_node> jump_statement statement statement_list compound_statement expr_statement

%type <string> IDENTIFIER LITERAL INT RETURN 
%type <string> type_specifier 


%type <int_num> CONSTANT

%start ROOT

%%

ROOT : translation_unit	{ root = $1; }

translation_unit	: global_declaration					 { $$ = $1; }
					| translation_unit global_declaration   { $$ = new TranslationUnit($1,$2); }

global_declaration	: function_definition
					| declaration

function_definition	: type_specifier IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new FunctionDefinition(*$1,*$2,$6); }

declaration 		: type_specifier ';'							{ $$ = new Declaration(*$1); }
					| type_specifier IDENTIFIER ';'					{ $$ = new Declaration(*$1,*$2); }
					| type_specifier IDENTIFIER '=' expression ';'	{ $$ = new Declaration(*$1,*$2,$4); }
								




parameter_declaration	: type_specifier IDENTIFIER { $$ = new Declaration(*$1,*$2); }		


parameter_list		:	parameter_declaration				 { $$ = $1; }
					|	parameter_declaration ',' parameter_list { $1->next = $3; $$ = $1; }





expression			: add_expression			{ $$ = $1;}
					| IDENTIFIER '=' expression { $$ = new AssignmentExpression(*$1,$3); }	


add_expression		: mult_expression					  { $$ = $1; }
					| add_expression '+' mult_expression  { $$ = new AddExpression($1,$3);	}
					| add_expression '-' mult_expression  { $$ = new SubExpression($1,$3);	}


mult_expression		: base_expression					  { $$ = $1; }
					| mult_expression '*' base_expression { $$ = new MultExpression($1,$3);	}
					| mult_expression '/' base_expression { $$ = new DivExpression($1,$3);	}


base_expression		: CONSTANT			 { $$ = new Constant($1);    	}
					| IDENTIFIER		 { $$ = new Identifier(*$1); 	}	
					| LITERAL			 { $$ = new StringLiteral(*$1); }	
					| '(' expression ')' { $$ = $2;						}










statement 			: jump_statement			{ $$ = $1; }
					| compound_statement		{ $$ = $1; }
					| expr_statement			{ $$ = $1; }


statement_list 		: statement 				{ $$ = $1; }
					| statement statement_list 	{ $1->next = $2; $$ = $1; }


compound_statement  : '{' statement_list '}'	{ $$ = new CompoundStatement($2); }
					| '{' '}'					{ $$ = new CompoundStatement();}
				
				
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