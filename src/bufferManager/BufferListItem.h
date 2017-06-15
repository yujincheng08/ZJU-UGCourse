#ifndef LISTITEM_H
#define LISTITEM_H

class BufferList;
class BufferListItem
{
    friend class BufferList;
    static BufferList * const List;
    BufferListItem *Prev = nullptr;
    BufferListItem *Next = nullptr;

protected:
    static BufferList * const &list();
    BufferListItem();
    void top();
public:
    virtual bool removeable() = 0;
    virtual ~BufferListItem();
};

inline BufferList * const &BufferListItem::list()
{
    return List;
}
#endif // LISTITEM_H
