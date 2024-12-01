#ifndef _NODE_H_
#define _NODE_H_

#include <bits/stdc++.h>
#include "Storage.h"
#include "FunctionTable.h"

using namespace std;

class Noncopyable // 防止copy用
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

private:
    Noncopyable(const Noncopyable &);
    const Noncopyable &operator=(const Noncopyable &) {}
};

class Node : private Noncopyable // abstract class
{
public:
    virtual double Calculate() const = 0;
    virtual bool is_Lvalue() const { return false; }
    virtual void Assign(double value) { assert(!"Assign() called incorrectly"); }
    virtual ~Node(){};
};

class NumberNode : public Node
{
private:
    const double number;

public:
    NumberNode(double number) : number(number) {}
    virtual double Calculate() const { return this->number; }
};

class BinaryNode : public Node // is an abstract class
{
protected:
    Node *const left; // 指標所指向位址不可改變
    Node *const right;

public:
    BinaryNode(Node *left, Node *right) : left(left), right(right) {}
    ~BinaryNode();
};

class UnaryNode : public Node // is an abstract class
{
public:
    UnaryNode(Node *child) : child(child) {}
    ~UnaryNode() { delete this->child; }

protected:
    Node *const child;
};

class MultipleNode : public Node
{
protected:
    vector<Node *> children;
    vector<bool> positives;

public:
    MultipleNode(Node *node) { AppendChild(node, true); } // 第一個節點必為正(負號已經交由UminusNode處理了)
    void AppendChild(Node *node, bool positive);
    ~MultipleNode();
};

class UMinusNode : public UnaryNode
{
public:
    UMinusNode(Node *child) : UnaryNode(child) {}
    double Calculate() const { return ((-1) * child->Calculate()); }
};

class FunctionaNode : public UnaryNode
{
private:
    function_ptr fptr;

public:
    FunctionaNode(Node *child, function_ptr fptr) : UnaryNode(child), fptr(fptr) {}
    double Calculate() const { return (*fptr)(child->Calculate()); }
};

class SumNode : public MultipleNode
{
public:
    SumNode(Node *node) : MultipleNode(node) {}
    double Calculate() const;
};

class ProductNode : public MultipleNode
{
public:
    ProductNode(Node *node) : MultipleNode(node) {}
    double Calculate() const;
};

class VariableNode : public Node
{
private:
    const unsigned int id;
    Storage &storage;

public:
    VariableNode(unsigned int id, Storage &storage) : id(id), storage(storage) {}
    double Calculate() const;
    bool is_Lvalue() const { return true; }
    void Assign(double value) { storage.set_value(id, value); }
};

class AssignNode : public BinaryNode
{
public:
    AssignNode(Node *left, Node *right) : BinaryNode(left, right) { assert(left->is_Lvalue()); }
    double Calculate() const;
};

/* BinaryNode */
BinaryNode::~BinaryNode()
{
    delete this->left;
    delete this->right;
}

/* MultipleNode */
void MultipleNode::AppendChild(Node *node, bool positive)
{
    children.push_back(node);
    positives.push_back(positive);
}

MultipleNode::~MultipleNode()
{
    vector<Node *>::const_iterator it; // it為指向指標(node*)的指標
    for (it = children.begin(); it != children.end(); ++it)
        delete *it;
}

/* SumNode */
double SumNode::Calculate() const
{
    double result = 0;
    vector<Node *>::const_iterator childIt = children.begin();
    vector<bool>::const_iterator positiveIt = positives.begin();

    for (; childIt != children.end(); ++childIt, ++positiveIt)
    {
        assert(positiveIt != positives.end());
        double value = (*childIt)->Calculate();

        if (*positiveIt == true)
            result += value;
        else
            result -= value;
    }
    assert(positiveIt == positives.end());

    return result;
}

/* ProductNode */
double ProductNode::Calculate() const
{
    double result = 1;
    vector<Node *>::const_iterator childIt = children.begin();
    vector<bool>::const_iterator positiveIt = positives.begin();

    for (; childIt != children.end(); ++childIt, ++positiveIt)
    {
        assert(positiveIt != positives.end());
        double value = (*childIt)->Calculate();

        if (*positiveIt == true)
            result *= value;
        else if (value != 0)
            result /= value;
        else
        {
            cout << "Error!!! Divison by zero" << endl;
            return HUGE_VAL;
        }
    }
    assert(positiveIt == positives.end());

    return result;
}

/* VariableNode */
double VariableNode::Calculate() const
{
    double x = 0;

    if (storage.is_initialized(id))
        x = storage.get_value(id);
    else
        throw SyntaxError("Syntax error: use of uninitialized variable");

    return x;
}

/* AssignNode */
double AssignNode::Calculate() const
{
    double x = 0;

    x = right->Calculate();
    left->Assign(x);

    return right->Calculate();
}

#endif // _NODE_H_
