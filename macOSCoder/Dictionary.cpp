//
//  Dictionary.cpp
//  macOSCoder
//
//  Copyright © 2016 Andy Warner.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software distributed
//    under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied. See the License for the
//    specific language governing permissions and limitations under the License.
//
// sample text:
// hey i really do not care what kind of hero you may think you are because he is nothing in comparison dude
#include "Dictionary.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;


class Tree
{
public:
    Tree(const std::string& newValue);
    ~Tree();
    
    // Returns tree node where node was added or NULL if the value already exists.
    Tree* add(Tree* node);
    Tree* batchAdd(Tree* node, int level);
    Tree* balance();
    // returns number of nodes
    unsigned int treeToVine(Tree* root);
    void vineToTree(Tree* root, unsigned int size);
    unsigned int getDepth(unsigned int depth = 1) const;
    
    bool lookup(const std::string& name) const;
    std::string print() const;
    std::string printTree() const;
    // path contains left and right turns to reach current node
    // the current level is path length + 1.
    void printTraverse(vector<string>& lines, unsigned int maxLevel, vector<bool>& path) const;
    
    std::string value;
    Tree* parent;
    Tree* left;
    Tree* right;
};

Tree::Tree(const std::string& newValue)
: value(newValue)
, parent(0)
, left(0)
, right(0)
{
    
}

Tree::~Tree()
{
    if (left)  delete left;
    if (right) delete right;
}

Tree* Tree::batchAdd(Tree* node, int level)
{
    if (!node) return 0;     // What?
    
    ++level;
    int compareValue = value.compare(node->value);
    if (compareValue == 0)
    {
        cout << "Word " << node->value << " already in dictionary." << endl;
        return 0;
    }
    else if (compareValue > 0)
    {
        if (!left)
        {
            left = node;
            node->parent = this;
        }
        else
        {
            Tree* newLeft = left->batchAdd(node, level);
            if (!newLeft)
            {
                return 0;
            }
        }
    }
    else if (compareValue < 0)
    {
        if (!right)
        {
            right = node;
            node->parent = this;
        }
        else
        {
            Tree* newRight = right->batchAdd(node, level);
            if (!newRight)
            {
                return 0;
            }
        }
    }
    
    return this;
}

/* Return 'this' if node is added as a decendent or return new node if there is a swap in ancestry.
 */
Tree* Tree::add(Tree* node)
{
    if (!node) return 0;     // What?
    Tree* retval = this;

    int compareValue = value.compare(node->value);
    if (compareValue == 0)
    {
        cout << "Word " << node->value << " already in dictionary." << endl;
        return 0;
    }
    else if (compareValue > 0)
    {
        if (!left)
        {
            left = node;
            node->parent = this;
        }
        else
        {
            Tree* newLeft = left->add(node);
            if (!newLeft)
            {
                return 0;
            }
            left = newLeft;
            if (!right && !left->right && node->value.compare(left->value) < 0)
            {   // swap
                cout << "Swap from the left" << endl;
                retval = left;
                left->right = this;
                left->add(node);
                left = 0;
                right = 0;
            }
        }
    }
    else if (compareValue < 0)
    {
        if (!right)
        {
            right = node;
            node->parent = this;
        }
        else
        {
            Tree* newRight = right->add(node);
            if (!newRight)
            {
                return 0;
            }
            right = newRight;
            if (!left && !right->left && node->value.compare(right->value) > 0)
            {   // swap
                cout << "Swap from the right" << endl;
                retval = right;
                right->left = this;
                left = 0;
                right = 0;
            }
        }
    }
    
    return retval;
}

// DSW based on Stout and Warren paper: http://web.eecs.umich.edu/~qstout/pap/CACM86.pdf
Tree* Tree::balance()
{
    Tree tempRoot("<root>");
    tempRoot.right = this;
    
    unsigned int count = treeToVine(&tempRoot);
    vineToTree(&tempRoot, count);
    
    Tree* retval = tempRoot.right;
    tempRoot.right = 0;     // otherwise we get deleted on out-of-scope
    return retval;
}

unsigned int Tree::treeToVine(Tree* root)
{
    Tree* vineTail  = root;
    Tree* remainder = vineTail->right;
    unsigned int vineSize = 0;
    
    while (remainder)
    {
        if (!remainder->left)
        {
            vineTail = remainder;
            remainder = remainder->right;
            ++vineSize;
        }
        else
        {   // rotate
            Tree* tmp = remainder->left;
            remainder->left = tmp->right;
            tmp->right = remainder;
            remainder = tmp;
            vineTail->right = tmp;
        }
    }
    
    return vineSize;
}

void compression(Tree* root, unsigned int count)
{
    Tree* scanner = root;
    for (int idx = 0; idx < count; ++idx)
    {
        Tree* child = scanner->right;
        scanner->right = child->right;
        scanner = scanner->right;
        child->right = scanner->left;
        scanner->left = child;
    }
}

void Tree::vineToTree(Tree* root, unsigned int size)
{
    // 2 ** log2(size + 1)
    int log2val = (int)log2(size + 1);
    log2val = log2val < 1 ? 1 : 1 << log2val;
    unsigned int leafCount = size + 1 - log2val;
    compression(root, leafCount);
    size -= leafCount;
    
    while (size > 1)
    {
        compression(root, size >> 1);
        size >>= 1;
    }
}

unsigned int Tree::getDepth(unsigned int depth) const
{
    unsigned int leftDepth = 0;
    unsigned int rightDepth = 0;
    if (left)  leftDepth  = left->getDepth(depth + 1);
    if (right) rightDepth = right->getDepth(depth + 1);
    if (leftDepth > depth) depth = leftDepth;
    if (rightDepth > depth) depth = rightDepth;
    
    return depth;
}

bool Tree::lookup(const std::string& name) const
{
    int compareValue = value.compare(name);
    if (compareValue == 0)
    {
        return true;
    }
    if (compareValue > 0)
    {
        if (!left) return false;
        return left->lookup(name);
    }
    else
    {
        if (!right) return false;
        return right->lookup(name);
    }
    
    return false;
}

std::string Tree::print() const
{
    ostringstream oss;
    if (left)  oss << left->print();
    oss << value << endl;
    if (right) oss << right->print();
    
    return oss.str();
}

// 0:                1
// 1:        2               3
// 2:    4       5       6       7
// 3:  8   9   a   b   c   d   e   f
// 4: g h i j k l m n o p q r s t u v
// distribute space before and after each node:
void Tree::printTraverse(vector<string>& lines, unsigned int maxLevel, vector<bool>& path) const
{
    unsigned int level = (unsigned int)path.size() + 1;
    if (level > 20)
    {
        cout << "Level " << level << " out of " << maxLevel << " too deep to print.";
        return;
    }
    unsigned int width = (1 << maxLevel) - 1;
    unsigned int position = ((width + 1) / 2) - 1;       // start in the middle
    for (unsigned int idx = 1; idx < level; ++idx)
    {
        unsigned int nodes = 1 << idx;
        unsigned int space = (width + 1) / (nodes * 2);
        // trace path, varying space per level
        position += path[idx - 1] ? -space : space;
    }
#if 0
    cout << "Val:" << value << " #" << nodes << " space=" << space
    << " space=" << space << " level/pos=" << level << "/" << position << endl;
#endif
    while (lines.size() < level)
    {
        lines.push_back(string(width, ' '));
    }
    
    // maxlevel - level == space to enter label
    size_t numChars = maxLevel + 1 - level;
    if (numChars > value.size()) numChars = value.size();
    if (position < numChars / 2)
    {
        position = 0;
    }
    else
    {
        position -= numChars / 2;
    }
    for (size_t idx = 0; idx < numChars; ++idx)
    {
        lines[level - 1][position + idx] = value[idx];
    }
    if (left)
    {
        size_t pathSize = path.size();
        path.push_back(true);
        left->printTraverse(lines, maxLevel, path);
        path.erase(&path[pathSize], path.end());
    }
    if (right)
    {
        size_t pathSize = path.size();
        path.push_back(false);
        right->printTraverse(lines, maxLevel, path);
        path.erase(&path[pathSize], path.end());
    }
}

string Tree::printTree() const
{
    ostringstream log;
    vector<string> lines;
    vector<bool> path;
    unsigned int depth = getDepth();
    printTraverse(lines, depth, path);
    
    log << "Print Tree (depth " << depth << ")" << endl;
    for (int idx = 0; idx < lines.size(); ++idx)
    {
        log << "|" << lines[idx] << "|" << endl;
    }
    return log.str();
}


Dictionary::Dictionary(std::ostream& logger)
: log_(logger)
, tree_(0)
, count_(0)
, depth_(0)
{
    
}

Dictionary::~Dictionary()
{
    if (tree_) delete tree_;
}

bool Dictionary::add(const std::string& name)
{
    if (!tree_)
    {
        tree_ = new Tree(name);
        count_ = 1;
        depth_ = 1;
        log_ << "Node \"" << name << "\" added as new root" << endl;
    }
    else
    {
        Tree* node = new Tree(name);
        Tree* newTree = tree_->add(node);
        if (!newTree)
        {
            log_ << "Unable to add " << name << " to dictionary" << endl;
            return false;
        }
        else
        {
            ++count_;
            tree_ = newTree;
            log_ << "Node " << name << " added, size is now " << count_ << (newTree != tree_ ? " (Swap)" : "") << endl;
            if ((float)getDepth(true) / count_  > 0.7)
            {
                log_ << "Try to balance tree" << endl;
                balance();
            }
        }
    }
    
    return true;
}

bool Dictionary::addSimple(const std::string& name)
{
    if (!tree_)
    {
        tree_ = new Tree(name);
        count_ = 1;
        depth_ = 1;
        cout << "Node \"" << name << "\" added as new root" << endl;
    }
    else
    {
        Tree* node = new Tree(name);
        Tree* newTree = tree_->batchAdd(node, 1);
        if (!newTree)
        {
            cout << "Unable to add " << name << " to dictionary" << endl;
            return false;
        }
        else
        {
            ++count_;
        }
    }
    
    return true;
}

int Dictionary::balance()
{
    if (getDepth(true) > 80)
    {
        tree_ = tree_->balance();
    }
    else
    {
        cout << "Skip balance at depth=" << getDepth() << endl;
    }
    return 0;
}

unsigned int Dictionary::getCount() const
{
    return count_;
}

unsigned int Dictionary::getDepth(bool recalculate)
{
    if (!tree_) return 0;
    if (recalculate) depth_ = tree_->getDepth();
    return depth_;
}

bool Dictionary::lookup(const std::string& name) const
{
    if (!tree_) return false;
    
    return tree_->lookup(name);
}

void Dictionary::print(bool asTree) const
{
    if (!tree_) return;
    
    if (asTree)
    {
        log_ << tree_->printTree();
    }
    else
    {
        log_ << tree_->print();
    }
}
