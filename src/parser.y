%code requires{
#include "ast.hpp"
	
#include <cassert>

extern const ASTNode *root; // Definition of variable (to match declaration earlier)



int yylex(void);
void yyerror(const char *);

}


%union{
	const ASTNode *node;
	Statement *statement_node;
	std::string *string;
}

%token INT
%token RETURN
%token IDENTIFIER





%type <node> translation_unit function_definition

%type <statement_node> jump_statement statement statement_list compound_statement 

%type <string> IDENTIFIER INT RETURN 
%type <string> type_specifier identifier 

%start ROOT

%%

ROOT : translation_unit	{ root = $1; }

translation_unit 	 : function_definition					  { $$ = $1; }
					 | translation_unit function_definition   { $$ = new TranslationUnit($1,$2); }


function_definition  : type_specifier identifier '(' parameter_list ')' compound_statement { $$ = new FunctionDefinition(*$1,*$2,$6); }


parameter_list 		:


compound_statement  : '{' statement_list '}'	{ $$ = $2; }
					| '{' '}'					{ $$ = NULL;}


statement_list 		: statement 				{ $$ = $1; }
					| statement_list statement 	


statement 			: jump_statement			{ $$ = $1; }
					| compound_statement	
				
jump_statement		: RETURN ';'  {	$$ = new JumpStatement("return"); }


type_specifier		 : INT 		  { $$ = new std::string("int"); }

identifier 	  		 : IDENTIFIER { $$ = new std::string(*$1);   } 

%%

const ASTNode *root; // Definition of variable (to match declaration earlier)



const ASTNode* parse(){
	root = 0;
	yyparse();
	return root;
}