#ifndef DICT_H
#define DICT_H
//stuct to store the contents of idx files, which is privided by BlackWhite
typedef struct 
{
   char entry[20];
   long lib_offset;
   short int  xlat_len;
   short int  exam_len;
}INDEX;

extern int initDict();
//initilize dict
extern const INDEX *getIndex();
//used to fetch the index object
extern int getDictLen();
//used to realize the length of the INDEX array
extern int entryLen();
//used to get the length of entry string, which might not ends with '/0'
#endif
