#ifndef TREE_H
#define TREE_H

#include "TreeNode.h"
using namespace std;

void generateST(TreeNode *currentNode, TreeNode *parentNode);

// tree structure
class Tree
{
private:
    static Tree *tree;           // singleton instance of the tree
    TreeNode *astRoot = nullptr; // root node of the ast
    TreeNode *stRoot = nullptr;  // root node of the st

    // constructor and destructor
    Tree() {}
    ~Tree() {}

    Tree(const Tree &) = delete;

    Tree &operator=(const Tree &) = delete;

public:
    static Tree &getInstance() // get the singleton instance of the tree
    {
        return *tree;
    }

    void setASTRoot(TreeNode *r) // set the root node of the ast
    {
        astRoot = r;
    }

    TreeNode *getASTRoot() // get the root node of the ast
    {
        return astRoot;
    }

    void setSTRoot(TreeNode *r) // set the root node of the st
    {
        stRoot = r;
    }

    TreeNode *getSTRoot() // get the root node of the st
    {
        return stRoot;
    }

    static void releaseASTMemory() // release the memory of the ast
    {
        if (tree->astRoot != nullptr)
        {
            if (tree->stRoot == nullptr)
            {
                tree->stRoot = tree->astRoot;
            }
            tree->astRoot = nullptr;
        }
    }

    static void releaseSTMemory() // release the memory of the st
    {
        TreeNode::releaseNodeMemory(tree->stRoot);
    }

    // generate st from the ast
    static void generate()
    {
        releaseASTMemory();
        generateST(tree->stRoot, nullptr);
    }
};

#endif // TREE_H