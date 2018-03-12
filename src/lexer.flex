%option noyywrap

%{
#include "parser.tab.hpp"
%}


D	[0-9]
L	[a-zA-Z_]
E	[Ee][+-]?{D}+



%%


int								{ return INT; 	 	}
unsigned						{ return UNSIGNED; 	}
signed							{ return SIGNED; 	}
long							{ return LONG;	 	}
void							{ return VOID;		}
char							{ return CHAR;		}
short							{ return SHORT;		}
float							{ return FLOAT;		}
double							{ return DOUBLE;	}


return							{ return RETURN;	}
if 								{ return IF;		}
else							{ return ELSE; 		}
while							{ return WHILE; 	}
for								{ return FOR;		}
switch							{ return SWITCH; 	}
case							{ return CASE;	 	}
break 							{ return BREAK; 	}
default 						{ return DEFAULT; 	}


{L}({L}|{D})*					{ yylval.string = new std::string(yytext); return IDENTIFIER;	}



{D}+							{ yylval.string = new std::string(yytext); return CONSTANT_I; }

L?'(\\.|[^\\'])+'				{ yylval.string = new std::string(yytext); return CONSTANT_C; }

{D}+{E}?						{ yylval.string = new std::string(yytext); return CONSTANT_F; }
{D}*"."{D}+({E})?				{ yylval.string = new std::string(yytext); return CONSTANT_F; }
{D}+"."{D}*({E})?				{ yylval.string = new std::string(yytext); return CONSTANT_F; }




[L]?["](\\.|[^\\"\n])*["]		{ yylval.string = new std::string(yytext);	return LITERAL;		}


[[]				{ return ('[');	}
[]]				{ return (']');	}
[(]				{ return ('(');	}
[)]				{ return (')');	}
[{]				{ return ('{'); }
[}]				{ return ('}'); }
[;]				{ return (';'); }
[,]				{ return (','); }
[:]				{ return (':'); }


[=]				{ return ('='); }
[*]				{ return ('*'); }
[/]				{ return ('/'); }
[%]				{ return ('%'); }
[+]				{ return ('+'); }
[-]				{ return ('-'); }
[~]				{ return ('~'); }
[!]				{ return ('!'); }
[&]				{ return ('&'); }
[\^]				{ return ('^'); }
[|]				{ return ('|'); }		


\==				{ return EQ_OP;	}
\!=				{ return NE_OP;	}
\<				{ return LT_OP;	}
\>				{ return GT_OP; }
\<=				{ return LE_OP; }
\>=				{ return GE_OP; }





\++				{ return INC_OP;}
\--				{ return DEC_OP;}
\<<				{ return LS_OP; }
\>>				{ return RS_OP;	}
\&&				{ return AND_OP;}
\|\|				{ return OR_OP;}




"//".*			{;}

[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror (char const *s){
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

