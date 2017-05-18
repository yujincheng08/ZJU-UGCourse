//
// Created by laikitman on 5/17/17.
//

#ifndef LEMONADE_MINISQLDEF_H
#define LEMONADE_MINISQLDEF_H


#include <stdint.h>

typedef struct Token Token;
typedef struct Parser Parser;
typedef struct NameList NameList;
struct Token {
    const char *text;
    uint32_t n;
};
struct NameList {
    uint32_t size;
    Token *list;
};
struct Parser {

};
void miniSqlParser(
        void *yyp,                   /* The parser */
        int yymajor,                 /* The major token code number */
        Token yyminor,       /* The value for the token */
        Parser *pParser               /* Optional %extra_argument parameter */
);
void *miniSqlParserAlloc(void *(*mallocProc)(uint64_t));
void miniSqlParserFree(void*, void(*)(void*));
void miniSqlLog(Parser *parser, const char *log);
void miniSqlError(Parser *parser, const char *error);
#endif //LEMONADE_MINISQLDEF_H
