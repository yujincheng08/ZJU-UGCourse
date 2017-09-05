#include <graphics.h>
#include "queryExp.h"
static char queryExp[70];
//the string, which is shorter than 69 charactors execpt for the '\0'
static short queryLen;
//store the length of the string

static void printQuery()
{
    //print the string on the screen
    //easy job
    setfillstyle(SOLID_FILL,BLACK);
    bar(70,5,_width-10,27);
    setfillstyle(SOLID_FILL,WHITE);
    rectangle(70,5,_width-10,25);
    outtextxy(75,10,queryExp);
}

void deleteChar()
{
    if(queryLen>0)//before deleting, make sure it contains some charactors
    {
        queryExp[--queryLen]='\0';//set the last charactors to '\0' and decrease len
        printQuery();//print it to screen
    }
}

void addChar(char c)
{
    if(queryLen<69) //make sure the array is not full
    {
        queryExp[queryLen++]=c;//add the charactors
        queryExp[queryLen]='\0';//dont forget the '\0'
        printQuery();//print it to screen
    }
}

const char *getQueryExp()
{
    return queryExp;//simply return the array
}

void clearQuery()
{
    queryLen=0;
    queryExp[0]='\0';//simply set the first element to '\0'
    printQuery();//print it, as clearing the screen
}
