#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* thisNode);
    void removeFix(AVLNode<Key, Value>* parent, int8_t diff);
    void rotateRight(AVLNode<Key,Value>* thisNode);
    void rotateLeft(AVLNode<Key,Value>* thisNode);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    if (this->root_ == nullptr) // If the tree is empty
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    
    AVLNode<Key, Value>* currentNode = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* nextNode;
    
    while (true)
    {
        if (currentNode->getKey() == new_item.first)
        {
            currentNode->setValue(new_item.second);
            return;
        }
        else if (new_item.first < currentNode->getKey())
        {
            nextNode = currentNode->getLeft();
        }
        else
        {
            nextNode = currentNode->getRight();
        }
        
        if (nextNode == nullptr) break;
        else currentNode = nextNode;
    }
    // by this point, currentNode is the parent, nextNode is where to insert
    
    // Creating the new node
    AVLNode<Key, Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, currentNode);
    
    // updating the parent nodes left/right
    if (new_item.first < currentNode->getKey())
    {
        currentNode->setLeft(newNode);
    }
    else
    {
        currentNode->setRight(newNode);
    }


    // if parent node's balance is +- 1
    int parentBalance = newNode->getParent()->getBalance();
    
    //Update parent node's balance
    if (newNode->getParent()->getLeft() == newNode) newNode->getParent()->updateBalance(-1);
    else newNode->getParent()->updateBalance(1);
    if (parentBalance == 1 || parentBalance == -1)
    {
        newNode->getParent()->setBalance(0);
        return;
    }

    // if parent node's balance is 0: gotta fricking rotate and fix
    insertFix(newNode->getParent(), newNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* thisNode)
{
    if (parent == nullptr) return;
    if (parent->getParent() == nullptr) return;

    AVLNode<Key, Value>* grandParent = parent->getParent();

    if (grandParent->getLeft() == parent)
    {
        grandParent->updateBalance(-1);
        if (grandParent->getBalance() == 0) return;
        if (grandParent->getBalance() == -1) insertFix(grandParent, parent);
        else if (grandParent->getBalance() == -2)
        {
            if (parent->getLeft() == thisNode)
            {
                rotateRight(grandParent);
                grandParent->setBalance(0);
                parent->setBalance(0);
                return;
            }
            if (parent->getRight() == thisNode)
            {
                rotateLeft(parent);
                rotateRight(grandParent);
            }

            if (thisNode->getBalance() == -1)
            {
                parent->setBalance(0);
                grandParent->setBalance(1);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 0)
            {
                parent->setBalance(0);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 1)
            {
                parent->setBalance(-1);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
        }
    }
    else
    {
        grandParent->updateBalance(1);
        if (grandParent->getBalance() == 0) return;
        if (grandParent->getBalance() == 1) insertFix(grandParent, parent);
        else if (grandParent->getBalance() == 2)
        {
            if (parent->getRight() == thisNode)
            {
                rotateLeft(grandParent);
                grandParent->setBalance(0);
                parent->setBalance(0);
                return;
            }
            if (parent->getLeft() == thisNode)
            {
                rotateRight(parent);
                rotateLeft(grandParent);
            }

            if (thisNode->getBalance() == 1)
            {
                parent->setBalance(0);
                grandParent->setBalance(-1);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 0)
            {
                parent->setBalance(0);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == -1)
            {
                parent->setBalance(1);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
        }
    }
}

/*
    if (grandParent->getBalance() == 0) return;
    if (grandParent->getBalance() == 1 || grandParent->getBalance() == -1) insertFix(grandParent, parent);
    else if (grandParent->getBalance() == 2 || grandParent->getBalance() == -2)
    {
        // zig zig going left
        if (grandParent->getLeft() == parent && parent->getLeft() == thisNode)
        {
            rotateRight(grandParent);
            grandParent->setBalance(0);
            parent->setBalance(0);
            return;
        }
        // zig zig going right
        if (grandParent->getRight() == parent && parent->getRight() == thisNode)
        { 
            rotateLeft(grandParent);
            grandParent->setBalance(0);
            parent->setBalance(0);
            return;
        }

        // zig zag left then right
        if (grandParent->getLeft() == parent && parent->getRight() == thisNode)
        {
            rotateLeft(parent);
            rotateRight(grandParent);
        }
        // zig zag right then left
        else if (grandParent->getRight() == parent && parent->getLeft() == thisNode)
        {
            rotateRight(parent);
            rotateLeft(grandParent);
        }
        // updating balance in the case of zig zag
        if (grandParent->getLeft() == parent)
        {
            if (thisNode->getBalance() == -1)
            {
                parent->setBalance(0);
                grandParent->setBalance(1);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 0)
            {
                parent->setBalance(0);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 1)
            {
                parent->setBalance(-1);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
        }
        else
        {
            if (thisNode->getBalance() == 1)
            {
                parent->setBalance(0);
                grandParent->setBalance(-1);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == 0)
            {
                parent->setBalance(0);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
            else if (thisNode->getBalance() == -1)
            {
                parent->setBalance(1);
                grandParent->setBalance(0);
                thisNode->setBalance(0);
            }
        }
        
    } 
}
*/

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (nodeToRemove == nullptr) return;
    if (nodeToRemove == this->root_ && nodeToRemove->getLeft() == nullptr && nodeToRemove->getRight() == nullptr)
    {
        delete nodeToRemove;
        this->root_ = nullptr;
        return;
    }

    if (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr)
    {
        nodeSwap(nodeToRemove, static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(nodeToRemove)));
    }

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();
    int8_t diff;
    if (parent != nullptr)
    {
        if (parent->getLeft() == nodeToRemove) diff = 1;
        else diff = -1;
        //parent->updateBalance(diff);
    }

    // delete node and update pointers
    AVLNode<Key, Value>* left = nodeToRemove->getLeft();
    AVLNode<Key, Value>* right = nodeToRemove->getRight();
    if (left != nullptr)
    {
        if (nodeToRemove == this->root_)
        {
            this->root_ = left;
            left->setParent(nullptr);
        }
        else if (parent->getRight() == nodeToRemove)
        {
            parent->setRight(left);
            left->setParent(parent);
        }
        else
        {
            parent->setLeft(left);
            left->setParent(parent);
        }
    }
    else if (right != nullptr)
    {
        if (nodeToRemove == this->root_)
        {
            this->root_ = right;
            right->setParent(nullptr);
        }
        else if (parent->getRight() == nodeToRemove)
        {
            parent->setRight(right);
            right->setParent(parent);
        }
        else
        {
            parent->setLeft(right);
            right->setParent(parent);
        }
    }
    else
    {
        if (parent->getLeft() == nodeToRemove) 
        {
            parent->setLeft(nullptr);
        }
        else
        {
            parent->setRight(nullptr);
        }
    }
    delete nodeToRemove;

    //patch tree
    removeFix(parent, diff);
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* parent, int8_t diff)
{
    if (parent == nullptr) return;

    AVLNode<Key, Value>* nextParent = parent->getParent();
    int8_t ndiff;
    if (nextParent != nullptr)
    {
        if (nextParent->getLeft() == parent) ndiff = 1;
        else ndiff = -1;
        //nextParent->updateBalance(diff);
    }

    int8_t balance = parent->getBalance();
    if (diff == -1)
    {
        if (balance + diff == -2)
        {
            AVLNode<Key, Value>* tallerChild = parent->getLeft();
            if (tallerChild->getBalance() == -1)
            {
                rotateRight(parent);
                parent->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(nextParent, ndiff);
            }
            else if (tallerChild->getBalance() == 0)
            {
                rotateRight(parent);
                parent->setBalance(-1);
                tallerChild->setBalance(1);
            }
            else if (tallerChild->getBalance() == 1)
            {
                AVLNode<Key, Value>* grandChild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(parent);
                if (grandChild->getBalance() == 1)
                {
                    parent->setBalance(0);
                    tallerChild->setBalance(-1);
                    grandChild->setBalance(0);
                }
                else if (grandChild->getBalance() == 0)
                {
                    parent->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if (grandChild->getBalance() == -1)
                {
                    parent->setBalance(1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(nextParent, ndiff);
            }
            return;
        }

        if (balance + diff == -1)
        {
            parent->setBalance(-1);
            return;
        }

        if (balance + diff == 0)
        {
            parent->setBalance(0);
            removeFix(nextParent, ndiff);
        }
    }

    
    else if (diff == 1)
    {
        if (balance + diff == 2)
        {
            AVLNode<Key, Value>* tallerChild = parent->getRight();
            if (tallerChild->getBalance() == 1)
            {
                rotateLeft(parent);
                parent->setBalance(0);
                tallerChild->setBalance(0);
                removeFix(nextParent, ndiff);
            }
            else if (tallerChild->getBalance() == 0)
            {
                rotateLeft(parent);
                parent->setBalance(1);
                tallerChild->setBalance(-1);
            }
            else if (tallerChild->getBalance() == -1)
            {
                AVLNode<Key, Value>* grandChild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(parent);
                if (grandChild->getBalance() == -1)
                {
                    parent->setBalance(0);
                    tallerChild->setBalance(1);
                    grandChild->setBalance(0);
                }
                else if (grandChild->getBalance() == 0)
                {
                    parent->setBalance(0);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                else if (grandChild->getBalance() == 1)
                {
                    parent->setBalance(-1);
                    tallerChild->setBalance(0);
                    grandChild->setBalance(0);
                }
                removeFix(nextParent, ndiff);
            }
            return;
        }

        if (balance + diff == 1)
        {
            parent->setBalance(1);
            return;
        }

        if (balance + diff == 0)
        {
            parent->setBalance(0);
            removeFix(nextParent, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* grandParent)
{
    AVLNode<Key, Value>* parent = grandParent->getRight();
    AVLNode<Key, Value>* parentOriginalLeft = parent->getLeft();
    parent->setLeft(grandParent);
    grandParent->setRight(parentOriginalLeft);
    if (parentOriginalLeft != nullptr) parentOriginalLeft->setParent(grandParent);

    if (grandParent->getParent() != nullptr) //grandParent has a parent
    {
        parent->setParent(grandParent->getParent());
        if (grandParent->getParent()->getLeft() == grandParent) grandParent->getParent()->setLeft(parent);
        else grandParent->getParent()->setRight(parent);
    }
    else //grandParent does not have a parent, meaning its the root
    {
        parent->setParent(nullptr);
        this->root_ = parent;
    }
    grandParent->setParent(parent);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* grandParent)
{
    AVLNode<Key, Value>* parent = grandParent->getLeft();
    AVLNode<Key, Value>* parentOriginalRight = parent->getRight();
    parent->setRight(grandParent);
    grandParent->setLeft(parentOriginalRight);
    if (parentOriginalRight != nullptr) parentOriginalRight->setParent(grandParent);

    if (grandParent->getParent() != nullptr) //grandParent has a parent
    {
        parent->setParent(grandParent->getParent());
        if (grandParent->getParent()->getLeft() == grandParent) grandParent->getParent()->setLeft(parent);
        else grandParent->getParent()->setRight(parent);
    }
    else //grandParent does not have a parent, meaning its the root
    {
        parent->setParent(nullptr);
        this->root_ = parent;
    }
    grandParent->setParent(parent);
}


#endif
