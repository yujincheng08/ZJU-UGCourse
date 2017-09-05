#include <graphics.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*Makefile:
 *hitword.exe : hitword.c
 *wcl386 /fe=hitword.exe hitword.c
 */
 
/*Written by YU Jincheng
 *3150101155
 */

#define MAXWORDS 26
#define MAXSCREENLINES 23
#define MAXLEN 20
#define TIMEOUT 500
#define ARROWTIMEOUT 125

const char KEY[35]={'q','w','e','r','t','y','u','i','o','p','[',']','\n',0x11,'a','s','d','f','g','h','j','k','l',';','\'','~',0x10,'\\','z','x','c','v','b','n','m'};

typedef struct
{
	short x;
	short y;
	char *word;
	int len;
}WORD;

typedef struct
{
	char entry[MAXLEN];
	long lib_offset;
	short int xlat_len;
	short int exam_len;
}IDX;

typedef struct _ARROW
{
	short x;
	short y;
	char buff;
	int color;
	WORD* word;
	struct _ARROW* next;
}ARROW;

const char	BGreen	= BLACK	<<4	| GREEN, 
			BWhite	= BLACK	<<4	| WHITE, 
			BGray	= BLUE	<<4	| WHITE;
int screenWords=0;
int index[26][2];
int stop = 0;
int totalNum = 0, 
	hitNum = 0, 
	lostNum = 0;
int needRefresh = 1;
int timeout = 0;
double hit_rate=0.0;
InterruptFunctionPointer old_9h;
InterruptFunctionPointer old_8h;
ARROW *arrows=NULL;
WORD words[MAXWORDS],sample={0,0,NULL,-1},*hitWord;
IDX *pdic;

IDX* read_dic()
{
   FILE *fp;
   IDX *pdic;
   int len, entries;
   int i, begin;
   fp = fopen("dict.idx", "rb");
   if(fp == NULL)
      return NULL;
   fseek(fp, 0, SEEK_END);
   len = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   entries = len / sizeof(IDX);
   pdic = malloc(entries*sizeof(IDX));
   if(pdic == NULL)
      return NULL;
   fread(pdic, sizeof(IDX), entries, fp);
   fclose(fp);
   begin = 0;
   for(i=0; i<26; i++)
   {
      while(begin < entries)
      {
         if((pdic[begin].entry[0] | 0x20) == 'a'+i)
            break;
         begin++;
      }
      index[i][0] = begin;
      if(i>0)
         index[i-1][1] = begin - index[i-1][0];
   }
   index[i-1][1] = entries - begin;
   return pdic;
}

char *getVP(int x,int y)
{
	return _vp + (y*_width+x)*2;
}

int entrylen(char *entry)
{
	int i=0;
	for(;i<MAXLEN;i++)
		if(entry[i]=='\0')
			break;
	return i;
}

int isAllAlpha(char *s)
{
	int i, n = entrylen(s);
	for(i=0;i<n && islower(s[i]);i++);
	return i==n;
}

void putword(WORD *word)
{
	char *vp = getVP(word->x,word->y);
	int i = 0;
	if(word->len==-1)
		return;
	while(word->word[i]!='\0')
	{
		*vp = word->word[i];
		*(vp+1) = i<word->len?BGreen:BWhite;
		vp+=2;
		i++;
	}
}

void initwords()
{
	int i = 0;
	for(;i<MAXWORDS;i++)
		memcpy(words+i,&sample,sizeof(sample));
}

int countPos()
{
	int pos = 30+hitNum/5;
	pos = pos>60?60:pos;
	pos += 2*hit_rate/5;
	if(totalNum==0) pos=100;
	return pos;
}
void generateWord()
{
	int i = 0, len=0;
	char *newWord = NULL;
	int offset = random( MAXWORDS-screenWords);
	
	if(screenWords>MAXSCREENLINES||random(100)>countPos())
		return;
	for(i=0;i<MAXWORDS;i++)
	{
		if(words[i].len==-1&&!offset)
			break;
		if(words[i].len==-1)
			offset--;
	}
	do
	{
		newWord = pdic[random(index[i][1])+index[i][0]].entry;
	}while(!isAllAlpha(newWord));
	len = entrylen(newWord);
	words[i].x = random(_width-len);
	words[i].y = 0;
	words[i].len=0;
	words[i].word = (char*)malloc(len+1);
	memcpy(words[i].word,newWord,len);
	words[i].word[len]='\0';
	screenWords++;
	totalNum++;
}

void interrupt keyEvent()
{
	unsigned char key = inportb(0x60) - 0x10;
	if(key==0x71) stop=1;
	if(key<35&&isalpha(key=KEY[key]))
	{
		if(hitWord)
		{
			if(hitWord->word[hitWord->len]==key)
				hitWord->len++;
			else
			{
				hitWord->len=0;	
				hitWord=NULL;
			}
		}
		else
		{
			WORD *hit = &words[key-'a'];
			if(hit->len!=-1)
			{
				hitWord=hit;
				hit->len=1;
			}
		}
	}
	needRefresh=1;
	old_9h();
	outportb(0x20, 0x20);
}

void set_clock_frequency(int n)
{
   outportb(0x43, 0x36);
   outportb(0x40, (1193180/n) & 0xFF);
   outportb(0x40, (1193180/n) >> 8);
}
void moveDown()
{
	int i=0;
	for(;i<MAXWORDS;i++)
	{
		words[i].y++;
		if(words[i].y>MAXSCREENLINES&&words[i].len!=-1)
		{
			if(hitWord==words+i)
				hitWord=NULL;
			free(words[i].word);
			memcpy(words+i,&sample,sizeof(sample));
			screenWords--;	
			lostNum++;
		}
	}
}

void showSocre(char *score)
{
	int i=0;
	char *vp = getVP(0,MAXSCREENLINES+1);
	sprintf(score,"words_list=%d; words_hit=%d; "
		"words_lost=%d; hit_rate=%.2lf%%",totalNum,hitNum,lostNum,hit_rate);
	while(score[i]!='\0')
	{
		*vp = score[i];
		*(vp+1) = BGray;
		vp+=2;
		i++;
	}
}
int moveArrow()
{
	ARROW *arrow=arrows,*prearr=NULL,*next=NULL;
	int moved=0;
	while(arrow)
	{
		moved=1;
		if(!prearr)
			prearr=arrow;
		next=arrow->next;
		if(arrow->y==arrow->word->y)
		{
			if(prearr=arrow)
				prearr=next;
			else
				prearr->next=next;
			if(arrow==arrows)
				arrows=next;
			memcpy(arrow->word,&sample,sizeof(sample));
			screenWords--;
			free(arrow);
		}
		else if(!(timeout==0&&arrow->y-arrow->word->y==1))
			arrow->y--;
		arrow=next;
	}
	return moved;
}

void putsArrow()
{
	ARROW *arrow = arrows;
	while(arrow)
	{
		char *vp = getVP(arrow->x,arrow->y+1);
		*vp=arrow->buff;
		*(vp+1)=arrow->color;
		vp=getVP(arrow->x,arrow->y);
		arrow->buff=*vp;
		arrow->color=*(vp+1);
		*vp=0x18;
		*(vp+1) = RED;
		arrow=arrow->next;
	}
}

void refresh()
{
	int i=0;
	needRefresh=0;
	clrscr();
	for(;i<MAXWORDS;i++)
		putword(words+i);
	putsArrow();
}

void refreshArrow()
{
	if(hitWord->len==strlen(hitWord->word))
	{
		WORD *old=hitWord;
		ARROW *arrow=arrows,*newArrow;
		hitWord=NULL;
		while(arrow&&arrow->next)
			arrow=arrow->next;
		newArrow = malloc(sizeof(ARROW));
		newArrow->next=NULL;
		if(arrow)
			arrow->next=newArrow;
		else
			arrows=newArrow;
		newArrow->word=old;
		newArrow->x=old->x+old->len/2;
		newArrow->y=MAXSCREENLINES;
		hitNum++;
	}
	if(moveArrow())
		needRefresh=1;
}

void interrupt timeoutEvent()
{
	if(timeout%ARROWTIMEOUT==0)
	{
		refreshArrow();
	}
	if(timeout==0)
	{
		timeout=TIMEOUT-1;
		moveDown();
		generateWord();
		needRefresh=1;
	}
	else timeout--;
	if(needRefresh)
		refresh();
	old_8h();
	outportb(0x20, 0x20);
}

void initGame()
{
	randomize();
	if(!(pdic = read_dic()))
		exit(0);
	clrscr();
	initwords();
	old_9h = getvect(9);
	old_8h = getvect(8);
	setvect(9, keyEvent);
	setvect(8, timeoutEvent);
	set_clock_frequency(1000);
}

void startGame()
{
	char score[80];
	while(!stop)
	{
		if(totalNum)
			hit_rate=hitNum*100.0/totalNum;
		showSocre(score);
	}
}

void endGame()
{
	setvect(9, old_9h);
	setvect(8, old_8h);
	clrscr();
}

int main()
{
	initGame();
	startGame();
	endGame();
	return 0;
}
