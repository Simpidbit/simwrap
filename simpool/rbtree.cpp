#include <iostream>

class Node {
public:
    int value;
    int color;          // red = 0, black = 1

    Node * left;
    Node * right;
    Node * parent;


public:
    Node();
    ~Node();
};


/*
   1. 节点是红色或黑色
   2. 根是黑色
   3. 叶子节点都是黑色(null 节点)
   4. 红色节点的子节点都是黑色
        - 红色节点的父节点都是黑色
        - 从根节点到叶子节点的所有路径上不能有2个连续的红色节点
    5. 从任一节点到叶子节点的所有路径都包含相同数目的黑色节点
*/
class RBTree {
public:
    Node * root;
    Node * soilder;


public:
    RBTree();
    ~RBTree();

    void insert(int value);
};

RBTree::RBTree()
{
    this->soilder = new Node();
}

RBTree::~RBTree()
{
    delete this->soilder;
}



int main()
{
    return 0;
}
