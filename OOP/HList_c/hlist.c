#include "hlist.h"
#include <stdlib.h>
#include <string.h>

static HList *newNode()
{
    HList *node = (HList*)malloc(sizeof(HList));
    node->Element = NULL;
    node->next = NULL;
    node->ElementType.id = 0;
    node->ElementType.size = 0;
    return node;
}

static void seek(HList **node, HList **pre, void *element, Type type)
{
    if(pre)
        *pre = *node;
    HList *current = (*node)->next;
    while(current)
    {
        if(current->ElementType.id == type.id)
            if(!memcmp(current->Element,element,type.size))
                break;
        if(pre)
            *pre = current;
        current = current->next;
    }
    *node = current;
}

static HList *last(HList *list, HList **pre)
{
    while(list->next)
    {
        if(pre)
            *pre = list;
        list = list->next;
    }
    return list;
}

void Append(HList *list, void *element, Type type)
{
    if(!list) return;
    list = last(list,NULL);
    list->next = newNode();
    list = list->next;
    list->Element = element;
    list->ElementType = type;
}

void Remove(HList *list, void *element, Type type)
{
    if(!list || !list->next) return;
    list = list->next;
    HList *pre;
    seek(&list,&pre,element,type);
    if(!list) return;
    pre->next = list->next;
    free(list->Element);
    free(list);
}


HList *Find(HList *list, void *element, Type type)
{
    seek(&list,NULL,element,type);
    return list;
}

HList *Create()
{
    return newNode();
}

HList *TypeNext(HList *list, Type type)
{
    if(!list) return NULL;
    list = list->next;
    while(list && list->ElementType.id != type.id)
        list = list->next;
    return list;
}

void printAll(HList *list)
{
    if(!list)
        return;
    list = list->next;
    while(list && list->ElementType.print)
    {
        list->ElementType.print(list->Element);
        list = list->next;
    }
}

void Push(HList *list, void *element, Type type)
{
    if(list)
        return;
    HList *New = newNode();
    New->next = list->next;
    list->next = New;
    New->Element = element;
    New->ElementType = type;
}

void Reverse(HList *list)
{
    HList *cur, *next, *last;
    cur = list->next;
    last = NULL;
    while(cur)
    {
        next = cur->next;
        cur->next = last;
        last = cur;
        cur = next;
    }
    list->next = last;
}

void merge(HList *list, HList *operand)
{
    if(!list||!operand) return;
    while(list->next)
        list = list->next;
    list->next = operand->next;
    free(operand);
}

void empty(HList *list)
{
    if(!list)
        return;
    HList *next;
    next = list->next;
    list->next = NULL;
    list = next;
    while(list)
    {
        next = list->next;
        free(list);
        list = next;
    }
}

void Pop(HList *list)
{
    HList *next = list->next;
    list->next = next->next;
    free(next);
}

void Out(HList *list)
{
    HList *pre;
    list = last(list,&pre);
    pre->next = NULL;
    free(list);
}
