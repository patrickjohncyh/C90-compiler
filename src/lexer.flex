%option noyywrap

%{
#include "parser.tab.hpp"
%}
%%



[a-zA-Z]+	{yylval.string = new std::string(yytext); return T_STRING;}


%%

void yyerror (char const *s){
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

