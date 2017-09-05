#include <graphics.h>
#include "list.h"

static const LIST *selectItem,*curTopItem;
//store the selected item and the top item on the screen
static int selectIndex,curTopIndex;
//store the index of them

void clearList()
{
    setfillstyle(SOLID_FILL,BLACK);
    bar(11,31,209,_height-11);//just a solid rectangle
}

static void printList()
{
    //print list on the screen
    int i=0;
    const LIST *item = curTopItem;
    char buff[20];
    sprintf(buff,"%d/%d",getListLen()==0?0:selectIndex+1,getListLen());
    clearList();
    outtextxy(10+(200-strlen(buff)*8)/2,32,buff);
    for(;i<21&&item&&item->index;i++)
    {
        if(item==selectItem)
        {
            setfillstyle(SOLID_FILL,BLUE);
            bar(15,47+i*20,200,47+i*20+20);
            setfillstyle(SOLID_FILL,WHITE);
        }
        outtextxy(20,50+i*20,(char*)item->index->entry);
        item=item->next;
    }
}

searchType query(const char *exp)
{
    searchType type = search(exp);
    home();
    //seach and set current selected item to the first element
    return type;
}

const INDEX *getCurIndex()
{
    return selectItem->index;//simply return it
}

void moveDown()
{
    if(selectIndex<getListLen()-1)
    {
        selectIndex++;
        if(selectIndex>curTopIndex+20)//is the bottom?
        {
            curTopIndex++;
            curTopItem = curTopItem->next;
        }
        selectItem = selectItem->next;
        printList();//refresh
    }
}

void moveUp()
{
    if(selectIndex>0)//is the top?
    {
        selectIndex--;
        if(selectIndex<curTopIndex)
        {
            curTopIndex--;
            curTopItem = curTopItem->previous;
        }
        selectItem = selectItem->previous;
        printList();//refresh
    }
}

void pageUp()
{
    if(curTopIndex-21>=0)//the top?
    {
        int i = 0;
        selectIndex = curTopIndex = curTopIndex-21;
        for(;i<21;i++)//iterate 21 times
            curTopItem = curTopItem ->previous;
        selectItem = curTopItem;
        printList();
    }
    else
        home();//directly go to the top
    
}

void pageDown()
{
    int i;
    if(curTopIndex+42<=getListLen())
    {
        curTopIndex = curTopIndex+21;
        selectIndex = curTopIndex+20;
        //plus 20 and iterate 20 times
        for(i=0;i<21;i++)
            curTopItem = curTopItem ->next;
        for(i=0,selectItem = curTopItem;i<20;i++)
            selectItem = selectItem->next;
        printList();
    }
    else
        end();
    // selectIndex = curTopIndex+21;
    // selectItem = curTopItem+21;
    
}

void home()
{
    selectItem = curTopItem = getList();//all set to the top of the list
    selectIndex = curTopIndex = 0;
    printList();
}

void end()
{
    if(getListLen()>21)//is the bottom?
    {
        int i;
        selectIndex = getListLen()-1;
        curTopIndex = selectIndex-20;
        while(curTopItem->next)//iterate to the last
            curTopItem = curTopItem->next;
        for(i=0,selectItem = curTopItem;i<20;i++)//remount to the last but 20
            curTopItem = curTopItem->previous;
    }
    else
        del();
    printList();
}

void insert()
{
    selectIndex = curTopIndex;//all set to top
    selectItem = curTopItem;
    printList();
}

void del()
{
    while(selectItem->next&&selectIndex-curTopIndex<20)//iterate to the last but 20
        selectItem = selectItem->next,
        selectIndex++;
    printList();
}


