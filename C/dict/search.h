#ifndef SEARCH_H
#define SEARCH_H
#include "index.h"

//this file takes over the task to search the matching words in the index and create the list to store the result

//list struct that stores the searching result
typedef struct _LIST
{
	const INDEX *index;//store the index objext
    struct _LIST *previous;
	struct _LIST *next;
    short prefix;//refers to the number of letters that has been matched
    short len;//store the length of the entry
}LIST;
//search types
//EXACT refers to exact search and FUZZY refers to fuzzy search
typedef enum SearchType{EXACT,FUZZY} searchType;
//search the given string and return search type
extern searchType search(const char *);
//used to realize the length of the result list
extern int getListLen();
//it returns the list of the searching result
extern const LIST* getList();
#endif
