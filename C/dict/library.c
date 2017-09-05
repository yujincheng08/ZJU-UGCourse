#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "phonetic.h"

typedef struct _CUR
{
    const char *cur;
    struct _CUR *next;
    struct _CUR *previous;
}CUR;
//cursor, a list of the pages first charactor of the interpret

static CUR *cur = NULL;
//the list object of cursor
static FILE *lib = NULL;
//file pointer of the lib file
static FILE *hz = NULL;
//file pointer of the hz file, which is use to print chinese charactor
static WORD *Word = NULL;
//current word user selected

static void deleteWord()
{
    //before selecting another word, free the old one
	free(Word->word);
	free(Word->interpret);
	free(Word);
    //also the cursor
    while(cur->previous)
        cur = cur->previous;//seek the first element
    while(cur)
    {
        CUR *next = cur->next;
        free(cur);
        cur = next;
    }
    cur = NULL;
    Word = NULL;
}

void readWord(const INDEX *index)
{
    //read word via index from lib file
	int wordLen;
    if(Word) deleteWord();//delete the old word
	Word = malloc(sizeof(WORD)); //new object
	wordLen = entryLen(index->entry);//get its entry length
	Word->word = malloc((wordLen+1)*sizeof(char));//the length should be 1 bit longer to store '\0'
    //cannot be replace by strdup() for the string might not end with '\0'
	if(!Word->word)
		return;
	strncpy(Word->word,index->entry,wordLen);//copy it
	Word->word[wordLen]='\0';
	fseek(lib,index->lib_offset,0);//set the cursor of the file
	Word->interpret = malloc((index->xlat_len+index->exam_len+3)*sizeof(char));
    //3 bit longer to contian '\0' and 2 '\0' between the interpret and example
	if(!Word->interpret)
    {
        deleteWord();//fail to alloc memory, delete the word object
		return;
    }
	fread(Word->interpret,sizeof(char),index->xlat_len,lib);//read interpret from the file
	Word->interpret[index->xlat_len]='\n';
    Word->interpret[index->xlat_len+1]='\n';//add two '\n'
	fseek(lib,index->lib_offset,index->xlat_len);
	fread(Word->interpret+index->xlat_len+2,sizeof(char),index->exam_len,lib);
    //read example
	Word->interpret[index->xlat_len+index->exam_len+2]='\0';
    //add the '\0'
    cur = malloc(sizeof(CUR));
    cur->previous = NULL;
    cur->next = NULL;
    cur->cur = Word->interpret;
    //a new list
}

void clearWord()
{
    setfillstyle(SOLID_FILL,BLACK);
    bar(223,33,_width-13,_height-13);
    //crear the word from the screen
}

static void printPhonetic(int x,int y,char p)
{
    //print the phonetic on (x,y)
    char str[2]={0,0};
    if(p>='/'&&p<='9')//if it is a phonetic charactor
    {
        int i,j;
        const char *c = phonetic[p - 0x2F];
        for(j=0;j<16;j++)
            for(i=0;i<8;i++)
                if(c[j]&(1<<(8-i)))
                    putpixel(x+i-1,y+j,WHITE);
    }
    else// a normal charactor
    {
        str[0]=p;
        outtextxy(x,y,str);
    }
}

static void printKanji(int x,int y,const char kanji[2])
{
    //print a chinese charactor form the hz file
    //easy to realize
   int offset = ((kanji[0] - 0xA1) * 94 + (kanji[1] - 0xA1)) * 0x20;
   int i, j;
   byte buf[16][2];
   word sixteen_dots;
   fseek(hz, offset, SEEK_SET);
   fread(buf, 1, 2*16, hz);
   for(i=0; i<16; i++)
   {
      sixteen_dots = buf[i][0]<<8 | buf[i][1];
      for(j=0; j<16; j++)
         if(sixteen_dots  &  1<<(15-j))
            putpixel(x+j, y+i, WHITE);
   }
}

static void printInterpret()
{
    //print the interpret and example
    const char *pstr = cur->cur;//start from the cursor
    int x,y,isPho=0;
    char str[2]={0,0};
    outtextxy(350,50,Word->word);//first, the word itself
    for(y=80;y<_height-20&&*pstr;y+=20) //the coordinate range of y
    {
        for(x=240;x<_width-40&&*pstr;x+=8,pstr++) //range of x
            if(*pstr>0xA0) //encounter a chinese charactor
            {
                isPho=0;//sure it cannot be a part of phonetic
                printKanji(x,y,pstr++);
                x+=8;//add more 8 offset
            }
            else
            {
                if(*pstr=='\n') //encounter '\n'
                {
                    isPho=0;//cannot be a part of phonetic
                    pstr++;//skip
                    break;
                }
                if(*pstr=='\r')
                    continue;//new line
                if(*pstr==']')
                    isPho=0;//phonetic ended
                if(isPho)
                    printPhonetic(x,y,*pstr);//in the part of phonetic, print it
                else
                {
                    str[0]=*pstr;
                    outtextxy(x,y,str);//normal charactor
                }
                if(*pstr=='[')
                    isPho=1;//phonetic begins
            }
        
    }
    if(*pstr)//cannot print all in one page, need to create a cursor
    {
        cur->next = malloc(sizeof(CUR));
        cur->next->cur = pstr;//store the breakpoint
        cur->next->previous = cur;
        cur->next->next=NULL;
    }
}

void nextPage()
{
    if(cur&&cur->next)//if it does have next page
    {
        cur = cur->next;//set cursor to the next page
        clearWord();//clear the screen
        printInterpret();//and print
    }
}

void previousPage()
{
    //similar with nextPage()
    if(cur&&cur->previous)
    {
        cur = cur->previous;
        clearWord();
        printInterpret();
    }
}


void printWord(const INDEX *index)
{
    //read word from file and print them
    readWord(index);
    clearWord();
    if(!Word)
        return;
    printInterpret();
    
}



int initLib()
{
    //open the lib file and hz file
    lib = fopen("dict.lib","rb");
    hz = fopen("hzk16", "rb");
    return (int)lib&&(int)hz;
}
