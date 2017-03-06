#ifndef HLIST_H
#define HLIST_H
#include <cctype>
#include <ostream>
namespace HListBase
{
template<typename T>
class HNode;
struct Base
{
    template <typename T>
    T &toValue();
    virtual void output(std::ostream &out) = 0;
    virtual ~Base() = 0;
};

template<typename T>
T &HListBase::Base::toValue()
{
    return *((dynamic_cast<HNode<T*>>(this)).Element);
}
}

template<typename T = void*>
class HNode : public HListBase::Base
{
    friend class HList;
private:
    T Element;
    Base *next;
    Base *previous;
    inline HNode(const T& element);
    inline virtual ~HNode() override;
    virtual void output(std::ostream &out) override;
};

class HList
{
private:
    size_t Size;
    HNode<>* Head;
    HNode<>* Tail;
public:
    inline HList();
    inline const size_t &size() const;
    template<typename T>
    inline T &front() const;
    void clear();
    template<typename T>
    void append(const T& element);
    template<typename T>
    void insert(const T& element);
    class Iterator;
    template<typename T>
    class TypeIterator;
    ~HList();
};

template<typename T>
T &HList::front() const
{
    return Head->next();
}

HList::HList()
    :Size(0),Head(new HNode<>(nullptr)),Tail(Head)
{
}

const size_t &HList::size() const
{
    return Size;
}

void HList::clear()
{

}

HList::~HList()
{
    clear();
}


template<typename T>
void HList::append(const T &element)
{
    HNode<T> *newNode = new HNode<T>(element);
    Tail->next = newNode;
    newNode->previous = Tail;
    Tail = Tail->next;
    ++Size;
}

class HList::Iterator
{
private:
    HListBase::Base *node;
public:
    HList::Iterator operator++(int);
    HList::Iterator operator++();
    HList::Iterator operator--(int);
    HList::Iterator operator--();
    template<typename T>
    T &toValue();
};

template<typename T>
HNode<T>::HNode(const T &element)
   :Element(element),next(nullptr),previous(nullptr)
{
}

template<typename T>
HNode<T>::~HNode()
{
}

template<typename T>
void HNode<T>::output(std::ostream &out)
{
    out<<Element;
}

template<typename T>
void HList::insert(const T &element)
{
}


template<typename T>
T &HList::Iterator::toValue()
{
    return node->toValue<T>();
}
#endif // HLIST_H
