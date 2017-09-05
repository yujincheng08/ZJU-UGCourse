#include <graphics.h>
#include <ctype.h>
#include "app.h"
#include "library.h"
#include "list.h"
#include "index.h"
#include "queryExp.h"

static InterruptFunctionPointer old_9h;//old interrupt function in port 9
static InterruptFunctionPointer old_8h;//and the fuction in port 8
static int stop = 0;//control the message loop. If its value have been changed into 1, the message loop will end

static const char KEY[35]={'q','w','e','r','t','y','u','i','o','p','[',']','\n',0x11,'a','s','d','f','g','h','j','k','l',';','\'','~',0x10,'\\','z','x','c','v','b','n','m'};
//key maps
void endApp()
{
    setvect(9,old_9h);
	setvect(8,old_8h);//reset interrupt fuction
	closegraph();//library function
}

static void drawBg()
{
    //draw the background
    setfillstyle(SOLID_FILL,WHITE);
    rectangle(10,30,210,_height-10);
    rectangle(220,30,_width-10,_height-10);
    rectangle(222,32,_width-12,_height-12);
    rectangle(70,5,_width-10,25);
    outtextxy(12,10,"Search:");
}

static void interrupt keyEvent()
{
    //interrupt fuction in port 9, which responds when a current key is pressed or released
    unsigned char key = inportb(0x60);
    static int shift = 0;
    static int capLock = 0; //shift and capLock logs whether they have been pressed or toggled
    static int searched = 0; //if in search mode, it equals 1
    switch(key)
    {
    case 0x81://esc key
        searched=0;
        clearQuery();
        clearList();
        clearWord();
        break;
    case 0x2A: //shift key pressed
        shift=1;
        break;
    case 0xAA: //shift key released
        shift=0;
        break;
    case 0xBA: //caplock key released
        capLock = 0;
        break;
    case 0x3A: //caplock key pressed
        capLock = 1;
        break;
    case 0x0E: //backspace key
        if(!searched)
            deleteChar();
        break;
    case 0x1C: //enter
        if(!searched)
        {
            searchType type = query(getQueryExp()); //query the querystr, and get the search type
            if(stricmp(getQueryExp(),"exit")==0||stricmp(getQueryExp(),"quit")==0)
            { //if exit or quit input, end the program
                stop=1;
                break;
            }
            searched=1;
            if(type==EXACT)
            {
                searched=0; //exact search does not need to enter search mode
                clearWord(); //clear query string
                if(getListLen()!=0) //make sure not to dereference wild pointer
                    printWord(getCurIndex()); //print current word, which is the only word in the list
            }
        }
        else
        {
            if(getListLen()!=0)
                printWord(getCurIndex());//print current word, which is the first word in the list
        }
        break;
    case 0x09: //8&* key
        if(shift&&!searched)
            addChar('*');
        break;
    case 0x35: ///&? key
        if(shift&&!searched)
            addChar('?');
        break;
    // , - \ charactors
    case 0x33:
        addChar(',');
        break;
    case 0x0C:
        addChar('-');
        break;
    case 0x28:
        addChar('\'');
        break;
    //respond up down right lest pgUp pgDn Home end delete and Inster keys
    case _UP:
        moveUp();
        break;
    case _DOWN:
        moveDown();
        break;
    case _RIGHT:
        nextPage();
        break;
    case _LEFT:
        previousPage();
        break;
    case 0x49:
        pageUp();
        break;
    case 0x47:
        home();
        break;
    case 0x4F:
        end();
        break;
    case 0x52:
        insert();
        break;
    case 0x53:
        del();
        break;
    case 0x51:
        pageDown();
        break;
    default: //respond other keys
        if(key>=0x10&&key - 0x10<35&&isalpha(key=KEY[key - 0x10])&&!searched)
        //if they are alpha
            addChar(shift^capLock?toupper(key):key);
        break;
    };
    outportb(0x20,0x20); //processing ends
}

static void interrupt timeoutEvent()
{
    //there is no need to respond 8 port
    outportb(0x20,0x20);
}


static void set_clock_frequency(int n)
{
    //fuction to set the clock interrupt freqency
   outportb(0x43, 0x36);
   outportb(0x40, (1193180/n) & 0xFF);
   outportb(0x40, (1193180/n) >> 8);
}

int exec()
{
    while(!stop)
    {};//continue loop when stop doesnt equal to 1
    endApp(); //loop ended, program should end
    return 0;
}

int initApp()
{
    int result = 1; //log whether initilization is successful or not
    int driver=0, mode=VESA_640x480x8bit;
    initgraph(&driver, &mode, "");
    //initilize dict and lib
    result=result && initDict();
    result=result && initLib();
    //draw background
    drawBg();
    old_8h=getvect(8),old_9h=getvect(9);
    //log the old fuctions and set new functions
    setvect(9, keyEvent);
	setvect(8, timeoutEvent);
    set_clock_frequency(1000);
    return result;
    //tell main fuction whether initilization is successful
}
