#include <iostream>

using namespace std;

class AVL
{
    friend class AVLTree;
public:
    int getElement() const;
private:
    AVL(int element);

    AVL *insert(int element);

    static int height(AVL *node);

    int differHeight() const;
    void updateHeight();

    AVL *RRRotate();
    AVL *LLRotate();
    AVL *RLRotate();
    AVL *LRRotate();

private:
    int Element;
    AVL *Left;
    AVL *Right;
    int Height;

};

class AVLTree
{
private:
    AVL *Root;
public:
    AVLTree();
    AVLTree(int element);
    void insert(int element);
    int getRootElement() const;
    AVL *getRoot() const;
};


int main()
{
    int size;
    cin>>size;
    AVLTree tree;
    for(int i = 0; i!=size; ++i)
    {
        int tmp;
        cin>>tmp;
        tree.insert(tmp);
    }
    cout<<tree.getRootElement()<<endl;
    return 0;
}

int AVL::getElement() const
{
    return Element;
}

AVL::AVL(int element)
    :Element(element),
      Left(nullptr),
      Right(nullptr),
      Height(0)
{

}

AVL* AVL::insert(int element)
{
    AVL *root = this;
    if(element > Element)
    {
        if(Right)
            Right = Right->insert(element);
        else
            Right = new AVL(element);
    }
    else if(element < Element)
    {
        if(Left)
            Left = Left->insert(element);
        else
            Left = new AVL(element);
    }
    switch(differHeight())
    {
    case 2:
        if(element < Left->Element)
            root = RRRotate();
        else
            root = LRRotate();
        break;
    case -2:
        if(element > Right->Element)
            root = LLRotate();
        else
            root = RLRotate();
        break;
    }
    updateHeight();
    return root;

}

int AVL::height(AVL *node)
{
    if(node)
        return node->Height;
    else
        return -1;
}

int AVL::differHeight() const
{
    return height(Left) - height(Right);
}

void AVL::updateHeight()
{
    Height = max(height(Left),height(Right)) + 1;
}

AVL *AVL::RRRotate()
{
    AVL *root = Left;
    Left = root->Right;
    root->Right = this;

    updateHeight();
    root->updateHeight();
    return root;
}

AVL *AVL::LLRotate()
{
    AVL *root = Right;
    Right = root->Left;
    root->Left = this;

    updateHeight();
    root->updateHeight();

    return root;
}

AVL *AVL::LRRotate()
{
    Left = Left->LLRotate();
    return RRRotate();

}

AVL *AVL::RLRotate()
{
    Right = Right->RRRotate();
    return LLRotate();
}

AVLTree::AVLTree()
    :Root(nullptr)
{
}

AVLTree::AVLTree(int element)
    :Root(nullptr)
{
    insert(element);
}

void AVLTree::insert(int element)
{
    if(Root)
        Root = Root->insert(element);
    else
        Root = new AVL(element);
}

int AVLTree::getRootElement() const
{
    return Root->Element;
}

AVL *AVLTree::getRoot() const
{
    return Root;
}
