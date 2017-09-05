#ifndef LIST_H
#define LIST_H
#include "search.h"

//this file takes over the list of the querying result and the task to print them on the screen

//interface to search word by the given expression
extern searchType query(const char *);

extern void clearList();
//clear list on the screen

//respond keys user pressed
extern void moveDown();
extern void moveUp();
extern void pageUp();
extern void pageDown();
extern void home();
extern void end();
extern void insert();
extern void del();

//get the current selected element's index
extern const INDEX *getCurIndex();
#endif
