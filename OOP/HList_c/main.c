#include <stdio.h>
#include "hlist.h"
#include <windows.h>

void printInt(void *element);
void printDouble(void *element);
void printChar(void *element);
void printBool(void *element);
int printColorf(char *fmt, unsigned short colorIndex, ...);
void printPrompt(char *fmt);

int main()
{
    HList *list = Create();
    //Define varibles to append.
    //In pratice they are usually defined by programmers
    //or input by users.
    int a = 1, b = 2, c = 3;
    char x = 'x', y = 'y', z = 'z';
    double i = 0.1, j = 0.2, k = 0.3;
    boolean m = {0}, n = {1};
    //Define types that's going to insert.
    //It should be defined by programmers.
    //The output funcitons are defined by programmers.
    Type Int={1,sizeof(a),printInt};
    Type Double={2,sizeof(i),printDouble};
    Type Char={3,sizeof(x),printChar};
    Type Boolean={4,sizeof(m),printBool};
    //Append some elements with different types randomly.
    //Testing function Append.
    Append(list, &a, Int);
    Append(list,&i,Double);
    Append(list,&b,Int);
    Append(list,&m,Boolean);
    Append(list,&x,Char);
    Append(list,&c,Int);
    Append(list,&n,Boolean);
    Append(list,&y,Char);
    Append(list,&j,Double);
    //Print the result.
    printPrompt("After appending:\n");
    PrintAll(list);
    //Testing function Push.
    Push(list,&j,Double);
    Push(list,&n,Boolean);
    Push(list,&k,Double);
    Push(list,&z,Char);
    Push(list,&a,Int);
    printPrompt("\nAfter pushing:\n");
    PrintAll(list);
    //Testing function Out.
    Out(list);
    printPrompt("\nAfter outting:\n");
    PrintAll(list);
    //Testing function Pop.
    Pop(list);
    printPrompt("\nAfter poping:\n");
    PrintAll(list);
    //Testing function Find.
    HList *node = Find(list,&n,Boolean);
    printPrompt("\nWhat we find with boolean n:\t");
    printBool(node->Element);
    //Testing function Remove.
    Remove(list,&n,Boolean);
    printPrompt("\nAfter Removing what we found:\n");
    PrintAll(list);
    //Testing function Typenext and its two usages.
    node = TypeNext(list,Double);
    if(node)
    {
        printPrompt("\nFirst double found:\t");
        printDouble(node->Element);
    }
    node = TypeNext(node,Double);
    if(node)
    {
        printPrompt("\nSecond double found:\t");
        printDouble(node->Element);
    }
    //Testing function Reverse.
    Reverse(list);
    printPrompt("\nAfter reversing:\n");
    PrintAll(list);
    //New another list for merging.
    HList *other = Create();
    Push(other,&m,Boolean);
    Push(other,&x,Char);
    printPrompt("\nAnother Hlist:\n");
    PrintAll(other);
    //Testing fucntion Merge.
    Merge(list,other);
    printPrompt("\nAfter merging:\n");
    PrintAll(list);
    //Finally, testing function Empty and IsEmpty.
    Empty(list);
    printPrompt("\nAfter empty:\n");
    if(IsEmpty(list))
        printf("Empty\n");
    else
        printf("Non-empty\n");
    return 0;
}

//These are function of outputing elements
//defined by programmer, giving them flexible
//method to use the elements.
void printInt(void *element)
{
    printColorf("%d\t",0xE,*(int*)element);
}

void printDouble(void *element)
{
    printColorf("%.3lf\t",0xD,*(double*)element);
}

void printChar(void *element)
{
    printColorf("%c\t",0xA,*(char*)element);
}

void printBool(void *element)
{
    boolean *e = (boolean*)element;
    if(*e)
        printColorf("true\t",0xB);
    else
        printColorf("false\t",0xB);
}

//Set color of the following output text.
void setConsoleColor(unsigned short colorIndex)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorIndex);
}

//Print varibles with function in format.
int printColorf(char *fmt,unsigned short colorIndex, ...)
{
    va_list arg;
    int s;
    va_start(arg, colorIndex);
    setConsoleColor(colorIndex);
    s = vprintf(fmt, arg);
    setConsoleColor(0xF);
    va_end(arg);
    return s;
}

//Print prompt
void printPrompt(char *fmt)
{
    printColorf(fmt,0xC);
}
