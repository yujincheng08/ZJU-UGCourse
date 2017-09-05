#include <graphics.h>
#include <string.h>
#include "index.h"

INDEX *index;
//first element of INDEX array
static int dictLen;
//store the length of the dict, also the length of INDEX array

int entryLen(char *entry)
{
    int wordLen = strlen(entry);
	return wordLen>=20?20:wordLen; //not longer than 20
}


int initDict()
{
	FILE *fp;
	fp = fopen("dict.idx", "rb");
	if(!fp)
	  return 0;
	fseek(fp, 0, SEEK_END);//set the cursor to the end of the file
	dictLen = ftell(fp)/sizeof(INDEX); //calculate the length 
    //by dividing the length of the file and the length of each element
	fseek(fp, 0, SEEK_SET);//set to the beginning
	index = malloc(dictLen*sizeof(INDEX));
    //require memmory
	if(!index)
	  return 0;//failed to apply, tell the source function
	fread(index, sizeof(INDEX), dictLen, fp); //read from the file to the array
	fclose(fp); //close it
    return 1; //success
}

int getDictLen()
{
    return dictLen; //simply return the value
}

const INDEX *getIndex()
{
    return index; // the same
}
