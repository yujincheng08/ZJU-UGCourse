%{
unsigned wordCount = 0;
unsigned charCount = 0;
unsigned lineCount = 0;
%}
/*rules begin*/
%%
\s {charCount++;}
[a-zA-Z0-9\\]+ {
  wordCount++;
  charCount += strlen(yytext);
}
\n {
  lineCount++;
  charCount++;
}
. {charCount++;}
%%
int main() {
  yylex();
  printf("char: %d\tword: %d\tline: %d\n", charCount, wordCount, lineCount);
  return 0;
}

int yywrap() {
  return 1;
}
