/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 21 "parse.y"

#include "parse.h"
#include "miniSqlDef.h"
#include <assert.h>
#line 33 "parse.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    miniSqlParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is miniSqlParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    miniSqlParserARG_SDECL     A static variable declaration for the %extra_argument
**    miniSqlParserARG_PDECL     A parameter declaration for the %extra_argument
**    miniSqlParserARG_STORE     Code to store %extra_argument into yypParser
**    miniSqlParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 50
#define YYACTIONTYPE unsigned char
#define miniSqlParserTOKENTYPE Token
typedef union {
  int yyinit;
  miniSqlParserTOKENTYPE yy0;
  int yy32;
  NameList* yy40;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define miniSqlParserARG_SDECL Parser *pParser;
#define miniSqlParserARG_PDECL ,Parser *pParser
#define miniSqlParserARG_FETCH Parser *pParser = yypParser->pParser
#define miniSqlParserARG_STORE yypParser->pParser = pParser
#define YYNSTATE             60
#define YYNRULE              33
#define YY_MAX_SHIFT         59
#define YY_MIN_SHIFTREDUCE   60
#define YY_MAX_SHIFTREDUCE   92
#define YY_MIN_REDUCE        93
#define YY_MAX_REDUCE        125
#define YY_ERROR_ACTION      126
#define YY_ACCEPT_ACTION     127
#define YY_NO_ACTION         128
/************* End control #defines *******************************************/

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
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
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
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (158)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   122,   18,   17,  122,  122,   22,    5,   15,  122,    6,
 /*    10 */   122,  122,  122,  122,  121,  122,  108,  121,  121,  114,
 /*    20 */     1,   46,  121,   28,  121,  121,  121,  121,   99,  121,
 /*    30 */    39,   10,   99,   11,   21,   53,   19,   99,   42,   99,
 /*    40 */   127,    2,   23,   59,   58,   16,   41,   50,  103,   47,
 /*    50 */    18,   17,  103,    5,  113,  113,    7,   54,   56,   37,
 /*    60 */    36,   29,   25,   59,   58,   16,  102,  101,   97,   97,
 /*    70 */   102,  101,   40,   45,   30,  102,  101,  102,  101,   32,
 /*    80 */    33,  100,   98,  112,  112,  100,   98,  116,   44,  116,
 /*    90 */   100,   98,  100,   98,  117,  115,  117,  115,   46,   96,
 /*   100 */    96,   35,   50,  120,   47,  125,  116,  120,  107,  125,
 /*   110 */    48,    3,  107,  117,  115,   42,   57,   26,  110,  106,
 /*   120 */    31,  109,  110,  106,   12,  109,  124,  123,  105,  104,
 /*   130 */   124,   14,  105,  104,  119,  111,    8,   43,  119,   27,
 /*   140 */    94,    9,   34,    4,   13,   51,   20,   49,   52,   95,
 /*   150 */    55,  118,   93,   24,   95,   95,   95,   38,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,   17,   18,    4,    5,   44,   39,   40,    9,   42,
 /*    10 */    11,   12,   13,   14,    1,   16,    5,    4,    5,    0,
 /*    20 */     9,    2,    9,   43,   11,   12,   13,   14,    5,   16,
 /*    30 */    43,   48,    9,   38,   11,   12,   13,   14,   19,   16,
 /*    40 */    31,   32,   33,   34,   35,   36,   39,   14,    5,   16,
 /*    50 */    17,   18,    9,   39,   17,   18,   42,   14,   41,   16,
 /*    60 */    46,   47,   33,   34,   35,   36,    5,    5,   17,   18,
 /*    70 */     9,    9,    6,   39,   39,   14,   14,   16,   16,   39,
 /*    80 */    45,    5,    5,   17,   18,    9,    9,    0,    6,    2,
 /*    90 */    14,   14,   16,   16,    0,    0,    2,    2,    2,   17,
 /*   100 */    18,   47,   14,    5,   16,    5,   19,    9,    5,    9,
 /*   110 */    39,    4,    9,   19,   19,   19,   37,    8,    5,    5,
 /*   120 */     5,    5,    9,    9,    9,    9,    5,    5,    5,    5,
 /*   130 */     9,    9,    9,    9,    5,    1,    3,    7,    9,    8,
 /*   140 */     4,    3,    5,    4,    4,   10,    5,   15,    4,    1,
 /*   150 */     5,    1,    1,    1,   49,   49,   49,   15,
};
#define YY_SHIFT_USE_DFLT (158)
#define YY_SHIFT_COUNT    (59)
#define YY_SHIFT_MIN      (-16)
#define YY_SHIFT_MAX      (152)
static const short yy_shift_ofst[] = {
 /*     0 */    96,   33,   19,  -16,  -16,   23,   43,   43,   66,   82,
 /*    10 */   -16,  -16,  -16,  -16,   88,   11,  107,   -1,   13,   61,
 /*    20 */    62,   76,   77,   87,   94,   95,   37,   51,   98,  100,
 /*    30 */   103,  113,  114,  115,  116,  121,  122,  123,  124,  129,
 /*    40 */   109,  134,  133,  131,  130,  136,  138,  139,  137,  140,
 /*    50 */   132,  141,  135,  144,  142,  148,  145,  150,  151,  152,
};
#define YY_REDUCE_USE_DFLT (-40)
#define YY_REDUCE_COUNT (16)
#define YY_REDUCE_MIN   (-39)
#define YY_REDUCE_MAX   (79)
static const signed char yy_reduce_ofst[] = {
 /*     0 */     9,   14,   29,  -33,   35,  -39,  -20,  -13,  -17,   -5,
 /*    10 */     7,   34,   40,   71,   54,   17,   79,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    10 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    20 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    30 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    40 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    50 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
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
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  miniSqlParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
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
void miniSqlParserTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "LP",            "RP",            "IF",            "NOT",         
  "EXISTS",        "COMMA",         "INTEGER",       "INT_TYPE",    
  "CHAR_TYPE",     "FLOAT_TYPE",    "PRIMARY",       "KEY",         
  "UNIQUE",        "STRING",        "ID",            "DROP",        
  "OR",            "AND",           "IS",            "NE",          
  "EQ",            "BETWEEN",       "GT",            "LE",          
  "LT",            "GE",            "error",         "input",       
  "cmdlist",       "cmd_and_end",   "singlecmd",     "cmd",         
  "create_table",  "create_table_args",  "if_not_exists",  "name",        
  "columnlist",    "optional_constraint_list",  "column_name",   "column_constraint",
  "type_token",    "name_list",     "constraint_list",  "table_constraint",
  "if_exists",   
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "singlecmd ::= cmd",
 /*   1 */ "create_table ::= CREATE TABLE if_not_exists name",
 /*   2 */ "create_table_args ::= LP columnlist optional_constraint_list RP",
 /*   3 */ "if_not_exists ::=",
 /*   4 */ "if_not_exists ::= IF NOT EXISTS",
 /*   5 */ "column_name ::= name type_token",
 /*   6 */ "type_token ::=",
 /*   7 */ "type_token ::= INT_TYPE",
 /*   8 */ "type_token ::= CHAR_TYPE LP INTEGER RP",
 /*   9 */ "type_token ::= FLOAT_TYPE",
 /*  10 */ "column_constraint ::=",
 /*  11 */ "column_constraint ::= PRIMARY KEY",
 /*  12 */ "column_constraint ::= UNIQUE",
 /*  13 */ "name_list ::= name_list COMMA name",
 /*  14 */ "name_list ::= name",
 /*  15 */ "optional_constraint_list ::=",
 /*  16 */ "table_constraint ::= PRIMARY KEY LP name RP",
 /*  17 */ "table_constraint ::= UNIQUE LP name_list RP",
 /*  18 */ "cmd ::= DROP TABLE if_exists name",
 /*  19 */ "if_exists ::=",
 /*  20 */ "if_exists ::= IF EXISTS",
 /*  21 */ "input ::= cmdlist",
 /*  22 */ "cmdlist ::= cmdlist cmd_and_end",
 /*  23 */ "cmdlist ::= cmd_and_end",
 /*  24 */ "cmd_and_end ::= singlecmd SEMICOLON",
 /*  25 */ "cmd ::= create_table create_table_args",
 /*  26 */ "columnlist ::= columnlist COMMA column_name column_constraint",
 /*  27 */ "columnlist ::= column_name column_constraint",
 /*  28 */ "name ::= STRING",
 /*  29 */ "name ::= ID",
 /*  30 */ "optional_constraint_list ::= COMMA constraint_list",
 /*  31 */ "constraint_list ::= constraint_list COMMA table_constraint",
 /*  32 */ "constraint_list ::= table_constraint",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to miniSqlParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void miniSqlParserInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
}

#ifndef miniSqlParser_ENGINEALWAYSONSTACK
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
** to miniSqlParser and miniSqlParserFree.
*/
void *miniSqlParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) miniSqlParserInit(pParser);
  return pParser;
}
#endif /* miniSqlParser_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  miniSqlParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
    case 45: /* name_list */
{
#line 79 "parse.y"

#line 532 "parse.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void miniSqlParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef miniSqlParser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void miniSqlParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  miniSqlParserFinalize(p);
  (*freeProc)(p);
}
#endif /* miniSqlParser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int miniSqlParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
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
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
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
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
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
static void yyStackOverflow(yyParser *yypParser){
   miniSqlParserARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
#line 15 "parse.y"

    miniSqlError(pParser, "parser stack overflow");
#line 714 "parse.c"
/******** End %stack_overflow code ********************************************/
   miniSqlParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  miniSqlParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 34, 1 },
  { 36, 4 },
  { 37, 4 },
  { 38, 0 },
  { 38, 3 },
  { 42, 2 },
  { 44, 0 },
  { 44, 1 },
  { 44, 4 },
  { 44, 1 },
  { 43, 0 },
  { 43, 2 },
  { 43, 1 },
  { 45, 3 },
  { 45, 1 },
  { 41, 0 },
  { 47, 5 },
  { 47, 4 },
  { 35, 4 },
  { 48, 0 },
  { 48, 2 },
  { 31, 1 },
  { 32, 2 },
  { 32, 1 },
  { 33, 2 },
  { 35, 2 },
  { 40, 4 },
  { 40, 2 },
  { 39, 1 },
  { 39, 1 },
  { 41, 2 },
  { 46, 3 },
  { 46, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  miniSqlParserARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* singlecmd ::= cmd */
#line 36 "parse.y"
{
    miniSqlLog(pParser, "finish a command");
}
#line 888 "parse.c"
        break;
      case 1: /* create_table ::= CREATE TABLE if_not_exists name */
#line 43 "parse.y"
{
    //
}
#line 895 "parse.c"
        break;
      case 2: /* create_table_args ::= LP columnlist optional_constraint_list RP */
      case 5: /* column_name ::= name type_token */ yytestcase(yyruleno==5);
      case 13: /* name_list ::= name_list COMMA name */ yytestcase(yyruleno==13);
      case 14: /* name_list ::= name */ yytestcase(yyruleno==14);
      case 16: /* table_constraint ::= PRIMARY KEY LP name RP */ yytestcase(yyruleno==16);
      case 17: /* table_constraint ::= UNIQUE LP name_list RP */ yytestcase(yyruleno==17);
      case 18: /* cmd ::= DROP TABLE if_exists name */ yytestcase(yyruleno==18);
#line 47 "parse.y"
{

}
#line 908 "parse.c"
        break;
      case 3: /* if_not_exists ::= */
      case 10: /* column_constraint ::= */ yytestcase(yyruleno==10);
      case 19: /* if_exists ::= */ yytestcase(yyruleno==19);
#line 52 "parse.y"
{yymsp[1].minor.yy32 = 0;}
#line 915 "parse.c"
        break;
      case 4: /* if_not_exists ::= IF NOT EXISTS */
#line 53 "parse.y"
{yymsp[-2].minor.yy32 = 1;}
#line 920 "parse.c"
        break;
      case 6: /* type_token ::= */
      case 7: /* type_token ::= INT_TYPE */ yytestcase(yyruleno==7);
      case 8: /* type_token ::= CHAR_TYPE LP INTEGER RP */ yytestcase(yyruleno==8);
      case 9: /* type_token ::= FLOAT_TYPE */ yytestcase(yyruleno==9);
#line 64 "parse.y"
{}
#line 928 "parse.c"
        break;
      case 11: /* column_constraint ::= PRIMARY KEY */
      case 20: /* if_exists ::= IF EXISTS */ yytestcase(yyruleno==20);
#line 71 "parse.y"
{yymsp[-1].minor.yy32 = 1;}
#line 934 "parse.c"
        break;
      case 12: /* column_constraint ::= UNIQUE */
#line 72 "parse.y"
{yymsp[0].minor.yy32 = 2;}
#line 939 "parse.c"
        break;
      case 15: /* optional_constraint_list ::= */
#line 90 "parse.y"
{yymsp[1].minor.yy0.n = 0; yymsp[1].minor.yy0.text = 0;}
#line 944 "parse.c"
        break;
      default:
      /* (21) input ::= cmdlist */ yytestcase(yyruleno==21);
      /* (22) cmdlist ::= cmdlist cmd_and_end */ yytestcase(yyruleno==22);
      /* (23) cmdlist ::= cmd_and_end */ yytestcase(yyruleno==23);
      /* (24) cmd_and_end ::= singlecmd SEMICOLON */ yytestcase(yyruleno==24);
      /* (25) cmd ::= create_table create_table_args */ yytestcase(yyruleno==25);
      /* (26) columnlist ::= columnlist COMMA column_name column_constraint */ yytestcase(yyruleno==26);
      /* (27) columnlist ::= column_name column_constraint */ yytestcase(yyruleno==27);
      /* (28) name ::= STRING */ yytestcase(yyruleno==28);
      /* (29) name ::= ID */ yytestcase(yyruleno==29);
      /* (30) optional_constraint_list ::= COMMA constraint_list */ yytestcase(yyruleno==30);
      /* (31) constraint_list ::= constraint_list COMMA table_constraint */ yytestcase(yyruleno==31);
      /* (32) constraint_list ::= table_constraint */ yytestcase(yyruleno==32);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
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
  miniSqlParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  miniSqlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  miniSqlParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  miniSqlParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 10 "parse.y"

    // TODO: add location of error
    miniSqlError(pParser, "syntax error");
#line 1019 "parse.c"
/************ End %syntax_error code ******************************************/
  miniSqlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  miniSqlParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  miniSqlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "miniSqlParserAlloc" which describes the current state of the parser.
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
void miniSqlParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  miniSqlParserTOKENTYPE yyminor       /* The value for the token */
  miniSqlParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  miniSqlParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
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
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
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
      yy_syntax_error(yypParser,yymajor, yyminor);
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
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
