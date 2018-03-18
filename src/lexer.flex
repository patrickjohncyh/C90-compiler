%option noyywrap

%{
#include "parser.tab.hpp"
%}


D	[0-9]
L	[a-zA-Z_]
E	[Ee][+-]?{D}+
H	[a-fA-F0-9]



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

do 								{ return DO;		}
return							{ return RETURN;	}
if 								{ return IF;		}
else							{ return ELSE; 		}
while							{ return WHILE; 	}
for								{ return FOR;		}
switch							{ return SWITCH; 	}
case							{ return CASE;	 	}
break 							{ return BREAK; 	}
default 						{ return DEFAULT; 	}
continue						{ return CONTINUE;	}
goto							{ return GOTO;		}


{L}({L}|{D})*					{ yylval.string = new std::string(yytext); return IDENTIFIER;	}


0{D}+							{ yylval.string = new std::string(yytext); return CONSTANT_I; }
0[xX]{H}+						{ yylval.string = new std::string(yytext); return CONSTANT_I; }
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
[\^]			{ return ('^'); }
[|]				{ return ('|'); }
[?]				{ return ('?'); }	


\==				{ return EQ_OP;	}
\!=				{ return NE_OP;	}
\<				{ return LT_OP;	}
\>				{ return GT_OP; }
\<=				{ return LE_OP; }
\>=				{ return GE_OP; }


\>>=			{ return RIGHT_ASSIGN; 	}
\<<=			{ return LEFT_ASSIGN; 	}
\+=				{ return ADD_ASSIGN; 	}
\-=				{ return SUB_ASSIGN; 	}
\*=				{ return MUL_ASSIGN; 	}
\/=				{ return DIV_ASSIGN; 	}
\%=				{ return MOD_ASSIGN; 	}
\&=				{ return AND_ASSIGN; 	}
\^=				{ return XOR_ASSIGN; 	}
\|=				{ return OR_ASSIGN; 	}



\++				{ return INC_OP;}
\--				{ return DEC_OP;}
\<<				{ return LS_OP; }
\>>				{ return RS_OP;	}
\&&				{ return AND_OP;}
\|\|			{ return OR_OP;}



"/*"([^\*]*(\*[^/])+[^\*/])*[^\*]*(\*)+"/"	{ printf("%s",yytext);}

"//".*			{;}

[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror (char const *s){
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

