%option noyywrap

%{
#include "parser.tab.hpp"
%}


D	[0-9]
L	[a-zA-Z_]



%%


int								{ return INT; 	 	}
void							{ return VOID;		}

return							{ return RETURN;	}
if 								{ return IF;		}
else							{ return ELSE; 		}
while							{ return WHILE; 	}
for								{ return FOR;		}
switch							{ return SWITCH; 	}
case							{ return CASE;	 	}
break 							{ return BREAK; 	}
default 						{ return DEFAULT; 	}


{L}({L}|{D})*					{ yylval.string = new std::string(yytext);  return IDENTIFIER;	}
{D}+							{ yylval.int_num=strtod(yytext, 0); 		return CONSTANT;	}
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


\==				{ return EQ_OP;	}
\!=				{ return NE_OP;	}
\<				{ return LT_OP;	}
\>				{ return GT_OP; }
\<=				{ return LE_OP; }
\>=				{ return GE_OP; }





\++				{ return INC_OP;}
\--				{ return DEC_OP;}








"//".*			{;}

[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

void yyerror (char const *s){
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}

