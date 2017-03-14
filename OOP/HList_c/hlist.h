#ifndef HLIST_H
#define HLIST_H
#include <stdlib.h>

typedef struct HList HList;
typedef unsigned TypeID;
typedef struct Type Type;

/* Sturct type identify element's in the list pointer.
 * The id member is the user-defined number to identify
 * of which type the element is.
 * Besides, print is a function pointer to print the element
 * of this type.
 * @Usage:
 * void printInt(void *intElement)
 * {
 * 	printf("%d\n",*((int*)intElement));
 * }
 * Type typeInt = {1, sizeof(int), printInt); */
struct Type
{
    TypeID id;
    size_t size;
    void (*print)(void *element);
};

/* HList is a heterogeneous list.
 * It contains an element which is a void pointer.
 * It also a member ElementType to identify the element's type.
 * The next pointer points to the next element of the list. */
struct HList
{
    void *Element;
    Type ElementType;
    HList *next;
};

/* Create is a function to allocate an empty HList.
 * @Usage: HList *list = Create(); */
HList *Create();

/* Append is a function to append a new element to
 * the last of list.
 * @Usage:
 * int a = 3;
 * Append(list, &a, typeInt);
 * @Caution: New element to the list is the copy
 * of the orignal value. So it's SAFE to append/push
 * local variables.*/
void Append(HList *list, void *element, Type type);

/* Push is a function to push a new element to
 * the top of list.
 * @Usage:
 * int b = 3;
 * Push(list, &b, typeInt);
 * @Caution: Same as Append, it's SAFE to push
 * local vriables into the list. */
void Push(HList *list, void *element, Type type);

/* Remove is a function to delete an element
 * of the list with the same value and type
 * of the argument.
 * @Usage:
 * int c = 3;
 * Remove(list, &c, typeInt); */
void Remove(HList *list, void *element, Type type);

/* Pop is to remove the first element of the list.
 * @Usage: Pop(list); */
void Pop(HList *list);

/* Out is to delete the last element of the list
 * @Usage: Out(list); */
void Out(HList *list);

/* Reverse the list.
 * For instance:
 * Original order: 1, 'a', "string"
 * Order after reverse: "string", 'a', 1
 * @Usage: Reverse(list); */
void Reverse(HList *list);

/* Print all the element by order.
 * @Caution: ONLY print the element of the types
 * which own print function pointer.
 * @Usage: printAll(list); */
void PrintAll(HList *list);

/* Find is to find the first element with the same
 * value and type of the argument.
 * @Usage:
 * int c = 4;
 * HList *first = Find(list, &c, typeInt);
 * HList *Second = Find(first, &c, &typeInt); */
HList *Find(HList *list, void *element, Type type);

/* TypeNext is to find the next element with the
 * same type of the argumets.
 * @Usage:
 * int d = 3;
 * HList *firstType = TypeNext(list, &d, typeInt);
 * HList *nextType = TypeNext(firstType, &d, typeInt); */
HList *TypeNext(HList *list, Type type);

/* Merge two list into one list
 * The second list will append to the last of the first list.
 * @Usage: Merge(list, otherlist);
 * @Caution: The second list will become invaild. DO NOT try
 * to use it after merging. */
void Merge(HList *list, HList *operand);

/* Clear the list.
 * @Usage: Empty(list); */
void Empty(HList *list);

/* Test whether a list is Empty.
 * Return 0 when it's not empty.
 * Return 1 when it's empty. */
int IsEmpty(HList *list);
#endif // HLIST_H
