#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRMAX 20

typedef struct List List;
struct List
{
    List *next;
    char first[STRMAX];
    char last[STRMAX];
    char num[STRMAX];
}; // Structure of the linked list

#define MAX 10005

// Stack for storing result
List *stack[MAX];
unsigned top = 0U;

// Get line but in size s
void get(char *buff, size_t s)
{
    fgets(buff,s, stdin);
    size_t ss = strlen(buff);
    if(buff[ss-1]=='\n')
       buff[ss-1]='\0';
}

int main()
{
    List head; // Dummy head
    head.next = NULL;
    int N;
    scanf("%d",&N);
    getc(stdin);
    for(int i=0; i<N; ++i)
    {
        // New node
        List *node = malloc(sizeof(List));
        get(node->first,STRMAX);
        get(node->last,STRMAX);
        get(node->num,STRMAX);
        // Attach to the dummpy head
        node->next = head.next;
        head.next = node;
    }
    int M;
    scanf("%d", &M);
    getc(stdin);
    while(M--)
    {
        top = 0U;
        char in[STRMAX];
        get(in,STRMAX);
        List *i = &head;
        // Travese the linked list and compare names
        while(i->next)
        {
            i = i->next;
            int flag = 0;
            if(strncmp(in, i->first, STRMAX)==0)
                flag = 1;
            else if(strncmp(in, i->last, STRMAX)==0)
                flag = 1;
            // If found, store in the stack
            if(flag)
                stack[top++] = i;
        }
        // Output the number of reuslt
        printf("%u\n", top);
        while(top)
        {
            i = stack[--top];
            // Outpu the nodes store in the stack
            if(top>0||M>0)
                printf("%s %s %s\n", i->last, i->first, i->num);
            else
                printf("%s %s %s", i->last, i->first, i->num);
        }
    }
    return 0;
}
