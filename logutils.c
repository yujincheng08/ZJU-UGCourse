//
// Created by laikitman on 5/17/17.
//

#include <stdio.h>
#include "miniSqlDef.h"

void miniSqlError(Parser* pParser, char const *error) {
    printf("%s\n", error);
}

void miniSqlLog(Parser* pParser, char const *log) {
    printf("%s\n", log);
}