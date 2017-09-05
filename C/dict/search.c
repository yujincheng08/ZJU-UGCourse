#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "search.h"

static int listLen;
//store the length of the list
static LIST *list,*head;
//list stores the first element of the list of the searching result
//head stores the first element of the whole list which stores all the words in the idx file
//head is used to free the memory space

static int KMP(const char *entry,int len,char *exp,int expLen,short prefix,int *next)
{
    //KMP matching algorithm, which is used to match the given expression and string
    //entry is the string to be matched
    //len is the length of entry stirng
    //exp is the matching string
    //expLen is the length of the exp
    //prefix refers to the first index of entry that is going to be match
    //characters befor prefix will be ignored
    //next is an array created according to exp
    //for more details please check for the KMP algorithm
    int i=prefix,j=0;
    if(prefix+expLen>len)//if the result bigger than len it cannot match the condiction
        return 0;
    while(j<expLen&&i<len)//main body of the KMP algorithm
    {
        if(tolower(entry[i])==exp[j]||exp[j]=='?')
            i++,j++;
        else if(j==0)
            i++;
        else
            j=next[j-1]+1;
    }
    if(j==expLen)
        return i;//successfully matched, return the character that follow the last matching letter
    return 0;//failed to match
}

static void KMPgetNext(char *exp,int expLen,int *next)
{
    //calculate the next array that is used by KMP algorithm
    //for more details please check for the KMP algorithm
    int i,j;
    next[0] = -1;
    for(i=1;i<expLen;i++)
    {
        j = next[i-1];
        while(j>=0&&exp[j+1]!=exp[i])
            j=next[j];
        if(exp[j+1]==exp[i])
            next[i]=j+1;
        else
            next[i]=-1;
    }
}

static LIST *deleteItem(LIST *item)
{
    //delete the given item from the list and return the following item
    LIST *next = item->next;
    if(item->previous)
        item->previous->next = next;
    else
        list=next;
    if(next)
        next->previous = item->previous;
    return next;
}

static void freeList()
{
    //free an old list
    free(head);
}

static void newList()
{
    //create a new list from idx file
    //a new list will store all the words
    int i=0,dictLen=getDictLen();
    const INDEX *index=getIndex();
    list = malloc(sizeof(LIST)*dictLen);
    for(;i<dictLen;i++)
    {
        list[i].previous = list+i-1;
        list[i].next = list+i+1;
        list[i].index = index+i;
        list[i].prefix=0;//at the beginning, all words are never matched
        list[i].len = entryLen(index[i].entry);
    }
    list->previous = NULL;
    list[dictLen-1].next=NULL;
    head = list;//store the head
}

static void fuzzySearch(char *exp)
{
	LIST *item=list;
    int expLen = strlen(exp);
    int *next = malloc(sizeof(int)*expLen);
    if(expLen==0)
        return;//there is no need to filter
    KMPgetNext(exp,expLen,next);
    //before filterring, work out the next array
	while(item)
	{
		if(item->prefix = 
            KMP(item->index->entry,item->len,exp,expLen,item->prefix,next))
            item=item->next;//if matched, skip it
        else
			item = deleteItem(item);//if not, delete it, and get the next element
	}
    free(next);//free the array 
}

static int myStrnicmp(const char *str1,const char *str2,int len)
{
    //similar to the library function strnicmp
    //but it can ignore '?'
    int i,result;
    for(i=0,result=0;i<len;i++)
        if(str1[i]&&str2[i]&&!result)
            result = str2[i]=='?'?0:tolower(str1[i]) - str2[i];
        else
            break;
    return result;
}

static const long exactSearch(const char *exp,int begin,int end)
{
    //bisection method
    //if matched returns positive number which refers to the object's address
    //if not matched returns a negative number which is the additive inverse of the closest object's address
    //the negative number is used by preFuzzySearch()
    int mid = (begin+end)/2;
    int result;
    const INDEX *index=getIndex();
    if(begin>end)
        return -(long)(index+mid);
    result = stricmp(exp,index[mid].entry);
    //recursion realiziation
    if(result==0)
        return (long)(index + mid);
    else if(result<0)
        return exactSearch(exp,begin,mid-1);
    else
        return exactSearch(exp,mid+1,end);
}

static const INDEX *searchFirst(const char *exp)
{
    //find the first prefix matching word of the given expression
    //it is used to filter words that has defferent prefix before the first element
    //matched by bisection method (exactSearch())
    long ptr = exactSearch(exp,0,getDictLen());
    INDEX *first=NULL;
    const INDEX *firstIndex = head->index;
    int expLen = strlen(exp);
    if(expLen==0) return firstIndex;
    if(ptr>0)
        return (INDEX*)ptr;
    first = (INDEX*)(-ptr);
    while(first>=firstIndex)
        if(myStrnicmp(first->entry,exp,expLen)<0)
            return first+1;
        else first--;
    return firstIndex;
}

static const INDEX *searchLast(const char *exp)
{
    //find the last prefix matching word of the given expression
    //similar to the fuction searchFirst
    char *fix = "\xFF",*buff = malloc((strlen(exp)+strlen(fix)+1)*sizeof(char));
    INDEX *last=NULL;
    const INDEX *lastIndex = head->index+getDictLen();
    int expLen = strlen(exp);
    if(expLen==0) return lastIndex;
    strcpy(buff,exp);
    strcat(buff,fix);
    last = (INDEX*)(-exactSearch(buff,0,getDictLen()))+1;
    while(last<lastIndex)
        if(myStrnicmp(last->entry,exp,expLen)>0)
            return last;
    return lastIndex;
}

static void preFuzzySearch(char *exp)
{
    //filter the words that have different prefix from the expression
    LIST *item = list;
    int expLen = strlen(exp),i=0,qmark=expLen;
    const INDEX *first= NULL, *last = NULL;
    //first refers to the first word with the same prefix
    //last points to the last one
    for(;i<expLen;i++)
        if(exp[i]=='?')
        {
            qmark=i;//find the first index of the question mark
            break;
        }
    if(expLen==0)
        return;//no need to filter
    exp[qmark]='\0';//set the question mark to '\0', as ignores the question mark and content after it
    first = searchFirst(exp), last = searchLast(exp);
    //get first and last
    //the new searching interval after filtering is as follow
    //[first,last)
    exp[qmark]='?';
    //reset the question mark
    if(last-first!=getDictLen())
    {
        //delete items outside the interval
        while(item->index<first)
            item = deleteItem(item);
        while(item->index<last)
        {
            item->prefix=expLen;
            item=item->next;
        }
        item->next=NULL;
        item = deleteItem(item);
    }
    //if there is no question mark, simply end the function
    if(qmark==expLen)
        return;
    item=list;
    //if there are question marks, further filter are required
    while(item)
    {
        if(expLen>item->len)
        {
            item = deleteItem(item);
            continue;
        }
        if(myStrnicmp(item->index->entry,exp,expLen))
            item = deleteItem(item);//not matched, simply remove
        else
            item = item->next;//matched and skip
    }
}

static void sufFuzzySearch(char *exp,int count)
{
    //the last step to the search
    LIST *item = list;
    int expLen = strlen(exp);
    listLen=0;
    while(item)
    {
        if(item->len<item->prefix+expLen&&count!=1||count==1&&item->len!=expLen)
        {
            item = deleteItem(item);
            //when query string has no '*', the length of the matched words are certain.
            //Remove those with wrong length
            //aslo those with shorter suffix not matched than the expression
            continue;
        }
        if(myStrnicmp(item->index->entry+item->len-expLen,exp,expLen))
            item = deleteItem(item);//simply not match the expression
        else
        {
            item = item->next;
            listLen++;//matched! log and turn to the next word
        }
    }
}

static LIST *newItem(INDEX *index)
{
    //create a new element that can be add to the list
    LIST* item = malloc(sizeof(LIST));
    item->previous = item->next = NULL;
    item->index = index;
    item->prefix = item->len = entryLen(index->entry);
    return item;
}

static LIST *addItem(LIST *i,INDEX *idx,int mood)
{
    //add the given item to the given list
    //push back when mood not equals to 0
    //and push front when mood equals to 1
    //at last return the item added to the list
    LIST* item = newItem(idx);
    if(mood)
    {
        i->previous = item;
        item->next = i;
        list = i;
    }
    else
    {
        i->next = item;
        item->previous = i;
    }
    listLen++;
    return item;
}

static searchType sigleList(const char *exp,const long p)
{
    //single list used to create a list for a exact search
    //but sometimes there can be multiple matching words
    INDEX* index=NULL;
    LIST *i=NULL;
    searchType type = EXACT;
    listLen = 0;
    if(p>0)//p should be positive if there is a word matched
    {
        index = (INDEX*)p;
        listLen = 1;
    }
    else
    {
        head = list = NULL;
        return type;//not matched
    }
    list = newItem(index);
    head = i = list; 
    //look forward and backward whether there are multiple matching
    while(stricmp(exp,(++index)->entry)==0)
    {
        i = addItem(i,index,0);//if so, add them to the list
        type = FUZZY;//and it becomes fuzzy search
    }
    i = list;
    index = (INDEX*)p;
    while(stricmp(exp,(--index)->entry)==0)
    {
        i = addItem(i,index,1);
        type = FUZZY;
    }
    
    return type;
}

searchType search(const char *exp)
{
    //main function of searching
    char *buff,*pstr,**queryList;
    //buff is a copy of the exp
    //pster is a pointer used in iteration
    int count=1,i,qmark=0;
    //count stores the number of substring
    //qmark stroe whether there are question marks in expression
    searchType type;
    freeList();
    buff = strdup(exp);//copy from exp
    //count how many substrings and whether there are question marks in expression
    for(pstr = buff;*pstr;pstr++)
    {
        *pstr = tolower(*pstr);
        if(*pstr=='*')
        {
            count++;
            *pstr='\0';//cut expression
        }
        else if(*pstr=='?')
            qmark=1;
    }
    if(count==1&&!qmark)
        type = sigleList(buff,exactSearch(buff,0,getDictLen()));
    //it should be exact search
    //get the search type to make sure there are no multiple matching words
    else
    {
        LIST *item;
        newList();
        queryList = malloc(sizeof(char*)*count);
        for(i=0,pstr=buff;i<count;i++)
        {
            queryList[i]=strdup(pstr);
            while(*pstr++ != '\0'); 
        }
        //cut the expression into substrings and copy them in the array queryList
        preFuzzySearch(queryList[0]);//the first substrings use to filter the searching interval
        for(i=1;i<count-1;i++)
            fuzzySearch(queryList[i]);//main part of filter
        sufFuzzySearch(queryList[count-1],count);//filter those with wrong string and those not match the last substring
        for(i=0;i<count;i++)
            free(queryList[i]);//free the temporary array
        free(queryList);
        type = FUZZY;//well, it must be fuzzy search
    }
    free(buff);
    return type;//tell the source function the searching type
}

int getListLen()
{
    return listLen;//simply return it
}

const LIST* getList()
{
    return list;//the same
}
