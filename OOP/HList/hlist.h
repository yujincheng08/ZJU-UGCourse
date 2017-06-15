#ifndef HLIST_H
#define HLIST_H
#include <cctype>
#include <ostream>
#include <typeinfo>
#include <cstring>
#define usingPostfix(a,b,c) inline b a(int x){return BaseIterator::c<b>(x);}
#define usingPrefix(a,b,c) inline b a(){return BaseIterator::c<b>();}
#define usingTypePostfix(a,b,c,d) inline b a(int x){return BaseIterator::c<b,d>(x);}
#define usingTypePrefix(a,b,c,d) inline b a(){return BaseIterator::c<b,d>();}
#define usingConstructor(a,b) inline a(const HList &list,BaseNode *node=nullptr):b(list,node){}

template<typename T>
class HNode;

class BaseNode
{
public:
    virtual std::ostream &output(std::ostream &out) = 0;
    virtual ~BaseNode(){}
protected:
    friend class HList;

    BaseNode(){}

private:
    template<typename T>
    inline bool operator==(const T&e);
    template <typename T>
    inline T &toValue()
    {
        return *((dynamic_cast<HNode<T>*>(this))->Element);
    }
    template<typename T>
    inline bool sametype()
    {
        return typeid(*this) == typeid(HNode<T>);
    }
    BaseNode *next = nullptr;
    BaseNode *previous = nullptr;
};


template<typename T = void*>
class HNode : public BaseNode
{
    friend class HList;
    friend class BaseNode;
private:
    T *Element;
    inline HNode(const T& element);
    inline HNode();
    virtual std::ostream &output(std::ostream &out) override;
public:
    virtual ~HNode();
};

template<typename T>
class SList;

template<typename T>
class ConstSList;

class HList
{
private:
    size_t Size;
    HNode<> *Head;
    class BaseIterator;
    template<typename T>
    BaseNode *find(BaseNode *start, const T&e) const;
public:
    class Iterator;
    template<typename T>
    class TypeIterator;

    class ReverseIterator;
    template<typename T>
    class ReverseTypeIterator;

    class ConstIterator;
    template<typename T>
    class ConstTypeIterator;

    class ConstReverseIterator;
    template<typename T>
    class ConstReverseTypeIterator;

public:
    inline HList();

    inline const size_t &size() const;

    void clear();

    template<typename T>
    HList &operator<<(const T& element);

    friend std::ostream &operator<<(std::ostream &out, const BaseIterator &iter);

    friend std::ostream &operator<<(std::ostream &out, const HList &list);

    template<typename T>
    HList &pushBack(const T& element);

    HList &pushBack(const char *element);

    template<typename T>
    HList &pushFront(const T& element);

    HList &pushFront(const char *element);

    HList &popFront();

    HList &popBack();

    template<typename T>
    HList &insert(const T& element, BaseIterator &iter);

    template<typename T>
    HList &insert(const T &start, const T &end, Iterator &iter);

    void reverse();

    void merge(HList &list);

    template<typename T>
    TypeIterator<T> nextType(const TypeIterator<T> &iter);

    template<typename T>
    ReverseTypeIterator<T> previousType(const ReverseTypeIterator<T> &iter);

    template<typename T>
    ConstTypeIterator<T> nextType(const ConstTypeIterator<T> &iter) const;

    template<typename T>
    ConstReverseTypeIterator<T> previousType(const ConstReverseTypeIterator<T> &iter) const;

    template<typename T>
    TypeIterator<T> nextType();

    template<typename T>
    ReverseTypeIterator<T> previousType();

    template<typename T>
    ConstTypeIterator<T> nextType() const;

    template<typename T>
    ConstReverseTypeIterator<T> previousType() const;

    template<typename T>
    TypeIterator<T> begin();

    template<typename T>
    TypeIterator<T> end();

    template<typename T>
    ReverseTypeIterator<T> rbegin();

    template<typename T>
    ReverseTypeIterator<T> rend();

    Iterator begin();

    Iterator end();

    ReverseIterator rbegin();

    ReverseIterator rend();

    template<typename T>
    ConstTypeIterator<T> begin() const;

    template<typename T>
    ConstTypeIterator<T> end() const;

    template<typename T>
    ConstReverseTypeIterator<T> rbegin() const;

    template<typename T>
    ConstReverseTypeIterator<T> rend() const;

    ConstIterator begin() const;

    ConstIterator end() const;

    ConstReverseIterator rbegin() const;

    ConstReverseIterator rend() const;

    template<typename T>
    TypeIterator<T> find(const T& e);

    template<typename T>
    ConstTypeIterator<T> find(const T& e) const;

    template<typename T>
    TypeIterator<T> find(const Iterator &iter, const T& e);

    template<typename T>
    ConstTypeIterator<T> find(const ConstIterator &iter, const T& e) const;

    template<typename T>
    SList<T> typeList();

    template<typename T>
    const ConstSList<T> typeList() const;

    void earse(BaseIterator &iter);

    inline ~HList();
};

class HList::BaseIterator
{
protected:
    friend class HList;
    const HList *List = nullptr;
    BaseNode *Node = nullptr;
    BaseIterator(const HList &list, BaseNode *node)
        :List(&list), Node(node){}
    template<typename T>
    inline T Next();
    template<typename T>
    inline T Next(int);
    template<typename T,typename K>
    inline T Next();
    template<typename T,typename K>
    inline T Next(int);
    template<typename T>
    inline T Previous();
    template<typename T>
    inline T Previous(int);
    template<typename T,typename K>
    inline T Previous();
    template<typename T,typename K>
    inline T Previous(int);

public:
    friend std::ostream &operator<<(std::ostream &out,const BaseIterator &iter);

    template<typename T>
    T &toValue();

    inline bool operator==(const BaseIterator &iter) const;

    inline bool operator!=(const BaseIterator &iter) const;

    virtual ~BaseIterator(){}
};

class HList::Iterator : public BaseIterator
{
    friend class HList;
    usingConstructor(Iterator,BaseIterator)
public:
    usingPostfix (operator++,Iterator,Next)

    usingPostfix (operator--,Iterator,Previous)

    usingPrefix (operator++,Iterator,Next)

    usingPrefix (operator--,Iterator,Previous)
};


template<typename T>
class HList::TypeIterator : public Iterator
{
    friend class HList;
    usingConstructor(TypeIterator,Iterator)
    T &toValue() = delete;

public:
    inline operator T();

    usingTypePostfix (operator++,TypeIterator<T>,Next,T)

    usingTypePostfix (operator--,TypeIterator<T>,Previous,T)

    usingTypePrefix (operator++,TypeIterator<T>,Next,T)

    usingTypePrefix (operator--,TypeIterator<T>,Previous,T)

    inline T &operator*();
};

class HList::ReverseIterator : public BaseIterator
{
    friend class HList;
    usingConstructor(ReverseIterator,BaseIterator)

public:
    usingPostfix (operator++,ReverseIterator,Previous)

    usingPostfix (operator--,ReverseIterator,Next)

    usingPrefix (operator++,ReverseIterator,Previous)

    usingPrefix (operator--,ReverseIterator,Next)
};


template <typename T>
class HList::ReverseTypeIterator : public ReverseIterator
{
    friend class HList;
    usingConstructor(ReverseTypeIterator,ReverseIterator)
    T &toValue() = delete;

public:
    inline operator T();

    usingTypePostfix (operator++,ReverseTypeIterator<T>,Previous,T)

    usingTypePostfix (operator--,ReverseTypeIterator<T>,Next,T)

    usingTypePrefix (operator++,ReverseTypeIterator<T>,Previous,T)

    usingTypePrefix (operator--,ReverseTypeIterator<T>,Next,T)

    inline T &operator*();
};

class HList::ConstIterator : public BaseIterator
{
    friend class HList;
    usingConstructor(ConstIterator,BaseIterator)
    inline ConstIterator(const Iterator &iter)
        :BaseIterator(*iter.List,iter.Node){}

public:
    usingPostfix (operator++,ConstIterator,Next)

    usingPostfix (operator--,ConstIterator,Previous)

    usingPrefix (operator++,ConstIterator,Next)

    usingPrefix (operator--,ConstIterator,Previous)

    template <typename T>
    inline const T &toValue();
};


template<typename T>
class HList::ConstTypeIterator : public ConstIterator
{
    friend class HList;
    usingConstructor(ConstTypeIterator,ConstIterator)
    inline ConstTypeIterator(const TypeIterator<T> &iter)
        :ConstIterator(*iter.List,iter.Node){}
    T &toValue() = delete;

public:
    inline operator const T() const;

    usingTypePostfix (operator++,ConstTypeIterator<T>,Next,T)

    usingTypePostfix (operator--,ConstTypeIterator<T>,Previous,T)

    usingTypePrefix (operator++,ConstTypeIterator<T>,Next,T)

    usingTypePrefix (operator--,ConstTypeIterator<T>,Previous,T)

    inline const T &operator*() const;
};

class HList::ConstReverseIterator : public BaseIterator
{
    friend class HList;
    usingConstructor(ConstReverseIterator,BaseIterator)
    inline ConstReverseIterator(const ReverseIterator &iter)
        :BaseIterator(*iter.List,iter.Node){}

public:
    usingPostfix (operator++,ConstReverseIterator,Previous)

    usingPostfix (operator--,ConstReverseIterator,Next)

    usingPrefix (operator++,ConstReverseIterator,Previous)

    usingPrefix (operator--,ConstReverseIterator,Next)
};


template <typename T>
class HList::ConstReverseTypeIterator : public ConstReverseIterator
{
    friend class HList;
    usingConstructor(ConstReverseTypeIterator,ConstReverseIterator)
    inline ConstReverseTypeIterator(const ReverseTypeIterator<T> &iter)
        :ConstReverseIterator(*iter.List,iter.Node){}
    T &toValue() = delete;

public:
    inline operator const T() const;

    usingTypePostfix (operator++,ConstReverseTypeIterator<T>,Previous,T)

    usingTypePostfix (operator--,ConstReverseTypeIterator<T>,Next,T)

    usingTypePrefix (operator++,ConstReverseTypeIterator<T>,Previous,T)

    usingTypePrefix (operator--,ConstReverseTypeIterator<T>,Next,T)

    inline const T &operator*() const;
};

template<typename T>
class SList
{
    HList *const hlist;
    friend class HList;

public:

    SList(HList *const list)
        :hlist(list){}

    HList::TypeIterator<T> begin();

    HList::TypeIterator<T> end();

    HList::ReverseTypeIterator<T> rbegin();

    HList::ReverseTypeIterator<T> rend();

    HList::ConstTypeIterator<T> begin() const;

    HList::ConstTypeIterator<T> end() const;

    HList::ConstReverseTypeIterator<T> rbegin() const;

    HList::ConstReverseTypeIterator<T> rend() const;
};

template<typename T>
class ConstSList
{
    const HList *const hlist;
    friend class HList;

public:
    ConstSList(const HList *const list)
        :hlist(list){}

    HList::ConstTypeIterator<T> begin() const;

    HList::ConstTypeIterator<T> end() const;

    HList::ConstReverseTypeIterator<T> rbegin() const;

    HList::ConstReverseTypeIterator<T> rend() const;
};

template<typename T>
bool BaseNode::operator==(const T &e)
{
    if(!sametype<T>())
        return false;
    if(toValue<T>()==e)
        return true;
    return false;
}


template<typename T>
T HList::BaseIterator::Next()
{
    if(Node)
        Node = Node->next;
    return *(dynamic_cast<T*>(this));
}


template<typename T>
T HList::BaseIterator::Next(int)
{
    T t= *(dynamic_cast<T*>(this));
    this->Next<T>();
    return t;
}

template<typename T,typename K>
T HList::BaseIterator::Next()
{
    for(Node = Node->next; Node != List->Head; Node = Node->next)
        if(Node->sametype<K>())
            break;
    return *(dynamic_cast<T*>(this));
}

template<typename T,typename K>
T HList::BaseIterator::Next(int)
{
    T t= *(dynamic_cast<T*>(this));
    this->Next<T,K>();
    return t;
}

template<typename T,typename K>
T HList::BaseIterator::Previous()
{
    for(Node = Node->previous; Node != List->Head; Node = Node->previous)
        if(Node->template sametype<K>())
            break;
    return *(dynamic_cast<T*>(this));
}

template<typename T,typename K>
T HList::BaseIterator::Previous(int)
{
    T t= *(dynamic_cast<T*>(this));
    this->Previous<T,K>();
    return t;
}

template<typename T>
T HList::BaseIterator::Previous()
{
    if(Node != List->Head)
        Node = Node->previous;
    return *(dynamic_cast<T*>(this));
}

template<typename T>
T HList::BaseIterator::Previous(int)
{
    T t= *(dynamic_cast<T*>(this));
    this->Previous<T>();
    return t;
}

std::ostream &operator<<(std::ostream &out, const HList::BaseIterator &iter)
{
    if(iter.Node == iter.List->Head)
        return out;
    return iter.Node->output(out);
}

bool HList::BaseIterator::operator==(const HList::BaseIterator &iter) const
{
    if(iter.Node == List->Head && Node == List-> Head)
        return true;
    if(iter.List != List)
        return false;
    return iter.Node == Node;
}

bool HList::BaseIterator::operator!=(const HList::BaseIterator &iter) const
{
    return !(*this == iter);
}

HList::HList()
    :Size(0),Head(new HNode<>(nullptr))
{
    Head->previous = Head;
    Head->next = Head;
}

const size_t &HList::size() const
{
    return Size;
}

void HList::clear()
{
    BaseNode* tail = Head->previous;
    while(tail != Head)
    {
        tail = tail->previous;
        delete tail->next;
        tail->next = nullptr;
    }
}

template<typename T>
HList::TypeIterator<T> HList::nextType(const TypeIterator<T> &iter)
{
    auto s(iter);
    return ++s;
}

template<typename T>
HList::ReverseTypeIterator<T> HList::previousType(const ReverseTypeIterator<T> &iter)
{
    auto s(iter);
    return ++s;
}

template<typename T>
HList::ConstTypeIterator<T> HList::nextType(const ConstTypeIterator<T> &iter) const
{
    auto s(iter);
    return ++s;
}

template<typename T>
HList::ConstReverseTypeIterator<T> HList::previousType(const ConstReverseTypeIterator<T> &iter) const
{
    auto s(iter);
    return ++s;
}


template<typename T>
HList::TypeIterator<T> HList::nextType()
{
    TypeIterator<T> iter(*this,Head->next);
    if(Head->next->sametype<T>())
        return iter;
    return ++iter;
}

template<typename T>
HList::ReverseTypeIterator<T> HList::previousType()
{
    ReverseTypeIterator<T> iter(*this,Head->previous);
    if(Head->previous->sametype<T>())
        return iter;
    return ++iter;
}

template<typename T>
HList::ConstTypeIterator<T> HList::nextType() const
{
    ConstTypeIterator<T> iter(*this,Head->next);
    if(Head->next->sametype<T>())
        return iter;
    return ++iter;
}

template<typename T>
HList::ConstReverseTypeIterator<T> HList::previousType() const
{
    ConstReverseTypeIterator<T> iter(*this,Head->previous);
    if(Head->previous->sametype<T>())
        return iter;
    return ++iter;
}


template<typename T>
HList::TypeIterator<T> HList::begin()
{
    return nextType<T>();
}

template<typename T>
HList::TypeIterator<T> HList::end()
{
    return TypeIterator<T>(*this,Head);
}

template<typename T>
HList::ReverseTypeIterator<T> HList::rbegin()
{
    return previousType<T>();
}


template<typename T>
HList::ReverseTypeIterator<T> HList::rend()
{
    return ReverseTypeIterator<T>(*this,Head);
}

template<typename T>
HNode<T>::HNode(const T &element)
   :Element(new T(element))
{
}

HList::Iterator HList::begin()
{
    return Iterator(*this,Head->next);
}

HList::Iterator HList::end()
{
    return Iterator(*this, Head);
}

HList::ReverseIterator HList::rbegin()
{
    return ReverseIterator(*this,Head->previous);
}

HList::ReverseIterator HList::rend()
{
    return ReverseIterator(*this,Head);
}

template<typename T>
HList::ConstTypeIterator<T> HList::begin() const
{
    return nextType<T>();
}

template<typename T>
HList::ConstTypeIterator<T> HList::end() const
{
    return ConstTypeIterator<T>(*this, Head);
}

template<typename T>
HList::ConstReverseTypeIterator<T> HList::rbegin() const
{
    return previousType<T>();
}


template<typename T>
HList::ConstReverseTypeIterator<T> HList::rend() const
{
    return ConstReverseTypeIterator<T>(*this,Head);
}

HList::ConstIterator HList::begin() const
{
    return ConstIterator(*this,Head->next);
}

HList::ConstIterator HList::end() const
{
    return ConstIterator(*this, Head);
}

HList::ConstReverseIterator HList::rbegin() const
{
    return ConstReverseIterator(*this,Head->previous);
}

HList::ConstReverseIterator HList::rend() const
{
    return ConstReverseIterator(*this,Head);
}

void HList::earse(HList::BaseIterator &iter)
{
    if(iter.List->Head != Head || iter.Node == Head)
        return;
    iter.Node->previous->next = iter.Node->next;
    iter.Node->next->previous = iter.Node->previous;
    delete iter.Node;
    iter.Node = Head;
}


template<typename T>
BaseNode *HList::find(BaseNode *start, const T&e) const
{
    for(auto i = start->next; i != Head; i = i->next)
        if(*i == e)
            return i;
    return nullptr;
}

template<typename T>
HList::TypeIterator<T> HList::find(const T& e)
{
    return TypeIterator<T>(*this,find(Head,e));
}

template<typename T>
HList::ConstTypeIterator<T> HList::find(const T &e) const
{
    return ConstTypeIterator<T>(*this,find(Head,e));
}

template<typename T>
HList::TypeIterator<T> HList::find(const HList::Iterator &iter, const T &e)
{
    if(iter.List->Head != Head)
        return TypeIterator<T>(*this);
    return TypeIterator<T>(*this,find(iter.Node,e));
}

template<typename T>
HList::ConstTypeIterator<T> HList::find(const HList::ConstIterator &iter, const T &e) const
{
    if(iter.List->Head != Head)
        return TypeIterator<T>(*this);
    return ConstTypeIterator<T>(*this,find(iter.Node,e));
}

HList::~HList()
{
    clear();
    delete Head;
}


template<typename T>
HList &HList::operator<<(const T &element)
{
    return pushBack(element);
}

std::ostream &operator<<(std::ostream &out, const HList &list)
{
    for(auto i = list.begin(); i!=list.end(); ++i)
        out<< i << '\t';
    return out;
}

HList &HList::pushBack(const char *element)
{
    return pushBack(std::string(element));
}

HList &HList::pushFront(const char *element)
{
    return pushFront(std::string(element));
}

HList &HList::popFront()
{
    auto pop = Head->next;
    if(Head == pop)
        return *this;
    Head->next = pop->next;
    pop->next->previous = Head;
    delete pop;
    --Size;
    return *this;

}

HList &HList::popBack()
{
    auto pop = Head->previous;
    if(pop == Head)
        return *this;
    Head->previous = pop->previous;
    pop->previous->next = Head;
    delete pop;
    --Size;
    return *this;
}

void HList::reverse()
{
    BaseNode * i = Head->next, *next;
    Head->next = Head;
    Head->previous = Head;
    for(; i != Head; i = next)
    {
        next = i->next;
        i->next = Head->next;
        i->next->previous = i;
        Head->next = i;
        i->previous = Head;
    }
}

void HList::merge(HList &list)
{
    auto &tail = Head->previous;
    tail->next = list.Head->next;
    list.Head->next->previous = tail;
    Head->previous = list.Head->previous;
    Head->previous->next = Head;
    Size += list.Size;
    list.Size = 0U;
    list.Head->next = list.Head;
    list.Head->previous = list.Head;
}

template<typename T>
HList &HList::pushBack(const T &element)
{
    HNode<T> *newNode = new HNode<T>(element);
    auto &tail = Head->previous;
    tail->next = newNode;
    newNode->previous = tail;
    newNode->next = Head;
    Head->previous = newNode;
    ++Size;
    return *this;
}

template<typename T>
HList &HList::pushFront(const T &element)
{
    HNode<T> *newNode = new HNode<T>(element);
    auto &head = Head->next;
    head->previous = newNode;
    newNode->next = head;
    newNode->previous = Head;
    Head->next = newNode;
    ++Size;
    return *this;
}

template<typename T>
HNode<T>::HNode()
    :Element(nullptr)
{
}

template<typename T>
std::ostream &HNode<T>::output(std::ostream &out)
{
    out<<*Element;
    return out;
}

template<typename T>
HNode<T>::~HNode()
{
    delete Element;
}

template<typename T>
T &HList::BaseIterator::toValue()
{
    return Node->toValue<T>();
}

template<typename T>
HList::TypeIterator<T>::operator T()
{
    return **this;
}

template<typename T>
T &HList::TypeIterator<T>::operator*()
{
    return Node->toValue<T>();
}


template<typename T>
HList::ReverseTypeIterator<T>::operator T()
{
    return **this;
}

template<typename T>
T &HList::ReverseTypeIterator<T>::operator*()
{
    return Node->toValue<T>();
}


template<typename T>
const T &HList::ConstIterator::toValue()
{
    return Node->toValue<T>();
}

template<typename T>
HList::ConstTypeIterator<T>::operator const T() const
{
    return **this;
}

template<typename T>
const T &HList::ConstTypeIterator<T>::operator*() const
{
    return Node->toValue<T>();
}

template<typename T>
HList::ConstReverseTypeIterator<T>::operator const T() const
{
    return **this;
}

template<typename T>
const T &HList::ConstReverseTypeIterator<T>::operator*() const
{
    return Node->toValue<T>();
}


template<typename T>
HList::TypeIterator<T> SList<T>::begin()
{
    return hlist->begin<T>();
}

template<typename T>
HList::TypeIterator<T> SList<T>::end()
{
    return hlist->end<T>();
}

template<typename T>
HList::ReverseTypeIterator<T> SList<T>::rbegin()
{
    return hlist->rbegin<T>();
}

template<typename T>
HList::ReverseTypeIterator<T> SList<T>::rend()
{
    return hlist->rend<T>();
}

template<typename T>
HList::ConstTypeIterator<T> SList<T>::begin() const
{
    return hlist->begin<T>();
}

template<typename T>
HList::ConstTypeIterator<T> SList<T>::end() const
{
    return hlist->end<T>();
}

template<typename T>
HList::ConstReverseTypeIterator<T> SList<T>::rbegin() const
{
    return hlist->rbegin<T>();
}

template<typename T>
HList::ConstReverseTypeIterator<T> SList<T>::rend() const
{
    return hlist->rend<T>();
}

template<typename T>
HList::ConstTypeIterator<T> ConstSList<T>::begin() const
{
    return hlist->begin<T>();
}

template<typename T>
HList::ConstTypeIterator<T> ConstSList<T>::end() const
{
    return hlist->end<T>();
}

template<typename T>
HList::ConstReverseTypeIterator<T> ConstSList<T>::rbegin() const
{
    return hlist->rbegin<T>();
}

template<typename T>
HList::ConstReverseTypeIterator<T> ConstSList<T>::rend() const
{
    return hlist->rend<T>();
}

template<typename T>
SList<T> HList::typeList()
{
    return SList<T>(this);
}

template<typename T>
const ConstSList<T> HList::typeList() const
{
    return ConstSList<T>(this);
}

template<typename T>
HList &HList::insert(const T &element, BaseIterator &iter)
{
    HNode<T> *newNode = new HNode<T>(element);
    newNode->next = iter.Node->next;
    newNode->previous = newNode;
    iter.Node->next = newNode;
    newNode->previous = iter.Node;
    ++Size;
    return *this;
}

template<typename T>
HList &HList::insert(const T &start, const T &end, Iterator &iter)
{
    auto j = iter;
    for(auto i = start; i!= end; ++i, ++iter)
        insert(*i, iter);
    return *this;
}

#endif // HLIST_H
