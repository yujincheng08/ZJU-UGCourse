/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 32 "parser.y"

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
#line 20 "parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParserARG_SDECL     A static variable declaration for the %extra_argument
**    ParserARG_PDECL     A parameter declaration for the %extra_argument
**    ParserARG_STORE     Code to store %extra_argument into yypParser
**    ParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 71
#define YYACTIONTYPE unsigned char
#define ParserTOKENTYPE const string *
typedef union {
  int yyinit;
  ParserTOKENTYPE yy0;
  Column::Type yy5;
  Constraint::Type yy16;
  Condition::Type yy46;
  Condition * yy74;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParserARG_SDECL BaseInterpreter *interpreter;
#define ParserARG_PDECL ,BaseInterpreter *interpreter
#define ParserARG_FETCH BaseInterpreter *interpreter = yypParser->interpreter
#define ParserARG_STORE yypParser->interpreter = interpreter
#define YYNSTATE 133
#define YYNRULE 71
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (217)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    97,   13,   11,    9,    8,    7,    6,    5,    4,   13,
 /*    10 */    11,    9,    8,    7,    6,   13,   11,    9,    8,    7,
 /*    20 */     6,    5,    4,   13,   11,    9,    8,    7,    6,    5,
 /*    30 */   204,  204,  204,  204,  204,  204,  133,   84,   59,   12,
 /*    40 */   123,   76,   87,  121,   99,  115,   32,  128,   35,  120,
 /*    50 */    59,   57,   33,   98,   10,   31,   73,  113,  205,   15,
 /*    60 */    80,   79,   43,   57,    2,  131,   36,   42,   96,  112,
 /*    70 */    35,   61,   91,   65,   33,   28,    2,   85,   36,  116,
 /*    80 */    75,   60,  114,   61,   34,   56,   86,   93,   40,   89,
 /*    90 */    58,  106,  108,   91,   65,  110,  132,   79,   43,  129,
 /*   100 */    84,   96,   17,   64,   55,   83,   22,   64,   39,  126,
 /*   110 */    92,   45,   64,  101,   74,   47,   64,  101,  107,   29,
 /*   120 */    44,  122,  101,   64,   48,   18,  101,   64,   64,   88,
 /*   130 */    21,   54,   64,  101,   71,   53,   52,  101,  101,   64,
 /*   140 */    51,  118,  101,   64,   64,  109,  108,   50,   77,  101,
 /*   150 */   125,   49,   46,  101,  101,   72,  111,   69,  120,   72,
 /*   160 */   116,   69,   67,  117,  110,  105,  104,  120,   95,   41,
 /*   170 */   110,   63,   23,  110,  110,   20,  110,   81,  102,  110,
 /*   180 */   110,   26,   30,   19,   94,  102,   27,   90,   62,   38,
 /*   190 */    37,   14,   16,   24,  130,    3,    1,  120,   78,   70,
 /*   200 */    68,   25,   82,   14,  206,   66,  127,  103,  100,  206,
 /*   210 */   206,  206,  206,  124,  206,  206,  119,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     5,   20,   21,   22,   23,   24,   25,   26,   27,   20,
 /*    10 */    21,   22,   23,   24,   25,   20,   21,   22,   23,   24,
 /*    20 */    25,   26,   27,   20,   21,   22,   23,   24,   25,   26,
 /*    30 */    20,   21,   22,   23,   24,   25,    0,   63,    2,    4,
 /*    40 */     7,    8,   68,   10,    9,    5,    6,   48,   49,   14,
 /*    50 */     2,   15,   53,   18,   19,    3,   57,   58,   39,   40,
 /*    60 */    41,   42,   43,   15,   28,    1,   30,   46,   33,   48,
 /*    70 */    49,   35,   52,   53,   53,    3,   28,    9,   30,   53,
 /*    80 */     4,   55,   56,   35,    6,   65,   18,   45,   36,   69,
 /*    90 */    51,   52,   53,   52,   53,   53,   41,   42,   43,    5,
 /*   100 */    63,   33,    6,   53,   67,   68,   64,   53,   36,   12,
 /*   110 */    69,   61,   53,   63,    9,   61,   53,   63,    5,    6,
 /*   120 */    61,    5,   63,   53,   61,   29,   63,   53,   53,    5,
 /*   130 */     6,   61,   53,   63,   12,   61,   61,   63,   63,   53,
 /*   140 */    61,    5,   63,   53,   53,   52,   53,   61,   11,   63,
 /*   150 */    13,   61,   61,   63,   63,   11,   45,   13,   14,   11,
 /*   160 */    53,   13,   45,   56,   53,   45,   45,   14,   45,    4,
 /*   170 */    53,   45,   45,   53,   53,    4,   53,   59,   60,   53,
 /*   180 */    53,    6,   37,    4,   59,   60,   37,   34,   31,   17,
 /*   190 */    17,   16,    4,   29,   44,    4,    6,   14,   47,   53,
 /*   200 */    53,   66,   32,   16,   70,   53,   50,   53,   53,   70,
 /*   210 */    70,   70,   70,   54,   70,   70,   58,
};
#define YY_SHIFT_USE_DFLT (-20)
#define YY_SHIFT_COUNT (79)
#define YY_SHIFT_MIN   (-19)
#define YY_SHIFT_MAX   (191)
static const short yy_shift_ofst[] = {
 /*     0 */    48,  144,  153,  183,   35,   35,   35,   35,   35,   35,
 /*    10 */    35,   35,   35,   35,   35,   36,   68,  183,  183,  183,
 /*    20 */   183,   68,  175,  187,  183,  183,  183,  183,  183,  183,
 /*    30 */   183,  183,  183,   33,  148,  137,  170,  183,  183,  183,
 /*    40 */   183,  183,  190,  191,   -5,  -19,    3,   10,   10,  -11,
 /*    50 */    10,   10,   10,   10,  -11,  124,   96,   72,  113,   52,
 /*    60 */    40,  164,  188,  157,  173,  172,  149,  179,  145,  171,
 /*    70 */   136,  165,  122,   78,  116,  105,   76,   97,   94,   64,
};
#define YY_REDUCE_USE_DFLT (-27)
#define YY_REDUCE_COUNT (43)
#define YY_REDUCE_MIN   (-26)
#define YY_REDUCE_MAX   (159)
static const short yy_reduce_ofst[] = {
 /*     0 */    19,   -1,   20,   21,   91,   90,   86,   79,   75,   74,
 /*    10 */    70,   63,   59,   54,   50,   55,   37,   41,   42,   39,
 /*    20 */    26,  -26,  125,  118,  127,  126,  123,  121,  120,   93,
 /*    30 */   117,  111,  107,  159,  158,  156,  135,  155,  154,  152,
 /*    40 */   147,  146,  151,  150,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   137,  204,  204,  204,  204,  204,  204,  204,  204,  204,
 /*    10 */   204,  204,  204,  204,  204,  137,  204,  204,  204,  204,
 /*    20 */   204,  204,  165,  165,  204,  204,  204,  204,  204,  204,
 /*    30 */   204,  204,  204,  204,  204,  151,  188,  204,  204,  204,
 /*    40 */   204,  204,  158,  204,  204,  167,  182,  175,  176,  181,
 /*    50 */   180,  179,  178,  177,  174,  204,  204,  204,  204,  204,
 /*    60 */   204,  204,  204,  204,  170,  146,  204,  204,  204,  204,
 /*    70 */   204,  204,  204,  159,  204,  204,  204,  204,  204,  204,
 /*    80 */   135,  201,  189,  192,  195,  194,  193,  191,  187,  198,
 /*    90 */   196,  200,  197,  185,  186,  184,  190,  173,  172,  171,
 /*   100 */   169,  168,  166,  199,  203,  164,  145,  202,  146,  144,
 /*   110 */   183,  139,  142,  161,  156,  163,  157,  155,  162,  160,
 /*   120 */   154,  150,  149,  148,  147,  153,  152,  143,  141,  140,
 /*   130 */   138,  136,  134,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "SEMICOLON",     "CREATE",        "TABLE",       
  "LEFTPARENTHESIS",  "RIGHTPARENTHESIS",  "COMMA",         "INT_TYPE",    
  "CHAR_TYPE",     "INTEGER",       "FLOAT_TYPE",    "PRIMARY",     
  "KEY",           "UNIQUE",        "NAME",          "DROP",        
  "WHERE",         "DOT",           "FLOAT",         "NOT",         
  "EQ",            "NE",            "LT",            "GT",          
  "LE",            "GE",            "AND",           "OR",          
  "SELECT",        "FROM",          "INSERT",        "VALUES",      
  "INTO",          "STRING",        "TIMES",         "DELETE",      
  "INDEX",         "ON",            "error",         "start",       
  "cmdList",       "cmdAndEnd",     "cmd",           "create_table",
  "create_table_args",  "table_name",    "columnlist",    "optional_constraint_list",
  "column_define",  "column",        "column_constraint",  "rawcolumnlist",
  "rawcolumn",     "name",          "type_token",    "unique_list", 
  "unique",        "constraint_list",  "table_constraint",  "opt_where_clause",
  "where_clause",  "expr",          "binary_op",     "stringvalue", 
  "table_list",    "select_column_list",  "into",          "valuelist",   
  "value",         "full_name",   
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= cmdList",
 /*   1 */ "cmdList ::= cmdList cmdAndEnd",
 /*   2 */ "cmdList ::= cmdAndEnd",
 /*   3 */ "cmdAndEnd ::= cmd SEMICOLON",
 /*   4 */ "cmd ::=",
 /*   5 */ "cmd ::= create_table create_table_args",
 /*   6 */ "create_table ::= CREATE TABLE table_name",
 /*   7 */ "create_table_args ::= LEFTPARENTHESIS columnlist optional_constraint_list RIGHTPARENTHESIS",
 /*   8 */ "columnlist ::= columnlist COMMA column_define",
 /*   9 */ "columnlist ::= column_define",
 /*  10 */ "column_define ::= column column_constraint",
 /*  11 */ "rawcolumnlist ::= rawcolumnlist COMMA rawcolumn",
 /*  12 */ "rawcolumnlist ::= rawcolumn",
 /*  13 */ "rawcolumn ::= name",
 /*  14 */ "column ::= name type_token",
 /*  15 */ "type_token ::= INT_TYPE",
 /*  16 */ "type_token ::= CHAR_TYPE LEFTPARENTHESIS INTEGER RIGHTPARENTHESIS",
 /*  17 */ "type_token ::= FLOAT_TYPE",
 /*  18 */ "column_constraint ::=",
 /*  19 */ "column_constraint ::= PRIMARY KEY",
 /*  20 */ "column_constraint ::= UNIQUE",
 /*  21 */ "name ::= NAME",
 /*  22 */ "unique_list ::= unique_list COMMA unique",
 /*  23 */ "unique_list ::= unique",
 /*  24 */ "unique ::= name",
 /*  25 */ "optional_constraint_list ::=",
 /*  26 */ "optional_constraint_list ::= COMMA constraint_list",
 /*  27 */ "constraint_list ::= constraint_list COMMA table_constraint",
 /*  28 */ "constraint_list ::= table_constraint",
 /*  29 */ "table_constraint ::= PRIMARY KEY LEFTPARENTHESIS name RIGHTPARENTHESIS",
 /*  30 */ "table_constraint ::= UNIQUE LEFTPARENTHESIS unique_list RIGHTPARENTHESIS",
 /*  31 */ "cmd ::= DROP TABLE table_name",
 /*  32 */ "opt_where_clause ::=",
 /*  33 */ "opt_where_clause ::= where_clause",
 /*  34 */ "where_clause ::= WHERE expr",
 /*  35 */ "expr ::= stringvalue",
 /*  36 */ "expr ::= name DOT name",
 /*  37 */ "expr ::= name",
 /*  38 */ "expr ::= INTEGER",
 /*  39 */ "expr ::= FLOAT",
 /*  40 */ "expr ::= LEFTPARENTHESIS expr RIGHTPARENTHESIS",
 /*  41 */ "expr ::= NOT expr",
 /*  42 */ "expr ::= expr EQ expr",
 /*  43 */ "expr ::= expr NE expr",
 /*  44 */ "expr ::= expr LT expr",
 /*  45 */ "expr ::= expr GT expr",
 /*  46 */ "expr ::= expr LE expr",
 /*  47 */ "expr ::= expr GE expr",
 /*  48 */ "expr ::= expr AND expr",
 /*  49 */ "expr ::= expr OR expr",
 /*  50 */ "table_name ::= name",
 /*  51 */ "table_list ::= table_list COMMA table_name",
 /*  52 */ "table_list ::= table_name",
 /*  53 */ "cmd ::= SELECT select_column_list FROM table_list opt_where_clause",
 /*  54 */ "cmd ::= INSERT into table_name VALUES LEFTPARENTHESIS valuelist RIGHTPARENTHESIS",
 /*  55 */ "into ::=",
 /*  56 */ "into ::= INTO",
 /*  57 */ "stringvalue ::= STRING",
 /*  58 */ "valuelist ::= valuelist COMMA value",
 /*  59 */ "valuelist ::= value",
 /*  60 */ "value ::= FLOAT",
 /*  61 */ "value ::= INTEGER",
 /*  62 */ "value ::= stringvalue",
 /*  63 */ "select_column_list ::= TIMES",
 /*  64 */ "select_column_list ::= select_column_list COMMA full_name",
 /*  65 */ "select_column_list ::= full_name",
 /*  66 */ "full_name ::= name DOT name",
 /*  67 */ "full_name ::= rawcolumn",
 /*  68 */ "cmd ::= DELETE FROM table_name opt_where_clause",
 /*  69 */ "cmd ::= CREATE INDEX name ON table_name LEFTPARENTHESIS rawcolumnlist RIGHTPARENTHESIS",
 /*  70 */ "cmd ::= DROP INDEX name ON table_name",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parser and ParserFree.
*/
void *ParserAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* SEMICOLON */
    case 2: /* CREATE */
    case 3: /* TABLE */
    case 4: /* LEFTPARENTHESIS */
    case 5: /* RIGHTPARENTHESIS */
    case 6: /* COMMA */
    case 7: /* INT_TYPE */
    case 8: /* CHAR_TYPE */
    case 9: /* INTEGER */
    case 10: /* FLOAT_TYPE */
    case 11: /* PRIMARY */
    case 12: /* KEY */
    case 13: /* UNIQUE */
    case 14: /* NAME */
    case 15: /* DROP */
    case 16: /* WHERE */
    case 17: /* DOT */
    case 18: /* FLOAT */
    case 19: /* NOT */
    case 20: /* EQ */
    case 21: /* NE */
    case 22: /* LT */
    case 23: /* GT */
    case 24: /* LE */
    case 25: /* GE */
    case 26: /* AND */
    case 27: /* OR */
    case 28: /* SELECT */
    case 29: /* FROM */
    case 30: /* INSERT */
    case 31: /* VALUES */
    case 32: /* INTO */
    case 33: /* STRING */
    case 34: /* TIMES */
    case 35: /* DELETE */
    case 36: /* INDEX */
    case 37: /* ON */
{
#line 3 "parser.y"
delete (yypminor->yy0);
#line 555 "parser.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
#line 26 "parser.y"

    interpreter->error("Stack overflow.");
#line 740 "parser.c"
   ParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 39, 1 },
  { 40, 2 },
  { 40, 1 },
  { 41, 2 },
  { 42, 0 },
  { 42, 2 },
  { 43, 3 },
  { 44, 4 },
  { 46, 3 },
  { 46, 1 },
  { 48, 2 },
  { 51, 3 },
  { 51, 1 },
  { 52, 1 },
  { 49, 2 },
  { 54, 1 },
  { 54, 4 },
  { 54, 1 },
  { 50, 0 },
  { 50, 2 },
  { 50, 1 },
  { 53, 1 },
  { 55, 3 },
  { 55, 1 },
  { 56, 1 },
  { 47, 0 },
  { 47, 2 },
  { 57, 3 },
  { 57, 1 },
  { 58, 5 },
  { 58, 4 },
  { 42, 3 },
  { 59, 0 },
  { 59, 1 },
  { 60, 2 },
  { 61, 1 },
  { 61, 3 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 3 },
  { 61, 2 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 45, 1 },
  { 64, 3 },
  { 64, 1 },
  { 42, 5 },
  { 42, 7 },
  { 66, 0 },
  { 66, 1 },
  { 63, 1 },
  { 67, 3 },
  { 67, 1 },
  { 68, 1 },
  { 68, 1 },
  { 68, 1 },
  { 65, 1 },
  { 65, 3 },
  { 65, 1 },
  { 69, 3 },
  { 69, 1 },
  { 42, 4 },
  { 42, 8 },
  { 42, 5 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 3: /* cmdAndEnd ::= cmd SEMICOLON */
#line 53 "parser.y"
{
  yy_destructor(yypParser,1,&yymsp[0].minor);
}
#line 927 "parser.c"
        break;
      case 5: /* cmd ::= create_table create_table_args */
#line 57 "parser.y"
{
    interpreter->setActionType(Action::CreateTable);
}
#line 934 "parser.c"
        break;
      case 6: /* create_table ::= CREATE TABLE table_name */
#line 61 "parser.y"
{
  yy_destructor(yypParser,2,&yymsp[-2].minor);
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 942 "parser.c"
        break;
      case 7: /* create_table_args ::= LEFTPARENTHESIS columnlist optional_constraint_list RIGHTPARENTHESIS */
#line 64 "parser.y"
{
  yy_destructor(yypParser,4,&yymsp[-3].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 950 "parser.c"
        break;
      case 8: /* columnlist ::= columnlist COMMA column_define */
      case 11: /* rawcolumnlist ::= rawcolumnlist COMMA rawcolumn */ yytestcase(yyruleno==11);
      case 22: /* unique_list ::= unique_list COMMA unique */ yytestcase(yyruleno==22);
      case 26: /* optional_constraint_list ::= COMMA constraint_list */ yytestcase(yyruleno==26);
      case 27: /* constraint_list ::= constraint_list COMMA table_constraint */ yytestcase(yyruleno==27);
      case 51: /* table_list ::= table_list COMMA table_name */ yytestcase(yyruleno==51);
      case 58: /* valuelist ::= valuelist COMMA value */ yytestcase(yyruleno==58);
      case 64: /* select_column_list ::= select_column_list COMMA full_name */ yytestcase(yyruleno==64);
#line 67 "parser.y"
{
  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 964 "parser.c"
        break;
      case 10: /* column_define ::= column column_constraint */
#line 69 "parser.y"
{
    interpreter->newConstraint(*yymsp[-1].minor.yy0,yymsp[0].minor.yy16);
}
#line 971 "parser.c"
        break;
      case 13: /* rawcolumn ::= name */
#line 75 "parser.y"
{
    interpreter->newColumn(*yymsp[0].minor.yy0,Column::Undefined);
}
#line 978 "parser.c"
        break;
      case 14: /* column ::= name type_token */
#line 79 "parser.y"
{
    interpreter->newColumn(*yymsp[-1].minor.yy0, yymsp[0].minor.yy5);
    yygotominor.yy0 = yymsp[-1].minor.yy0;
}
#line 986 "parser.c"
        break;
      case 15: /* type_token ::= INT_TYPE */
#line 86 "parser.y"
{ yygotominor.yy5 = Column::Int;   yy_destructor(yypParser,7,&yymsp[0].minor);
}
#line 992 "parser.c"
        break;
      case 16: /* type_token ::= CHAR_TYPE LEFTPARENTHESIS INTEGER RIGHTPARENTHESIS */
#line 87 "parser.y"
{
    int x = stoi(*yymsp[-1].minor.yy0);
    if(x<=0 || x > 255)
    {
        interpreter->error("Syntax error: Exceed rang of char.");
        yygotominor.yy5 = 0U;
    }
    else
        yygotominor.yy5 = (unsigned)x;
  yy_destructor(yypParser,8,&yymsp[-3].minor);
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1009 "parser.c"
        break;
      case 17: /* type_token ::= FLOAT_TYPE */
#line 97 "parser.y"
{ yygotominor.yy5 = Column::Float;  yy_destructor(yypParser,10,&yymsp[0].minor);
}
#line 1015 "parser.c"
        break;
      case 18: /* column_constraint ::= */
#line 100 "parser.y"
{yygotominor.yy16 = Constraint::None;}
#line 1020 "parser.c"
        break;
      case 19: /* column_constraint ::= PRIMARY KEY */
#line 101 "parser.y"
{yygotominor.yy16 = Constraint::PrimaryKey;  yy_destructor(yypParser,11,&yymsp[-1].minor);
  yy_destructor(yypParser,12,&yymsp[0].minor);
}
#line 1027 "parser.c"
        break;
      case 20: /* column_constraint ::= UNIQUE */
#line 102 "parser.y"
{yygotominor.yy16 = Constraint::Unique;  yy_destructor(yypParser,13,&yymsp[0].minor);
}
#line 1033 "parser.c"
        break;
      case 21: /* name ::= NAME */
#line 104 "parser.y"
{yygotominor.yy0=yymsp[0].minor.yy0;}
#line 1038 "parser.c"
        break;
      case 24: /* unique ::= name */
#line 108 "parser.y"
{
    interpreter->newConstraint(*yymsp[0].minor.yy0,Constraint::Unique);
}
#line 1045 "parser.c"
        break;
      case 29: /* table_constraint ::= PRIMARY KEY LEFTPARENTHESIS name RIGHTPARENTHESIS */
#line 118 "parser.y"
{
    interpreter->newConstraint(*yymsp[-1].minor.yy0,Constraint::PrimaryKey);
  yy_destructor(yypParser,11,&yymsp[-4].minor);
  yy_destructor(yypParser,12,&yymsp[-3].minor);
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1056 "parser.c"
        break;
      case 30: /* table_constraint ::= UNIQUE LEFTPARENTHESIS unique_list RIGHTPARENTHESIS */
#line 122 "parser.y"
{
  yy_destructor(yypParser,13,&yymsp[-3].minor);
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1065 "parser.c"
        break;
      case 31: /* cmd ::= DROP TABLE table_name */
#line 126 "parser.y"
{
    interpreter->setActionType(Action::DropTable);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 1074 "parser.c"
        break;
      case 34: /* where_clause ::= WHERE expr */
#line 137 "parser.y"
{
    interpreter->newCondition(yymsp[0].minor.yy74);
  yy_destructor(yypParser,16,&yymsp[-1].minor);
}
#line 1082 "parser.c"
        break;
      case 35: /* expr ::= stringvalue */
#line 143 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(*yymsp[0].minor.yy0,yymsp[0].minor.yy0->length());
}
#line 1089 "parser.c"
        break;
      case 36: /* expr ::= name DOT name */
#line 147 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(*yymsp[0].minor.yy0,Column::Undefined, *yymsp[-2].minor.yy0);
  yy_destructor(yypParser,17,&yymsp[-1].minor);
}
#line 1097 "parser.c"
        break;
      case 37: /* expr ::= name */
#line 151 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(*yymsp[0].minor.yy0,Column::Undefined);
}
#line 1104 "parser.c"
        break;
      case 38: /* expr ::= INTEGER */
#line 155 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(*yymsp[0].minor.yy0,Column::Int);
}
#line 1111 "parser.c"
        break;
      case 39: /* expr ::= FLOAT */
#line 158 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(*yymsp[0].minor.yy0,Column::Float);
}
#line 1118 "parser.c"
        break;
      case 40: /* expr ::= LEFTPARENTHESIS expr RIGHTPARENTHESIS */
#line 162 "parser.y"
{
    yygotominor.yy74 = yymsp[-1].minor.yy74;
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1127 "parser.c"
        break;
      case 41: /* expr ::= NOT expr */
#line 166 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::Not, yymsp[0].minor.yy74);
  yy_destructor(yypParser,19,&yymsp[-1].minor);
}
#line 1135 "parser.c"
        break;
      case 42: /* expr ::= expr EQ expr */
#line 170 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::Equal,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,20,&yymsp[-1].minor);
}
#line 1143 "parser.c"
        break;
      case 43: /* expr ::= expr NE expr */
#line 173 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::NotEqual,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,21,&yymsp[-1].minor);
}
#line 1151 "parser.c"
        break;
      case 44: /* expr ::= expr LT expr */
#line 176 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::LessThan,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,22,&yymsp[-1].minor);
}
#line 1159 "parser.c"
        break;
      case 45: /* expr ::= expr GT expr */
#line 179 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::GreaterThan,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,23,&yymsp[-1].minor);
}
#line 1167 "parser.c"
        break;
      case 46: /* expr ::= expr LE expr */
#line 182 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::LessEqual,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,24,&yymsp[-1].minor);
}
#line 1175 "parser.c"
        break;
      case 47: /* expr ::= expr GE expr */
#line 185 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::GreaterEqual,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,25,&yymsp[-1].minor);
}
#line 1183 "parser.c"
        break;
      case 48: /* expr ::= expr AND expr */
#line 189 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::And,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,26,&yymsp[-1].minor);
}
#line 1191 "parser.c"
        break;
      case 49: /* expr ::= expr OR expr */
#line 193 "parser.y"
{
    yygotominor.yy74 = interpreter->newCondition(Condition::Or,yymsp[-2].minor.yy74,yymsp[0].minor.yy74); 
  yy_destructor(yypParser,27,&yymsp[-1].minor);
}
#line 1199 "parser.c"
        break;
      case 50: /* table_name ::= name */
#line 203 "parser.y"
{
    interpreter->addTableName(*yymsp[0].minor.yy0);
}
#line 1206 "parser.c"
        break;
      case 53: /* cmd ::= SELECT select_column_list FROM table_list opt_where_clause */
#line 211 "parser.y"
{
    interpreter->setActionType(Action::Select);
    
  yy_destructor(yypParser,28,&yymsp[-4].minor);
  yy_destructor(yypParser,29,&yymsp[-2].minor);
}
#line 1216 "parser.c"
        break;
      case 54: /* cmd ::= INSERT into table_name VALUES LEFTPARENTHESIS valuelist RIGHTPARENTHESIS */
#line 217 "parser.y"
{
  yy_destructor(yypParser,30,&yymsp[-6].minor);
  yy_destructor(yypParser,31,&yymsp[-3].minor);
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1226 "parser.c"
        break;
      case 56: /* into ::= INTO */
#line 220 "parser.y"
{
  yy_destructor(yypParser,32,&yymsp[0].minor);
}
#line 1233 "parser.c"
        break;
      case 57: /* stringvalue ::= STRING */
#line 222 "parser.y"
{
    yygotominor.yy0 = new string(yymsp[0].minor.yy0->substr(1,yymsp[0].minor.yy0->length()-2));
    delete yymsp[0].minor.yy0;
}
#line 1241 "parser.c"
        break;
      case 60: /* value ::= FLOAT */
#line 230 "parser.y"
{
    interpreter->newColumn(*yymsp[0].minor.yy0, Column::Float);
}
#line 1248 "parser.c"
        break;
      case 61: /* value ::= INTEGER */
#line 233 "parser.y"
{
    interpreter->newColumn(*yymsp[0].minor.yy0, Column::Int);
}
#line 1255 "parser.c"
        break;
      case 62: /* value ::= stringvalue */
#line 236 "parser.y"
{
    interpreter->newColumn(*yymsp[0].minor.yy0, yymsp[0].minor.yy0->length());
}
#line 1262 "parser.c"
        break;
      case 63: /* select_column_list ::= TIMES */
#line 240 "parser.y"
{  yy_destructor(yypParser,34,&yymsp[0].minor);
}
#line 1268 "parser.c"
        break;
      case 66: /* full_name ::= name DOT name */
#line 246 "parser.y"
{
    interpreter->newColumn(*yymsp[0].minor.yy0,Column::Undefined, *yymsp[-2].minor.yy0);
  yy_destructor(yypParser,17,&yymsp[-1].minor);
}
#line 1276 "parser.c"
        break;
      case 68: /* cmd ::= DELETE FROM table_name opt_where_clause */
#line 254 "parser.y"
{
    interpreter->setActionType(Action::Delete);
  yy_destructor(yypParser,35,&yymsp[-3].minor);
  yy_destructor(yypParser,29,&yymsp[-2].minor);
}
#line 1285 "parser.c"
        break;
      case 69: /* cmd ::= CREATE INDEX name ON table_name LEFTPARENTHESIS rawcolumnlist RIGHTPARENTHESIS */
#line 260 "parser.y"
{
    interpreter->setActionType(Action::CreateIndex);
    interpreter->addIndexName(*yymsp[-5].minor.yy0);
  yy_destructor(yypParser,2,&yymsp[-7].minor);
  yy_destructor(yypParser,36,&yymsp[-6].minor);
  yy_destructor(yypParser,37,&yymsp[-4].minor);
  yy_destructor(yypParser,4,&yymsp[-2].minor);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 1298 "parser.c"
        break;
      case 70: /* cmd ::= DROP INDEX name ON table_name */
#line 267 "parser.y"
{
    interpreter->setActionType(Action::DropIndex);
    interpreter->addIndexName(*yymsp[-2].minor.yy0);
  yy_destructor(yypParser,15,&yymsp[-4].minor);
  yy_destructor(yypParser,36,&yymsp[-3].minor);
  yy_destructor(yypParser,37,&yymsp[-1].minor);
}
#line 1309 "parser.c"
        break;
      default:
      /* (0) start ::= cmdList */ yytestcase(yyruleno==0);
      /* (1) cmdList ::= cmdList cmdAndEnd */ yytestcase(yyruleno==1);
      /* (2) cmdList ::= cmdAndEnd */ yytestcase(yyruleno==2);
      /* (4) cmd ::= */ yytestcase(yyruleno==4);
      /* (9) columnlist ::= column_define */ yytestcase(yyruleno==9);
      /* (12) rawcolumnlist ::= rawcolumn */ yytestcase(yyruleno==12);
      /* (23) unique_list ::= unique */ yytestcase(yyruleno==23);
      /* (25) optional_constraint_list ::= */ yytestcase(yyruleno==25);
      /* (28) constraint_list ::= table_constraint */ yytestcase(yyruleno==28);
      /* (32) opt_where_clause ::= */ yytestcase(yyruleno==32);
      /* (33) opt_where_clause ::= where_clause */ yytestcase(yyruleno==33);
      /* (52) table_list ::= table_name */ yytestcase(yyruleno==52);
      /* (55) into ::= */ yytestcase(yyruleno==55);
      /* (59) valuelist ::= value */ yytestcase(yyruleno==59);
      /* (65) select_column_list ::= full_name */ yytestcase(yyruleno==65);
      /* (67) full_name ::= rawcolumn */ yytestcase(yyruleno==67);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParserARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 9 "parser.y"

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
#line 1403 "parser.c"
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParserTOKENTYPE yyminor       /* The value for the token */
  ParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
