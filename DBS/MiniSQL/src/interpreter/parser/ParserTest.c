#include <stdlib.h>
#include <stdio.h>
#include "miniSqlDef.h"
#include "parse.h"
#include "ParserTest.h"

void testCreateTable() {
    void *pParser = miniSqlParserAlloc(malloc);
    Parser *context = (Parser *) malloc(sizeof(Parser));

    Token token = {};

    /**
     * test case zero: the simplest one
     * should pass
     * create table table_name (name);
     */
    int i = 0;

    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);


    /**
     * test case one : the longest one with all constraint
     * should pass
     * create table if not exists table_name (
     *     name int,
     *     name char(5),
     *     name float,
     *     primary key (name),
     *     unique (name, name)
     *  );
    */

    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);

    miniSqlParser(pParser, TOKEN_LP, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_CHAR_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_INTEGER, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_FLOAT_TYPE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_PRIMARY, token, context);
    miniSqlParser(pParser, TOKEN_KEY, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_COMMA, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 2
     * column definitions followed by table constraints
     * should pass
     * create table if not exists table_name (
     *     name int primary key,
     *     name char(5),
     *     name float UNIQUE,
     *     name int UNIQUE
     *  );
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);

    miniSqlParser(pParser, TOKEN_IF, token, context);
    miniSqlParser(pParser, TOKEN_NOT, token, context);
    miniSqlParser(pParser, TOKEN_EXISTS, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);

    miniSqlParser(pParser, TOKEN_LP, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_PRIMARY, token, context);
    miniSqlParser(pParser, TOKEN_KEY, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_CHAR_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_INTEGER, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_FLOAT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);

    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 3: if not exists is optional
     * should pass
     *
     *  create table if not exists table_name (
     *     name int primary key,
     *     name char(5),
     *     name float UNIQUE,
     *     name int UNIQUE
     *  );
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);

    miniSqlParser(pParser, TOKEN_LP, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_PRIMARY, token, context);
    miniSqlParser(pParser, TOKEN_KEY, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_CHAR_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_INTEGER, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_FLOAT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);

    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 4: missing table name
     * shouldn't pass
     * create table (name);
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 5: missing column name
     * create table table_name (int);
     * shouldn't pass
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 6: two column names are not separated by comma
     * create table table_name (name1 name2);
     * shouldn't pass
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 7: two constraint on the same column
     * create table table_name (name1 int, name2 char(10) PRIMARY KEY UNIQUE, name3 float);
     * shouldn't pass
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);

    miniSqlParser(pParser, TOKEN_LP, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_INT_TYPE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_CHAR_TYPE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_INTEGER, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_PRIMARY, token, context);
    miniSqlParser(pParser, TOKEN_KEY, token, context);
    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_FLOAT_TYPE, token, context);

    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 8: missing semicolon
     * create table table_name (name)
     * shouldn't pass
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test case 9: missing column definition list
     * create table table_name (PRIMARY KEY(name), UNIQUE(name, name2));
     */
    printf("test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_TABLE, token, context);

    miniSqlParser(pParser, TOKEN_LP, token, context);

    miniSqlParser(pParser, TOKEN_PRIMARY, token, context);
    miniSqlParser(pParser, TOKEN_KEY, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_COMMA, token, context);

    miniSqlParser(pParser, TOKEN_UNIQUE, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_COMMA, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);

    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    miniSqlParserFree(pParser, free);
    free(context);
}

void testIndex() {
    void *pParser = miniSqlParserAlloc(malloc);
    Parser *context = (Parser *) malloc(sizeof(Parser));

    Token token = {};

    int i = 0;

    /**
     * test0: the simplest case should pass
     * CREATE INDEX index-name ON table-name (index-column);
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_ON, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test1: column list should pass
     * CREATE INDEX index-name ON table-name (index-column0, index-column1);
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_ON, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_COMMA, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test2: wrong index name with schema should pass
     * CREATE INDEX schema.index-name ON table-name (index-column0, index-column1);
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_DOT, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_ON, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test3: missing column list shouldn't pass
     * CREATE INDEX index-name ON table-name;
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_ON, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test4: table name with schema should pass
     * CREATE INDEX index-name ON schema-name.table-name (index-column0);
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_CREATE, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_ON, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_DOT, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_LP, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_RP, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test5: simplest drop index case should pass
     * DROP INDEX index-name;
     */
    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_DROP, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    /**
     * test6: index name with schema name
     */

    printf("index test case %d:\n", i++);
    miniSqlParser(pParser, TOKEN_DROP, token, context);
    miniSqlParser(pParser, TOKEN_INDEX, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_DOT, token, context);
    miniSqlParser(pParser, TOKEN_STRING, token, context);
    miniSqlParser(pParser, TOKEN_SEMICOLON, token, context);
    miniSqlParser(pParser, 0, token, pParser);

    miniSqlParserFree(pParser, free);
    free(context);
}
