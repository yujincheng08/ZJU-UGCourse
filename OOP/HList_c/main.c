#include <stdio.h>
#include "hlist.h"

void printInt(void *element)
{
    printf("%d\n",*(int*)element);
}

void printDouble(void *element)
{
    printf("%lf\n",*(double*)element);
}

void printChar(void *element)
{
    printf("%c\n",*(char*)element);
}

int main()
{
    HList *list = Create();
    Type Int={1,sizeof(int),printInt};
    Type Double={2,sizeof(double),printDouble};
    Type Char={3,sizeof(char),printChar};
    int a = 1, b = 2, c = 3;
    char x = 'x', y = 'y', z = 'z';
    double i = 0.1, j = 0.2, k = 0.3;
    Append(list, &a, Int);
    Append(list,&i,Double);
    Append(list,&b,Int);
    Append(list,&x,Char);
    Append(list,&c,Int);
    Append(list,&y,Char);
    Append(list,&j,Double);
    Append(list,&k,Double);
    Append(list,&z,Char);
    printAll(list);
    Reverse(list);
    printAll(list);
    empty(list);
    printAll(list);
//    HList * found = Find(list,&x,Char);
//    printf("%c\n",*((char*)found->Element));
//    found = TypeNext(found,Double);
//    printf("%lf\n",*((double*)found->Element));
    return 0;
}

