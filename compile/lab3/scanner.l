%{
#include "y.tab.h"
%}
%%
"-"|"+"|"/"|"*"|"^"|"("|")" {return yytext[0];}
[0-9]*(\.[0-9]+)? {
  yylval = strtod(yytext, NULL);
  return NUM;
}
\n {return NEWLINE;};
. ;
%%
int yywrap() {
  return 1;
}
