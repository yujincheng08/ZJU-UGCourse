#include <iostream>
#include <algorithm>
template<typename T, class U>
class AVLTree;

template<typename T, class U = std::less<T>>
class AVL
{
    friend class AVLTree<T, U>;
public:
    T &getElement() const;
private:
    AVL(const T &element, U &compare);

    AVL<T,U> *insert(const T &element);

    static ssize_t height(AVL<T,U> *node);

    ssize_t differHeight() const;
    void updateHeight();

    AVL<T,U> *RRRotate();
    AVL<T,U> *LLRotate();
    AVL<T,U> *RLRotate();
    AVL<T,U> *LRRotate();

private:
    T Element;
    U &Compare;
    AVL<T,U> *Left = nullptr;
    AVL<T,U> *Right = nullptr;
    ssize_t Height = 0;
};

template<typename T, class U = std::less<T>>
class AVLTree
{
private:
    AVL<T,U> *Root = nullptr;
    U Compare;
    T *find(AVL<T,U> *node, const T &element);
public:
    AVLTree(U comapre = U());
    AVLTree(const T &element, U comapre = U());
    void insert(const T&element);
    T *find(const T &element);
};

int main()
{
    int size;
    std::cin>>size;
    AVLTree<int> tree;
    for(int i = 0; i!=size; ++i)
    {
        int tmp;
        std::cin>>tmp;
        tree.insert(tmp);
    }
    while(true)
    {
        int s;
        std::cin>>s;
        int *r = tree.find(s);
        if(r)
            std::cout<<"Found "<<*r<<std::endl;
        else
            std::cout<<"Not Found"<<std::endl;
    }
    return 0;
}

template<typename T, class U>
T &AVL<T,U>::getElement() const
{
    return Element;
}

template<typename T, class U>
AVL<T,U>::AVL(const T &element, U &cpmpare)
    :Element(element), Compare(cpmpare)
{}

template<typename T, class U>
AVL<T,U>* AVL<T,U>::insert(const T &element)
{
    AVL *root = this;
    if(Compare(Element, element))
    {
        if(Right)
            Right = Right->insert(element);
        else
            Right = new AVL<T,U>(element, Compare);
    }
    else if(Compare(element, Element))
    {
        if(Left)
            Left = Left->insert(element);
        else
            Left = new AVL<T,U>(element, Compare);
    }
    switch(differHeight())
    {
    case 2:
        if(Compare(element, Left->Element))
            root = RRRotate();
        else
            root = LRRotate();
        break;
    case -2:
        if( Compare(Right->Element,element))
            root = LLRotate();
        else
            root = RLRotate();
        break;
    }
    updateHeight();
    return root;

}

template<typename T, class U>
ssize_t AVL<T,U>::height(AVL<T,U> *node)
{
    if(node)
        return node->Height;
    else
        return -1;
}

template<typename T, class U>
ssize_t AVL<T,U>::differHeight() const
{
    return height(Left) - height(Right);
}

template<typename T, class U>
void AVL<T,U>::updateHeight()
{
    Height = std::max(height(Left),height(Right)) + 1;
}

template<typename T, class U>
AVL<T,U> *AVL<T,U>::RRRotate()
{
    AVL<T,U> *root = Left;
    Left = root->Right;
    root->Right = this;

    updateHeight();
    root->updateHeight();
    return root;
}

template<typename T, class U>
AVL<T,U> *AVL<T,U>::LLRotate()
{
    AVL *root = Right;
    Right = root->Left;
    root->Left = this;

    updateHeight();
    root->updateHeight();

    return root;
}

template<typename T, class U>
AVL<T,U> *AVL<T,U>::LRRotate()
{
    Left = Left->LLRotate();
    return RRRotate();

}

template<typename T, class U>
AVL<T,U> *AVL<T,U>::RLRotate()
{
    Right = Right->RRRotate();
    return LLRotate();
}

template<typename T, class U>
AVLTree<T,U>::AVLTree(U comapre)
    :Compare(comapre)
{
}

template<typename T, class U>
AVLTree<T,U>::AVLTree(const T &element, U comapre)
    :Compare(comapre)
{
    insert(element);
}

template<typename T, class U>
void AVLTree<T,U>::insert(const T &element)
{
    if(Root)
        Root = Root->insert(element);
    else
        Root = new AVL<T,U>(element, Compare);
}

template<typename T, class U>
T *AVLTree<T,U>::find(const T &element)
{
    return find(Root, element);
}


template<typename T, class U>
T *AVLTree<T,U>::find(AVL<T, U> *node, const T &element)
{
    if(!node)
        return nullptr;
    if(Compare(node->Element, element))
    {
        return find(node->Right, element);
    }
    else if(Compare(element, node->Element))
    {
        return find(node->Left, element);
    }
    else
        return &node->Element;
}
