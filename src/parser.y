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
	double float_num;
	Type *type_node;

	std::vector<Expression 	*>* argument_list_vector;
	std::vector<Statement 	*>* statement_list_vector;
	std::vector<Declaration *>* declaration_list_vector;
	std::vector<Declarator  *>* declarator_list_vector;

	std::vector<Expression 	*>* initializer_list_vector;

}

%token INT CHAR DOUBLE FLOAT VOID UNSIGNED SHORT SIGNED LONG

%token RETURN
%token IF
%token ELSE
%token DO
%token WHILE
%token FOR
%token SWITCH
%token CASE
%token BREAK
%token DEFAULT
%token CONTINUE
%token GOTO


%token IDENTIFIER LITERAL CONSTANT_I CONSTANT_F CONSTANT_C
%token INC_OP DEC_OP

%token EQ_OP NE_OP LT_OP GT_OP LE_OP GE_OP LS_OP RS_OP AND_OP OR_OP

%token RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN  XOR_ASSIGN OR_ASSIGN


%type <node> translation_unit global_declaration function_definition
%type <declarator_node>	declarator init_declarator 
%type <declarator_list_vector> init_declarator_list

%type <initializer_list_vector> initializer_list

%type <declaration_node> declaration parameter_declaration 
%type <declaration_list_vector> declaration_list parameter_list


%type<expression_node> base_expression postfix_expression prefix_expression cast_expression
%type<expression_node> mult_expression add_expression 
%type<expression_node> bw_shift_expression  compare_expression  equality_expression
%type<expression_node> bitwise_and_expression bitwise_or_expression bitwise_xor_expression logical_and_expression logical_or_expression ternary_expression 
%type<expression_node> assign_expression  expression 

%type<argument_list_vector> argument_list

%type <statement_node> jump_statement statement compound_statement expr_statement condition_statement iteration_statement labeled_statement
%type <statement_list_vector> statement_list



%type <string> IDENTIFIER LITERAL INT RETURN CONSTANT_I	CONSTANT_F CONSTANT_C
%type <type_node> type_specifier 


%start ROOT

%%



ROOT : translation_unit	{ root = $1; }

translation_unit	: 	global_declaration					  { $$ = $1; }
					| 	translation_unit global_declaration   { $$ = new TranslationUnit($1,$2); }

global_declaration	: 	function_definition						{ $$ = $1; }
					| 	declaration 							{ $$ = $1; }

function_definition	: 	type_specifier IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new FunctionDefinition($1,*$2,$4,$6); }

declarator			: 	IDENTIFIER 							{ $$ = new IdentifierDeclarator(*$1);		}
					|   IDENTIFIER '[' expression ']'		{ $$ = new ArrayDeclarator(*$1,$3);			}
					|   IDENTIFIER '['  ']'					{ $$ = new ArrayDeclarator(*$1,NULL);		}
					|	IDENTIFIER '(' parameter_list ')'	{ $$ = new FunctionPrototype(*$1,$3);		}


initializer_list 	:   assign_expression 							{ $$ = new std::vector<Expression*>(1,$1);	}
					|	initializer_list ',' assign_expression 		{ $1->push_back($3); $$=$1;					}


init_declarator		: 	declarator 								{ $$ = $1;															}
					| 	declarator '=' assign_expression 		{ $$ = new InitIdentifierDeclarator($1,$3);							}
					|	declarator '=' '{' initializer_list '}'	{ $$ = new InitArrayDeclarator($1,$4);								}
					|	declarator '=' '{' '}'					{ $$ = new InitArrayDeclarator($1,new std::vector<Expression*> );	}


init_declarator_list: 	init_declarator 							{ $$ = new std::vector<Declarator*>(1,$1);	}
					|	init_declarator_list ',' init_declarator	{ $1->push_back($3);  $$ = $1;				}


declaration 		:	type_specifier ';'							{ $$ = new Declaration(*$1);	}
					| 	type_specifier init_declarator_list ';' 	{ $$ = new Declaration(*$1,$2);	}
								

declaration_list	: 	declaration 						{ $$ = new std::vector<Declaration*>(1,$1); }
					| 	declaration_list declaration  		{ $1->push_back($2);	$$ = $1; 			}


parameter_declaration:	type_specifier declarator 	{ $$ = new Declaration(*$1,new std::vector<Declarator*>(1,$2)); }
					 |	type_specifier				{ $$ = new Declaration(*$1);									}		


parameter_list		:	parameter_declaration				     { $$ = new std::vector<Declaration*>(1,$1);	}
					|	parameter_list ',' parameter_declaration { $1->push_back($3); $$ = $1;					}
					|											 { $$ = NULL;}


/*---------------------------------------------------------------------------------------------------------------------*/
/* Expressions */


base_expression		: 	CONSTANT_I			{ $$ = new IntegralConstant(*$1);	}
					|	CONSTANT_F			{ $$ = new FloatingConstant(*$1);	}
					|	CONSTANT_C			{ $$ = new CharacterConstant(*$1);	}
					| 	IDENTIFIER		 	{ $$ = new Identifier(*$1);  		}	
					| 	LITERAL			 	{ $$ = new StringLiteral(*$1); 		}	
					| 	'(' expression ')'	{ $$ = $2; 							}
					

postfix_expression	:	base_expression	
					|	postfix_expression	INC_OP	
					{ $$ = new PostIncrementExpression($1, new DirectAssignmentExpression($1, new AddExpression($1,new IntegralConstant("1"))));}
					|	postfix_expression	DEC_OP	
					{ $$ = new PostDecrementExpression($1, new DirectAssignmentExpression($1, new SubExpression($1,new IntegralConstant("1"))));}




					|	postfix_expression	'(' ')'					{ $$ = new FunctionCallExpression($1) ;		}
					|	postfix_expression 	'(' argument_list ')'	{ $$ = new FunctionCallExpression($1,$3);	}
					|	postfix_expression 	'[' expression ']'		{ $$ = new ArrayAccessExpression($1,$3); 	}

prefix_expression	:	postfix_expression
					| 	INC_OP prefix_expression	{ $$ = new DirectAssignmentExpression($2, new AddExpression($2,new IntegralConstant("1"))); }
					| 	DEC_OP prefix_expression 	{ $$ = new DirectAssignmentExpression($2, new SubExpression($2,new IntegralConstant("1")));	}
					|	'!'	cast_expression			{ $$ = new EqualityExpression(new IntegralConstant("0"),$2);	}
					|   '+' cast_expression			{ $$ = new PrePositiveExpression($2);	} 
					|	'-' cast_expression			{ $$ = new PreNegativeExpression($2);	}
					|	'~' cast_expression			{ $$ = new BwNotExpression($2);			}
					|	'&'	cast_expression			{ $$ = new ReferenceExpression($2);		}
					|	'*'	cast_expression			{ $$ = new DereferenceExpression($2);	}

cast_expression 	: prefix_expression
					| '(' type_specifier ')' cast_expression	{$$ = new CastExpression($2,$4); }


mult_expression		:	cast_expression 				 	   { $$ = $1; }
					| 	mult_expression '*' cast_expression   { $$ = new MultExpression($1,$3);	}
					| 	mult_expression '/' cast_expression   { $$ = new DivExpression($1,$3);		}
					|	mult_expression '%' cast_expression   { $$ = new ModuloExpression($1,$3);	}


add_expression		: 	mult_expression					  	{ $$ = $1; }
					| 	add_expression '+' mult_expression	{ $$ = new AddExpression($1,$3);	}
					| 	add_expression '-' mult_expression  { $$ = new SubExpression($1,$3);	}

bw_shift_expression	:	add_expression
					|	bw_shift_expression LS_OP add_expression { $$ = new LeftShiftExpression($1,$3);	}
					|	bw_shift_expression RS_OP add_expression { $$ = new RightShiftExpression($1,$3);}

compare_expression	: 	bw_shift_expression
					|	compare_expression LT_OP bw_shift_expression	{ $$ = new LessThanExpression($1,$3); 		}
					|	compare_expression LE_OP bw_shift_expression	{ $$ = new LessThanEqExpression($1,$3); 	}
					|	compare_expression GT_OP bw_shift_expression	{ $$ = new MoreThanExpression($1,$3); 		}
					|	compare_expression GE_OP bw_shift_expression	{ $$ = new MoreThanEqExpression($1,$3); 	}

equality_expression	: 	compare_expression
					|	equality_expression EQ_OP compare_expression	{ $$ = new EqualityExpression($1,$3); 		}
					|	equality_expression NE_OP compare_expression	{ $$ = new NotEqualityExpression($1,$3); 	}



bitwise_and_expression	:	equality_expression
						|	bitwise_and_expression '&' equality_expression		{$$ = new BwAndExpression($1,$3);	}

bitwise_xor_expression	:	bitwise_and_expression
						|	bitwise_xor_expression  '^' bitwise_and_expression	{$$ = new BwXorExpression($1,$3);	}

bitwise_or_expression	: 	bitwise_xor_expression
						|	bitwise_or_expression '|' bitwise_xor_expression 	{$$ = new BwOrExpression($1,$3);	}

logical_and_expression	: 	bitwise_or_expression
						|   logical_and_expression AND_OP bitwise_or_expression {$$ = new LogicalAndExpression($1,$3);	}

logical_or_expression	:	logical_and_expression
						|	logical_or_expression OR_OP logical_and_expression	{$$ = new LogicalOrExpression($1,$3);	}

ternary_expression 		: 	logical_or_expression 
						|	logical_or_expression '?' expression ':' ternary_expression {$$ = new TernaryExpression($1,$3,$5);	}


assign_expression	: 	ternary_expression 
				|	prefix_expression '=' 			assign_expression { $$ = new DirectAssignmentExpression($1,$3); }
				|	prefix_expression RIGHT_ASSIGN assign_expression  { $$ = new DirectAssignmentExpression($1, new RightShiftExpression($1,$3));}
				|	prefix_expression LEFT_ASSIGN	assign_expression { $$ = new DirectAssignmentExpression($1, new LeftShiftExpression($1,$3)); }
				|	prefix_expression ADD_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new AddExpression($1,$3));		 }
				|	prefix_expression SUB_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new SubExpression($1,$3)); 		 }
				|	prefix_expression MUL_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new MultExpression($1,$3)); 	 }
				|	prefix_expression DIV_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new DivExpression($1,$3)); 		 }
				|	prefix_expression MOD_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new ModuloExpression($1,$3)); 	 }
				|	prefix_expression AND_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new BwAndExpression($1,$3)); 	 }
				|	prefix_expression XOR_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new BwXorExpression($1,$3)); 	 }
				|	prefix_expression OR_ASSIGN 	assign_expression { $$ = new DirectAssignmentExpression($1, new BwOrExpression($1,$3)); 	 }


expression 			:	assign_expression
					|	expression ',' assign_expression	{ $$ = new CommaExpression($1,$3);			}

argument_list		: 	assign_expression 					{ $$ = new std::vector<Expression*>(1,$1);	}
					| 	argument_list ',' assign_expression { $1->push_back($3); $$ = $1;		 		}


/*---------------------------------------------------------------------------------------------------------------------*/
/* Statements */

statement 			: 	jump_statement			{ $$ = $1; }
					| 	compound_statement		{ $$ = $1; }
					| 	expr_statement			{ $$ = $1; }
					| 	condition_statement		{ $$ = $1; }
					|	iteration_statement		{ $$ = $1; }
					|	labeled_statement		{ $$ = $1; }


statement_list 		: 	statement 				{ $$ = new std::vector<Statement*>(1,$1);	}
					| 	statement_list statement{ $1->push_back($2); $$ = $1; 				}


compound_statement  : 	'{' '}'										{ $$ = new CompoundStatement();	  		}
					| 	'{' statement_list   					'}'	{ $$ = new CompoundStatement(NULL,$2);	}
					| 	'{' declaration_list 					'}'	{ $$ = new CompoundStatement($2,NULL);	}
					| 	'{' declaration_list statement_list 	'}'	{ $$ = new CompoundStatement($2,$3);	}
					

				
jump_statement		: 	RETURN 	 ';'			{ $$ = new JumpStatement()		; 	}
					| 	RETURN expression ';'	{ $$ = new JumpStatement($2)	;	}
					|	BREAK 	 ';'			{ $$ = new JumpBreakStatement()	;	}
					|	CONTINUE ';'			{ $$ = new ContinueStatement()	;	}
					|	GOTO IDENTIFIER ';' 	{ $$ = new GotoStatement(*$2)	;	}


expr_statement		:  expression ';'			{ $$ = new ExprStatement($1);	}
					|			  ';'			{ $$ = new ExprStatement();		}	
					

condition_statement :	IF '(' expression ')' statement 				{ $$ = new ConditionIfStatement($3,$5); 		}	
					|	IF '(' expression ')' statement ELSE statement 	{ $$ = new ConditionIfElseStatement($3,$5,$7); 	}
					| 	SWITCH '(' expression ')' statement 			{ $$ = new ConditionSwitchStatement($3,$5); 	} 


iteration_statement	:	WHILE '(' expression ')' statement 								{ $$ = new WhileStatement($3,$5);		}
					|	DO statement WHILE '(' expression ')' ';'						{ $$ = new DoWhileStatement($2,$5);		}
					|	FOR '(' expr_statement expr_statement ')' statement				{ $$ = new ForStatement($3,$4,NULL,$6);	}
					| 	FOR '(' expr_statement expr_statement expression ')' statement 	{ $$ = new ForStatement($3,$4,$5,$7);	}



labeled_statement	:	CASE expression ':' statement 	{ $$ = new LabeledCaseStatement($2,$4);  }
					| 	DEFAULT 		':' statement 	{ $$ = new LabeledDefaultStatement($3);  }
					| 	IDENTIFIER 		':' statement 	{ $$ = new LabeledGotoStatement(*$1,$3);}


/*---------------------------------------------------------------------------------------------------------------------*/



type_specifier		:	VOID				{ $$ = new Type(Void);		}
					|	CHAR 				{ $$ = new Type(Char);		} 
					|	SIGNED CHAR			{ $$ = new Type(Char);		} 
					|	UNSIGNED CHAR 		{ $$ = new Type(UChar);		}
					|	SHORT 				{ $$ = new Type(Short);		} 
					|	SIGNED SHORT		{ $$ = new Type(Short);		} 
					|	SHORT INT 			{ $$ = new Type(Short);		} 
					|	SIGNED SHORT INT 	{ $$ = new Type(Short);		} 
					|	UNSIGNED SHORT 		{ $$ = new Type(UShort);	}
					|	UNSIGNED SHORT INT	{ $$ = new Type(UShort);	}
					|	INT 				{ $$ = new Type(Int); 		}
					|	SIGNED  			{ $$ = new Type(Int);	 	}
					|	SIGNED INT 			{ $$ = new Type(Int);		}
					|	UNSIGNED 	 		{ $$ = new Type(UInt); 		}
					|	UNSIGNED INT 		{ $$ = new Type(UInt); 		}
					|	LONG				{ $$ = new Type(Long);  	}
					|	SIGNED LONG			{ $$ = new Type(Long); 		}
					|	LONG INT			{ $$ = new Type(Long);		}
					|	SIGNED LONG INT		{ $$ = new Type(Long); 		}
					|	UNSIGNED LONG		{ $$ = new Type(ULong); 	}
					|	UNSIGNED LONG INT	{ $$ = new Type(ULong); 	}
					|	FLOAT 				{ $$ = new Type(Float);		} 
					|	type_specifier '*'	{ $1->inc_pLevel(); $$ = $1;}

					/*|	DOUBLE 				{ $$ = new std::string("double");				}
					|	LONG DOUBLE 		{ $$ = new std::string("double");				}*/
								
%%

const ASTNode *root; // Definition of variable (to match declaration earlier)


const ASTNode* parse(){
	root = 0;
	yyparse();
	return root;
}