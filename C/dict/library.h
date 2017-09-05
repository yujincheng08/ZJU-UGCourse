#ifndef LIBRARY_H
#define LIBRARY_H
#include "index.h"
//struct to store lib file
typedef struct
{
	char *word;
	char *interpret;
}WORD;


/****this file also take over the display task of words and its interpret****/


extern void printWord(const INDEX *);
//print the word from index file
extern void clearWord();
//clear the word from the screen
extern void nextPage();
//next page of interpret if avaliable
extern void previousPage();
//previous page
extern int initLib();
//initilze the library object
#endif
