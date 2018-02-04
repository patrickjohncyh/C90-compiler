%code requires{
#include "ast.hpp"
	
#include <cassert>

extern const ASTNode *root; // Definition of variable (to match declaration earlier)



int yylex(void);
void yyerror(const char *);

}


%union{
	const ASTNode *declaration;
	std::string *string;
}

%token T_STRING

%type <declaration> translation_unit
%type <declaration> external_declaration
%type <string> T_STRING

%start ROOT

%%


ROOT : translation_unit 	{ root = $1; }

translation_unit : external_declaration { $$ = $1; }
				 | translation_unit external_declaration { $$ = new TranslationUnit($1,$2); }

external_declaration :  T_STRING  { $$ = new ExternalDeclaration(*$1); }


%%

const ASTNode *root; // Definition of variable (to match declaration earlier)



const ASTNode* parse(){
	root = 0;
	yyparse();
	return root;
}