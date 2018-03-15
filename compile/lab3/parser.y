%{
#include <stdio.h>
#include <math.h>
void yyerror(char const *);
int yylex();
%}

%define api.value.type {double}

%token NEWLINE NUM

%left '+' '-'
%left '*' '/'
%right '^'

%%
result: expr NEWLINE {
        printf("\t%lf\n", $1);
      } result
      |
      ;
expr : expr '+' expr {
       $$ = $1 + $3;
     }
     | expr '-' expr {
       $$ = $1 - $3;
     }
     | expr '*' expr {
       $$ = $1 * $3;
     }
     | expr '/' expr {
       if($3==0) {
         yyerror("Divided by zero");
         $$ = INFINITY;
       }
       else
         $$ = $1 / $3;
     }
     | expr '^' expr {
       $$ = pow($1, $3);
     }
     | '-' expr  %prec '*' {
       $$ = -$2;
     }
     | '(' expr ')' {
       $$ = $2;
     }
     | NUM {
       $$ = $1;
     }
     ;
%%

void yyerror(char const*token) {
  fprintf(stderr, "%s\n", token);
}

int yywarp() {
  return 1;
}

