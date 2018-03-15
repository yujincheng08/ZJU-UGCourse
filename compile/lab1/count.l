%{
unsigned wordCount = 0u;
unsigned charCount = 0u;
unsigned lineCount = 0u;
%}
whitespace [ \t]
%%
whitespace {charCount++; /*whitespace char*/}
[a-zA-Z0-9]+ {
  wordCount++; // This is a word
  charCount += strlen(yytext); // and plus the length
}
\n {
  lineCount++; // One line ends
  charCount++; // This is also a char
}
. { charCount++; /*Other char*/}
%%
int main() {
  yylex();
  printf("char: %d\tword: %d\tline: %d\n", charCount, wordCount, lineCount);
  return 0;
}

int yywrap() {
  return 1;
}
