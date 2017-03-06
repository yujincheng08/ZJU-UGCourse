#ifndef HLIST_H
#define HLIST_H
#include <stdlib.h>

//@TODO: 实现双向链表
typedef struct HList HList;
typedef unsigned TypeID;
typedef struct Type Type;

struct Type
{
    TypeID id;
    size_t size;
    void (*print)(void *element);
};

struct HList
{
    void *Element;
    Type ElementType;
    HList *next;
};

HList *Create();
void Append(HList *list, void *element, Type type);
void Push(HList *list, void *element, Type type);
void Remove(HList *list, void *element, Type type);
void Pop(HList *list);
void Out(HList *list);
void Reverse(HList *list);
void printAll(HList *list);
HList *Find(HList *list, void *element, Type type);
HList *TypeNext(HList *list, Type type);
void merge(HList *list, HList *operand);
void empty(HList *list);
#endif // HLIST_H
