%option noyywrap

%{
#include "parser.tab.hpp"
%}


D	[0-9]
L	[a-zA-Z_]

%%


int				{ return INT; 	 }
return			{ return RETURN; }
{L}({L}|{D})*	{ yylval.string = new std::string(yytext);  return IDENTIFIER;}




[(]				{ return ('(');	}
[)]				{ return (')');	}
[{]				{ return ('{'); }
[}]				{ return ('}'); }
[;]				{ return (';'); }


[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror (char const *s){
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

