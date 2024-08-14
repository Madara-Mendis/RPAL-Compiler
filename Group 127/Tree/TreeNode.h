#ifndef TREENODE_H
#define TREENODE_H

#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>

using namespace std;

// TreeNode Structure
class TreeNode
{
private:
    string label;
    vector<TreeNode *> children;
    string value;

public:
    // Constructor
    explicit TreeNode(string lbl) : label(move(lbl))
    {
        children = vector<TreeNode *>();
    }

    // Add child node to the current node
    void addChild(TreeNode *child)
    {
        children.push_back(child);
    }

    // reverse children
    void reverseChildren()
    {
        reverse(children.begin(), children.end());
    }

    // Remove child node from the current node
    void removeChild(int index = 0, bool deleteNode = false)
    {
        if (index < 0 || index >= children.size())
        {
            throw out_of_range("Index out of range");
        }

        if (deleteNode)
        {
            releaseNodeMemory(children[index]);
        }

        children.erase(children.begin() + index);
    }

    // return number of children
    int getNumChildren()
    {
        return children.size();
    }

    // get the lable of the node
    string getLabel()
    {
        return label;
    }

    // return a reference to the children vector
    vector<TreeNode *> &getChildren()
    {
        return children;
    }

    // return the value of the node
    virtual string getValue()
    {
        return value;
    }

    // set value of the node
    void setValue(string v)
    {
        value = move(v);
    }

    //  release memory of the node and its children
    static void releaseNodeMemory(TreeNode *node)
    {
        if (node == nullptr)
            return;

        for (TreeNode *child : node->getChildren())
        {
            releaseNodeMemory(child);
        }

        delete node;
    }
};

// internal node structure
class InternalNode : public TreeNode
{
public:
    //  constructor
    InternalNode(const string &lbl, const string &v = " ") : TreeNode(lbl)
    {
        setValue(" ");
    }
};

// leaf node structure
class LeafNode : public TreeNode
{
public:
    //  constructor
    LeafNode(const string &lbl, const string &v) : TreeNode(lbl)
    {
        setValue(v);
    }

    void addChild(TreeNode *child) = delete;
};

#endif // TREENODE_H
