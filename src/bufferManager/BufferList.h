#ifndef BUFFERLIST_H
#define BUFFERLIST_H

#include "BufferListItem.h"
#include <cstddef>
class BufferList
{
    friend class BufferListItem;
    friend class BufferManager;
    BufferListItem *head;
    BufferListItem *tail;
    size_t Size = 0U;
private:
    BufferList();
    void insert(BufferListItem *item);
    void top(BufferListItem *item);
    void remove(BufferListItem *item);
    BufferListItem *last();
    const size_t &size();
};

inline BufferList::BufferList()
    :head(nullptr),tail(nullptr)
{}

inline void BufferList::insert(BufferListItem *item)
{
    if(!head)
        head = item;
    if(!tail)
        tail = item;
    else
    {
        item->Prev = tail;
        tail->Next = item;
        tail = item;
    }
}

inline void BufferList::top(BufferListItem *item)
{
    if(head==item)
        return;
    remove(item);
    item->Next = head;
    if(head)
        head->Prev = item;
    head = item;
    if(!tail) tail = item;
}

inline void BufferList::remove(BufferListItem *item)
{
    if(item->Next)
        item->Next->Prev = item->Prev;
    if(item->Prev)
        item->Prev->Next = item->Next;
    if(tail==item)
        tail = item->Prev;
    if(head == item)
        head = item->Next;
}

inline BufferListItem *BufferList::last()
{
    return tail;
}

inline const size_t &BufferList::size()
{
    return Size;
}

#endif // BUFFERLIST_H
