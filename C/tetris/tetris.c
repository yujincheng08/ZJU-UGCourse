#include <graphics.h>

/*Makefile:
 *tetris.exe : tetris.c
 *wcl386 -bcl=pmodew /fe=tetris.exe tetris.c
 */
 
/*Written by YU Jincheng
 *3150101155
 */


#define LEFTMARGIN 320
#define TOPMARGIN 13
#define BLOCKLEN 36
#define LINE 20
#define CLOUMN 10
#define BOTTOMMAGIN TOPMARGIN+(BLOCKLEN+1)*LINE
#define RIGHTMARGIN LEFTMARGIN+CLOUMN*(BLOCKLEN+1)
#define TIMEOUT 1000
#define _SYSDOWN 0x0
#define BLOCK_SIZE 500
#define _SPACE 0x39

typedef struct
{
	int x;
	int y;
}POINT;	

typedef struct
{
	POINT points[4];
}SHAPE;

typedef struct
{
	SHAPE shape[4];
	int roateLen;
	int color;
}TETRISGROUPs;

typedef struct
{
	int filled;
	int color;
}BLOCK;

typedef struct
{
	TETRISGROUPs *group;
	int roate;
	POINT pos;
}TETRIS;

typedef struct
{
	PIC *pic;
	POINT pos;
}BIGTEXT;

TETRISGROUPs allTetris[7]=
{
	{{	{{{0,1},{1,1},{2,1},{3,1}}},
		{{{2,0},{2,1},{2,2},{2,3}}},
		{{{0,2},{1,2},{2,2},{3,2}}},
		{{{1,0},{1,1},{1,2},{1,3}}}},
		4,0x08
	},
	{{	{{{0,0},{0,1},{1,1},{2,1}}},
		{{{1,0},{2,0},{1,1},{1,2}}},
		{{{0,1},{1,1},{2,1},{2,2}}},
		{{{1,0},{1,1},{1,2},{0,2}}}},
		3,0x09
	},
	{{	{{{0,1},{1,1},{2,0},{2,1}}},
		{{{1,0},{1,1},{1,2},{2,2}}},
		{{{0,1},{0,2},{1,1},{2,1}}},
		{{{0,0},{1,0},{1,1},{1,2}}}},
		3,0x02
	},
	{{	{{{0,0},{0,1},{1,0},{1,1}}},
		{{{0,0},{0,1},{1,0},{1,1}}},
		{{{0,0},{0,1},{1,0},{1,1}}},
		{{{0,0},{0,1},{1,0},{1,1}}}},
		2,0x35
	},
	{{	{{{0,1},{1,0},{1,1},{2,0}}},
		{{{1,0},{1,1},{2,1},{2,2}}},
		{{{0,2},{1,1},{1,2},{2,1}}},
		{{{0,0},{0,1},{1,1},{1,2}}}},
		3,0x0C
	},
	{{	{{{0,1},{1,0},{1,1},{2,1}}},
		{{{1,0},{1,1},{1,2},{2,1}}},
		{{{0,1},{1,1},{1,2},{2,1}}},
		{{{0,1},{1,0},{1,1},{1,2}}}},
		3,0x0D
	},
	{{	{{{0,0},{1,0},{1,1},{2,1}}},
		{{{1,1},{1,2},{2,0},{2,1}}},
		{{{0,1},{1,1},{1,2},{2,2}}},
		{{{0,1},{0,2},{1,0},{1,1}}}},
		3,0x2B
	}
};

InterruptFunctionPointer old_9h;
InterruptFunctionPointer old_8h;
int stop=0;
BLOCK map[CLOUMN][LINE],empty={0,WHITE};
int score;
TETRIS *curTetris=NULL,*nextTetris=NULL;
BIGTEXT pauseText[5];
BIGTEXT overText[5];
BIGTEXT introText[14];
int timeout;
int pause = 0;
int score = 0;
int over = 0;
int overShow=0;
int needRefresh = 0;
PIC *nextPic=NULL;
struct picture *old_img=NULL;

POINT toAbsPoint(POINT relative)
{
	POINT p;
	p.x = LEFTMARGIN + relative.x*(BLOCKLEN+1)+1;
	p.y = TOPMARGIN + relative.y*(BLOCKLEN+1)+1;
	return p;
}

TETRIS* generateTetris()
{
	TETRIS* tetris = malloc(sizeof(TETRIS));
	int len ;
	tetris->group = allTetris+rand()%7;
	len = tetris->group->roateLen;
	tetris->roate=rand()%4;
	tetris->pos.x = (CLOUMN-len)/2;
	tetris->pos.y=-1;
	return tetris;
}

POINT calAbsPos(TETRIS *tetris)
{
	POINT p;
	p.x = LEFTMARGIN + tetris->pos.x*(BLOCKLEN+1)+1;
	p.y = TOPMARGIN + tetris->pos.y*(BLOCKLEN+1)+1;

	return toAbsPoint(tetris->pos);
}

void putBlock(POINT p,int color)
{
	setfillstyle(SOLID_FILL,color);
	if(p.y<TOPMARGIN||p.y+BLOCKLEN>BOTTOMMAGIN)
		return;
	bar(p.x,p.y,p.x+BLOCKLEN-1,p.y+BLOCKLEN-1);
}

SHAPE* getShape(TETRIS* tetris)
{
	return tetris->group->shape + tetris->roate%4;
}

POINT calRePos(TETRIS *tetris,int index)
{
	SHAPE *shape = getShape(tetris);
	POINT p;
	p.x = tetris->pos.x+shape->points[index].x;
	p.y = tetris->pos.y+shape->points[index].y; 
	return p;
}

void putTetris(TETRIS *tetris,POINT point)
{
	int i = 0 ,color = tetris->group->color;
	POINT p;
	SHAPE* shape = getShape(tetris);
	for(;i<4;i++)
	{
		p.x = point.x + shape->points[i].x*(BLOCKLEN+1);
		p.y = point.y + shape->points[i].y*(BLOCKLEN+1);
		putBlock(p,color);
	}
}

void printTetris(TETRIS *tetris)
{
	putTetris(tetris,calAbsPos(tetris));
}

void emptyBlock(POINT p)
{
	putBlock(toAbsPoint(p),WHITE);
}

void emptyTetris(TETRIS *tetris)
{
	int i = 0;
	for(;i<4;i++)
		emptyBlock(calRePos(tetris,i));
}

void putNext(TETRIS* tetris)
{
	int len = tetris->group->roateLen;
	int x,y;
	POINT absP = {CLOUMN+5,6};
	int offset = BLOCKLEN*len/2;
	POINT p=toAbsPoint(absP);
	setfillstyle(SOLID_FILL,WHITE);
	bar(RIGHTMARGIN+3*BLOCKLEN+3,
		TOPMARGIN+3*BLOCKLEN+18,
		RIGHTMARGIN+8*BLOCKLEN+7,
		TOPMARGIN+9*BLOCKLEN+8);
	p.x-=offset,p.y-=offset;
	putTetris(tetris,p);
}


void drawBg()
{
	int i;
	int x,y;
	setfillstyle(SOLID_FILL,WHITE);
	bar(0,0,_width,_height);
	setcolor(LIGHTGRAY);
	for(i=0;i<=LINE;i++)
		line(LEFTMARGIN,TOPMARGIN+(BLOCKLEN+1)*i,
			RIGHTMARGIN,
			TOPMARGIN+(BLOCKLEN+1)*i);
	for(i=0;i<=CLOUMN;i++)
		line(LEFTMARGIN+(BLOCKLEN+1)*i,TOPMARGIN,
				LEFTMARGIN+(BLOCKLEN+1)*i,
				BOTTOMMAGIN);
	rectangle(RIGHTMARGIN+2*(BLOCKLEN+1),
		TOPMARGIN+2*(BLOCKLEN+1),
		RIGHTMARGIN+8*(BLOCKLEN+1),
		TOPMARGIN+9*(BLOCKLEN+1));
	rectangle(RIGHTMARGIN+2*(BLOCKLEN+1),
		TOPMARGIN+11*(BLOCKLEN+1),
		RIGHTMARGIN+8*(BLOCKLEN+1),
		TOPMARGIN+18*(BLOCKLEN+1));
		x=RIGHTMARGIN+3*(BLOCKLEN+1);
	x = RIGHTMARGIN+2*(BLOCKLEN+1)+40;
	y = TOPMARGIN+2*(BLOCKLEN+1)+10;
	draw_picture(x,y,nextPic);
	for(i=0;i<14;i++)
		draw_picture(20,40+50*i,introText[i].pic);
}


void set_clock_frequency(int n)
{
   outportb(0x43, 0x36);
   outportb(0x40, (1193180/n) & 0xFF);
   outportb(0x40, (1193180/n) >> 8);
}

int isAvailable(TETRIS* tetris)
{
	int i=0,x,y;
	SHAPE* shape = getShape(tetris);
	for(;i<4;i++)
	{
		x = tetris->pos.x + shape->points[i].x;
		y = tetris->pos.y + shape->points[i].y;
		if((x>=0&&y>=0&&map[x][y].filled)||x<0||x>=CLOUMN||y>=LINE)
			break;
	}
	return i==4;
}

void rotateTetris(TETRIS *newOne)
{
	int len = newOne->group->roateLen;
	if(newOne->pos.x<0)
		newOne->pos.x=0;
	else if(newOne->pos.x+len>CLOUMN)
		newOne->pos.x=CLOUMN-len;
	newOne->roate++;
}

void printLine(int y)
{
    int x;
    POINT p;
    p.y=y;
	for(x=0;x<CLOUMN;x++)
	{
        p.x=x;
		if(y-1>=0&&map[x][y-1].filled)
		{
			putBlock(toAbsPoint(p),map[x][y-1].color);
			map[x][y].filled=1;
		}
		else
		{
			putBlock(toAbsPoint(p),WHITE);
			map[x][y].filled=0;
		}
	}
}
void clearLine(int index)
{
	int y=index;
	while(y>=0)
		printLine(y--);
}

void showScore()
{
	int x = RIGHTMARGIN+3*(BLOCKLEN+1);
	int y = TOPMARGIN+13*(BLOCKLEN+1);
	char buff[20];
	PIC *p[2];
	setfillstyle(SOLID_FILL,WHITE);
	bar(x,y,x+180,y+150);
	setcolor(RED);
	p[0] = get_ttf_text_pic("Score:","courierb.ttf",35);
	sprintf(buff,"%04d",score);
	p[1] = get_ttf_text_pic(buff,"courierb.ttf",50);
	draw_picture(x,y,p[0]);
	draw_picture(x,y+50,p[1]);
	destroy_picture(p[0]),destroy_picture(p[1]);
	needRefresh=0;
}
int putToMap()
{
	int i=0,j;
	POINT p;
	for(;i<4;i++)
	{
		p = calRePos(curTetris,i);
		map[p.x][p.y].filled=1;
		map[p.x][p.y].color = curTetris->group->color;
	}
	free(curTetris);
	curTetris = nextTetris;
	nextTetris = generateTetris();
	for(j=LINE;j>=0;j--)
	{
		for(i=0;i<CLOUMN;i++)
			if(!map[i][j].filled)
				break;
		if(i==CLOUMN)
		{
			clearLine(j);
			score++;
			needRefresh=1;
			j++;
		}
	}
	if(!isAvailable(curTetris))
	{
		free(curTetris);
		free(nextTetris);
		curTetris=NULL;
		nextTetris=NULL;
		over=1;
		return 0;
	}
	return 1;
}


void newGame()
{
	int i=0;
	drawBg();
	for(;i<LINE*CLOUMN;i++)
		memcpy((BLOCK*)map+i,&empty,sizeof(empty));
	if(old_img)
	{
		free(old_img);
		old_img=NULL;
	}
	curTetris = generateTetris();
	nextTetris = generateTetris();
	printTetris(curTetris);
	putNext(nextTetris);
	timeout = TIMEOUT-1;
	score=0;
	needRefresh=1;
}

int movedown(TETRIS* newOne)
{
	newOne->pos.y++;
	return isAvailable(newOne);
}

void moveStatus(TETRIS* newOne)
{
	emptyTetris(curTetris);
	*curTetris = *newOne;
	printTetris(curTetris);
}

void moveTetris(unsigned char key)
{
	TETRIS newOne = *curTetris;
	int putMap = 0;
	switch(key)
	{
	case _UP:
		rotateTetris(&newOne);
		break;
	case _RIGHT:
		newOne.pos.x++;
		break;
	case _LEFT:
		newOne.pos.x--;
		break;
	case _DOWN:
		if(movedown(&newOne))
			moveStatus(&newOne);
		else
			putMap = 1;
		putMap += !movedown(&newOne);
		break;
	case _SYSDOWN:
		putMap = !movedown(&newOne);
		break;
	case _SPACE:
		while(movedown(&newOne))
			moveStatus(&newOne);
		putMap = 1;
		break;
	}
	if(putMap)
	{
		if(putToMap())
		{
			putNext(nextTetris);
			printTetris(curTetris);
		}
	}
	else if(isAvailable(&newOne))
		moveStatus(&newOne);
}

void interrupt keyEvent()
{
	unsigned char key;
	key = inportb(0x60);
	if(!pause&&!over)
		if(key==_UP||key==_DOWN||key==_LEFT||
			key==_RIGHT||key==_SPACE)
			moveTetris(key);
	if(key==0x81)
	{
		if(pause||over)
			stop=1;
		else
			pause=1;
	}
	if(key==0x39)
	{
		if(pause)
			pause=0;
		if(over)
		{
			newGame();
			over=0;
			overShow=0;
		}
	}
	outportb(0x20,0x20);
}

void interrupt timeoutEvent()
{
	if(timeout==0)
	{
		timeout=TIMEOUT-(score*10>500?500:score*10);
		if(!pause&&!over)
			moveTetris(_SYSDOWN);
	}
	else timeout--;
	outportb(0x20,0x20);
}

void initGameText()
{
	int x[]={50,75,100,100,100,
	50,100,125,75,50};
	int y[]={100,200,250,300,350,100,200,250,300,350};
	int i=0;
	setcolor(RED);
	pauseText[0].pic = get_ttf_text_pic("PAUSE GAME","courierb.ttf",50);
	pauseText[1].pic = get_ttf_text_pic("PRESS SPACE TO","courierb.ttf",30);
	pauseText[2].pic = get_ttf_text_pic("RESUME GAME","courierb.ttf",30);
	pauseText[3].pic = get_ttf_text_pic("PRESS ESC TO","courierb.ttf",30);
	pauseText[4].pic = get_ttf_text_pic(" EXIT GAME","courierb.ttf",30);
	
	overText[0].pic = get_ttf_text_pic("GAME OVER!","courierb.ttf",50);
	overText[1].pic = get_ttf_text_pic("PRESS ESC TO","courierb.ttf",30);
	overText[2].pic = get_ttf_text_pic("EXIT GAME","courierb.ttf",30);
	overText[3].pic = get_ttf_text_pic("PRESS SPACE TO","courierb.ttf",30);
	overText[4].pic = get_ttf_text_pic("START A NEW GAME","courierb.ttf",30);
	
	introText[0].pic = get_ttf_text_pic("Press left/right","courierb.ttf",20);
	introText[1].pic = get_ttf_text_pic("key to move","courierb.ttf",20);
	introText[2].pic = get_ttf_text_pic("current tetris.","courierb.ttf",20);
	introText[3].pic = get_ttf_text_pic("Press up key","courierb.ttf",20);
	introText[4].pic = get_ttf_text_pic("to rotate","courierb.ttf",20);
	introText[5].pic = get_ttf_text_pic("current tetris.","courierb.ttf",20);
	introText[6].pic = get_ttf_text_pic("Press down key","courierb.ttf",20);
	introText[7].pic = get_ttf_text_pic("to move down","courierb.ttf",20);
	introText[8].pic = get_ttf_text_pic("tetris quicker.","courierb.ttf",20);
	introText[9].pic = get_ttf_text_pic("press space key","courierb.ttf",20);
	introText[10].pic = get_ttf_text_pic("to drop tetris","courierb.ttf",20);
	introText[11].pic = get_ttf_text_pic("immediately.","courierb.ttf",20);
	introText[12].pic = get_ttf_text_pic("Press esc key to","courierb.ttf",20);
	introText[13].pic = get_ttf_text_pic("pause the game.","courierb.ttf",20);
	
	setcolor(0x18);
	nextPic = get_ttf_text_pic("Next:","courierb.ttf",40);
	for(i=0;i<5;i++)
	{
		pauseText[i].pos.x=x[i];
		pauseText[i].pos.y=y[i];
	}
	for(i=0;i<5;i++)
	{
		overText[i].pos.x=x[i+5];
		overText[i].pos.y=y[i+5];
	}
}

void initGame()
{
	int i=0,x,y;
	int driver=0, mode=VESA_1024x768x8bit;
	initgraph(&driver, &mode, "");
	srand(time(NULL));
	initGameText();
	newGame();
	old_9h = getvect(9);
	old_8h = getvect(8);
	setvect(9, keyEvent);
	setvect(8, timeoutEvent);
	set_clock_frequency(1000);
}

void endGame()
{
	setvect(9,old_9h);
	setvect(8,old_8h);
	closegraph();
}

void pauseGame()
{
	int x = (_width-BLOCK_SIZE)/2;
	int y = (_height-BLOCK_SIZE)/2;
	int i=0;
	if(old_img)
		return;
	old_img = malloc(imagesize(0, 0, 
             BLOCK_SIZE-1, BLOCK_SIZE-1));
	getimage(x, y, x+BLOCK_SIZE-1,
		y+BLOCK_SIZE-1,old_img);
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(LIGHTGRAY);
	rectangle(x,y,x+BLOCK_SIZE-1,y+BLOCK_SIZE-1);
	bar(x+1,y+1,x+BLOCK_SIZE-2,y+BLOCK_SIZE-2);
	for(i=0;i<5;i++)
		draw_picture(pauseText[i].pos.x+x,
			pauseText[i].pos.y+y,pauseText[i].pic);
}

void gameOver()
{
	int x = (_width-BLOCK_SIZE)/2;
	int y = (_height-BLOCK_SIZE)/2;
	int i=0;
	if(overShow)
		return;
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(LIGHTGRAY);
	rectangle(x,y,x+BLOCK_SIZE-1,y+BLOCK_SIZE-1);
	bar(x+1,y+1,x+BLOCK_SIZE-2,y+BLOCK_SIZE-2);
	for(i=0;i<5;i++)
		draw_picture(overText[i].pos.x+x,
			overText[i].pos.y+y,overText[i].pic);
	overShow=1;
}

void resumeGame()
{
	int x = (_width-BLOCK_SIZE)/2;
	int y = (_height-BLOCK_SIZE)/2;
	if(!old_img)
		return;
	putimage(x,y,old_img,COPY_PUT);
	free(old_img);
	old_img=NULL;
}

void startGame()
{
	while(!stop)
	{
		if(needRefresh)
			showScore();
		if(pause)
			pauseGame();
		else
			resumeGame();
		if(over)
			gameOver();
	}
}

int main()
{
	initGame();
	startGame();
	endGame();
	return 0;
}

