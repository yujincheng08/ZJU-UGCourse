%token_type {const string *}
%default_type {const string *}
%token_destructor {delete $$;}
// the parser context
// the parser context
// 4 th arg of Parse
%extra_argument {BaseInterpreter *interpreter}

%syntax_error {
    //cout<<"error"<<endl;
    int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
    string expect = "Syntax error.\n";
    expect += "Expected token:\n";
    for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                    expect += '\t';
                    expect += yyTokenName[i];
                    expect += '\n';
            }
    }
    expect.pop_back();
    interpreter->error(expect);
}

%stack_overflow {
    interpreter->error("Stack overflow.");
}

%name Parser

%include {
#include "parser.h"
#include <iostream>
#include <string>
#include <list>
#include <assert.h>
#include "../BaseInterpreter.h"
#include "../Column.h"
#include "../Action.h"
#include "../Condition.h"
using namespace std;
}

//Rule start in input
%start_symbol  start
// every input starts here
start ::= cmdList.
// cmd list is a list of one or more singlecmd
// always use left recursion
cmdList ::= cmdList cmdAndEnd.
cmdList ::= cmdAndEnd.
cmdAndEnd ::= cmd SEMICOLON.

cmd ::= .

cmd ::= create_table create_table_args. {
    interpreter->setActionType(Action::CreateTable);
}

create_table ::= CREATE TABLE table_name.


create_table_args ::= LEFTPARENTHESIS columnlist optional_constraint_list RIGHTPARENTHESIS.

// column_arg_list
columnlist ::= columnlist COMMA column_define.
columnlist ::= column_define.
column_define ::= column(X) column_constraint(Y).{
    interpreter->newConstraint(*X,Y);
}

rawcolumnlist ::= rawcolumnlist COMMA rawcolumn.
rawcolumnlist ::= rawcolumn.
rawcolumn ::= name(X). {
    interpreter->newColumn(*X,Column::Undefined);
}

column(A) ::= name(X) type_token(Y). {
    interpreter->newColumn(*X, Y);
    A = X;
}
// default type is int
//%token_class int INTEGER.
%type type_token {Column::Type}
type_token(A) ::= INT_TYPE. { A = Column::Int; }
type_token(A) ::= CHAR_TYPE LEFTPARENTHESIS INTEGER(B) RIGHTPARENTHESIS. {
    int x = stoi(*B);
    if(x<=0 || x > 255)
    {
        interpreter->error("Syntax error: Exceed rang of char.");
        A = 0U;
    }
    else
        A = (unsigned)x;
}
type_token(A) ::= FLOAT_TYPE. { A = Column::Float;}

%type column_constraint {Constraint::Type}
column_constraint(A) ::= . {A = Constraint::None;}
column_constraint(A) ::= PRIMARY KEY. {A = Constraint::PrimaryKey;}
column_constraint(A) ::= UNIQUE. {A = Constraint::Unique;}

name(A) ::= NAME(B). {A=B;}

unique_list ::= unique_list COMMA unique.
unique_list ::= unique.
unique ::= name(N). {
    interpreter->newConstraint(*N,Constraint::Unique);
}

optional_constraint_list ::= . //no {}!!!!
optional_constraint_list ::= COMMA constraint_list.
// constraint_list is one or more table_constraints
constraint_list ::= constraint_list COMMA table_constraint.
constraint_list ::= table_constraint.
// primary key is for single column
table_constraint ::= PRIMARY KEY LEFTPARENTHESIS name(N) RIGHTPARENTHESIS. {
    interpreter->newConstraint(*N,Constraint::PrimaryKey);
}
// unique may apply to multiple column
table_constraint ::= UNIQUE LEFTPARENTHESIS unique_list RIGHTPARENTHESIS.


// drop table
cmd ::= DROP TABLE table_name. {
    interpreter->setActionType(Action::DropTable);
}

// where clause
opt_where_clause ::= . 
opt_where_clause ::= where_clause.


%type expr {Condition *}

where_clause ::= WHERE expr(A).{
    interpreter->newCondition(A);
}
%type binary_op{Condition::Type}

// column name
expr (A) ::= stringvalue(X). {
    A = interpreter->newCondition(*X,X->length());
}

expr(A) ::= name(X) DOT name(Y). {
    A = interpreter->newCondition(*Y,Column::Undefined, *X);
}

expr(A) ::= name(X) . {
    A = interpreter->newCondition(*X,Column::Undefined);
}

expr(A) ::= INTEGER(X). {
    A = interpreter->newCondition(*X,Column::Int);
}
expr(A) ::= FLOAT(X). {
    A = interpreter->newCondition(*X,Column::Float);
}

expr(A) ::= LEFTPARENTHESIS expr(B) RIGHTPARENTHESIS.{
    A = B;
}

expr(A) ::= NOT expr(B). {
    A = interpreter->newCondition(Condition::Not, B);
}

expr(X) ::= expr(A) EQ expr(B). {
    X = interpreter->newCondition(Condition::Equal,A,B); 
}
expr(X) ::= expr(A) NE expr(B). {
    X = interpreter->newCondition(Condition::NotEqual,A,B); 
}
expr(X) ::= expr(A) LT expr(B). {
    X = interpreter->newCondition(Condition::LessThan,A,B); 
}
expr(X) ::= expr(A) GT expr(B). {
    X = interpreter->newCondition(Condition::GreaterThan,A,B); 
}
expr(X) ::= expr(A) LE expr(B). {
    X = interpreter->newCondition(Condition::LessEqual,A,B); 
}
expr(X) ::= expr(A) GE expr(B). {
    X = interpreter->newCondition(Condition::GreaterEqual,A,B); 
}

expr(X) ::= expr(A) AND expr(B). {
    X = interpreter->newCondition(Condition::And,A,B); 
}

expr(X) ::= expr(A) OR expr(B). {
    X = interpreter->newCondition(Condition::Or,A,B); 
}



//xpr(X) ::= expr(A) binary_op(OP) expr(B). {
//    X = interpreter->newCondition(OP,A,B); 
//}

table_name ::= name(T).{
    interpreter->addTableName(*T);
}

table_list ::= table_list COMMA table_name.
table_list ::= table_name.

// select
cmd ::= SELECT select_column_list FROM table_list opt_where_clause. {
    interpreter->setActionType(Action::Select);
    
}

// insert
cmd ::= INSERT into table_name VALUES LEFTPARENTHESIS valuelist RIGHTPARENTHESIS.

into ::= .
into ::= INTO.

stringvalue(X) ::= STRING(A).{
    X = new string(A->substr(1,A->length()-2));
    delete A;
}

valuelist ::= valuelist COMMA value.
valuelist ::= value.

value ::= FLOAT(A).{
    interpreter->newColumn(*A, Column::Float);
}
value ::= INTEGER(A).{
    interpreter->newColumn(*A, Column::Int);
}
value ::= stringvalue(A).{
    interpreter->newColumn(*A, A->length());
}

select_column_list ::= TIMES.{}

select_column_list ::= select_column_list COMMA full_name.

select_column_list ::= full_name.

full_name ::= name(X) DOT name(Y). {
    interpreter->newColumn(*Y,Column::Undefined, *X);
}

full_name ::= rawcolumn.

// delete

cmd ::= DELETE FROM table_name opt_where_clause. {
    interpreter->setActionType(Action::Delete);
}

// create index

cmd ::= CREATE INDEX name(N) ON table_name LEFTPARENTHESIS rawcolumnlist RIGHTPARENTHESIS. {
    interpreter->setActionType(Action::CreateIndex);
    interpreter->addIndexName(*N);
}

// drop index

cmd ::= DROP INDEX name(N) ON table_name. {
    interpreter->setActionType(Action::DropIndex);
    interpreter->addIndexName(*N);
}

// operators
%left OR.
%left AND.
%right NOT.
%nonassoc EQ NE GT GE LT LE.
