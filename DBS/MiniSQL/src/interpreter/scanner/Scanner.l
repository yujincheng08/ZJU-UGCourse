%case-insensitive
%lex-source = "Scanner.cpp"
%%
[[:space:]]+
;			return SEMICOLON;
create			return CREATE;
table			return TABLE;
insert			return INSERT;
into			return INTO;
select			return SELECT;
from			return FROM;
where			return WHERE;
values			return VALUES;
int			return INT_TYPE;
float			return FLOAT_TYPE;
char			return CHAR_TYPE;
-?[[:digit:]]+		return INTEGER;
\.                      return DOT;
-?[[:digit:]]+\.[[:digit:]]+ return FLOAT;
\(			return LEFTPARENTHESIS;
\)			return RIGHTPARENTHESIS;
,			return COMMA;
'[^']*'                 return STRING;
unique			return UNIQUE;
primary			return PRIMARY;
key			return KEY;
drop			return DROP;
delete			return DELETE;
quit			return BaseInterpreter::QUIT;
exec			return BaseInterpreter::EXEC;
\<			return LT;
\>			return GT;
index			return INDEX;
on			return ON;
\*			return TIMES;
=			return EQ;
!=			return NE;
and			return AND;
//or			return OR;
//not                     return NOT;
[[:alpha:]_][[:alpha:][:digit:]_]* return NAME;
.                       return BaseInterpreter::UNEXPETED;
<<EOF>>			return BaseInterpreter::QUIT;
%%
