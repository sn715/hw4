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
		void rotateLeft(AVLNode<Key, Value>* node);
		void rotateRight(AVLNode<Key, Value>* node);
		void insertHelper(AVLNode<Key, Value>* node);
		void removeHelper(AVLNode<Key, Value>* node, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

		//create a new node
		if (this->root_ == NULL) {
			this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
			return;
		}

		AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
		AVLNode<Key, Value>* parent = NULL;

		//find where to insert it
		while (current != NULL) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        }
        else if (new_item.first > current->getKey()) {
            current = current->getRight();
        }
        else { 
            current->setValue(new_item.second);
            return;
        }
    }

		AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
		if (new_item.first < parent->getKey()) {
			parent->setLeft(newNode);
		}
		else {
			parent->setRight(newNode);
		}

		insertHelper(newNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
		AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
		if (node == NULL) {
			return;
		}

		//node has 2 children -> swap with predecessor
		if(node->getLeft() != NULL && node->getRight() != NULL) {
			AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
			if (pred != NULL) {
				nodeSwap(node, pred);
			}
		}

		AVLNode<Key, Value>* parent = node->getParent();
		AVLNode<Key, Value>* child = NULL;

		if (node->getLeft() != NULL) {
			child = node->getLeft();
		}
		else {
			child = node->getRight();
		}

		if (child != NULL) {
			child->setParent(parent);
		}

		int diff = 0;
		if(parent == NULL) {
			this->root_ = child;
		}
		else if(parent->getLeft() == node) {
			parent->setLeft(child);
			diff = 1;
		}
		else {
			parent->setRight(child);
			diff = -1;
		}

		delete node;

		if(parent != NULL) {
			removeHelper(parent, diff);
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

//HELPER FUNCTIONS

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
	if (node == NULL) {
		return;
	}

	AVLNode<Key,Value>* rChild = node->getRight();
	if (rChild == NULL) {
		return;
	}

	AVLNode<Key,Value>* parent = node->getParent();
	node->setRight(rChild->getLeft());
	if(rChild->getLeft() != NULL) {
		rChild->getLeft()->setParent(node);
	}

	rChild->setParent(parent);
	if(parent == NULL) {
		this->root_ = rChild;
	}
	else if (parent->getLeft() == node) {
		parent->setLeft(rChild);
	}
	else {
		parent->setRight(rChild);
	}

	rChild->setLeft(node);
	node->setParent(rChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
	if (node == NULL) {
		return;
	}

	AVLNode<Key,Value>* lChild = node->getLeft();
	if (lChild == NULL) {
		return;
	}

	AVLNode<Key,Value>* parent = node->getParent();
	node->setLeft(lChild->getRight());
	if(lChild->getRight() != NULL) {
		lChild->getRight()->setParent(node);
	}

	lChild->setParent(parent);
	if(parent == NULL) {
		this->root_ = lChild;
	}
	else if (parent->getLeft() == node) {
		parent->setLeft(lChild);
	}
	else {
		parent->setRight(lChild);
	}

	lChild->setRight(node);
	node->setParent(lChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertHelper(AVLNode<Key,Value>* node)
{
	AVLNode<Key,Value>* parent = node->getParent();
	if (parent == NULL) {
		return;
	}

	if (node == parent->getLeft()) {
		parent->updateBalance(-1);
	}
	else {
		parent->updateBalance(1);
	}

	if(parent->getBalance() == 0) {
		return;
	}
	else if (parent->getBalance() == -1 || parent->getBalance() == 1) {
		insertHelper(parent);
		return;
	}
	else {
		
		if(parent->getBalance() == -2) {

			AVLNode<Key, Value>* leftChild = parent->getLeft();
			if(leftChild->getBalance() == 1) {
				
				AVLNode<Key, Value>* grandChild = leftChild->getRight();

				int8_t grandBalance;
				if (grandChild != NULL) {
					grandBalance = grandChild->getBalance();
				}
				else {
					grandBalance = 0;
				}

				rotateLeft(leftChild);
				rotateRight(parent);
				
				if (grandBalance == -1) {
					parent->setBalance(1);
				}
				else {
					parent->setBalance(0);
				}
				
				if (grandBalance == 1) {
					leftChild->setBalance(-1);
				}
				else {
					leftChild->setBalance(0);
				}

				if(grandChild != NULL) grandChild->setBalance(0);
			}
			else {
				
				rotateRight(parent);
				parent->setBalance(0);
				leftChild->setBalance(0);
			}
		}
		else {
			
			AVLNode<Key, Value>* rightChild = parent->getRight();
			if(rightChild->getBalance() == -1) {
				
				AVLNode<Key, Value>* grandChild = rightChild->getLeft();

				int8_t grandBalance;
				if (grandChild != NULL) {
					grandBalance = grandChild->getBalance();
				} 
				else {
					grandBalance = 0;
				}

				rotateRight(rightChild);
				rotateLeft(parent);

				if (grandBalance == 1) {
					parent->setBalance(-1);
				}
				else {
					parent->setBalance(0);
				}

				if (grandBalance == -1) {
					rightChild->setBalance(1);
				}
				else {
					rightChild->setBalance(0);
				}

				if(grandChild != NULL) grandChild->setBalance(0);
			}
			else {
				
				rotateLeft(parent);
				parent->setBalance(0);
				rightChild->setBalance(0);
			}
		}
		return;
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* node, int8_t diff)
{
	node->updateBalance(diff);

	if (node->getBalance() == 1 || node->getBalance() == -1) {
		return;
	}

	if(node->getBalance() == 0) {
		AVLNode<Key, Value>* parent = node->getParent();
		if(parent == NULL) {
			return;
		}

		int nextDiff = 0;
		if (node == parent->getLeft()) {
			nextDiff = 1;
		}
		else {
			nextDiff = -1;
		}

		removeHelper(parent, nextDiff);
		return;
	}
	
	if (node->getBalance() == -2) {
		
		AVLNode<Key, Value>* leftChild = node->getLeft();
		int8_t leftBalance;
		if (leftChild != NULL) {
			leftBalance = leftChild->getBalance();
		}
		else {
			leftBalance = 0;
		}
		
		if(leftBalance <= 0) {

			rotateRight(node);
			
			if (leftBalance == 0) {
				node->setBalance(-1);
			}
			else {
				node->setBalance(0);
			}

			if (leftBalance == 0) {
				leftChild->setBalance(1);
			}
			else {
				leftChild->setBalance(0);
			}

			//stop
			if(leftBalance == 0) return; 
		}
		else {

			AVLNode<Key, Value>* grandChild = leftChild->getRight();
			int8_t grandBalance;
			if (grandChild != NULL) {
				grandBalance = grandChild->getBalance();
			}
			else {
				grandBalance = 0;
			}

			rotateLeft(leftChild);
			rotateRight(node);

			if (grandBalance == -1) {
				node->setBalance(1);
			}
			else {
				node->setBalance(0);
			}

			if (grandBalance == 1) {
				leftChild->setBalance(-1);
			}
			else {
				leftChild->setBalance(0);
			}

			if(grandChild != NULL) grandChild->setBalance(0);
		}
	}
	else {
		AVLNode<Key, Value>* rightChild = node->getRight();
		int8_t rightBalance;
		if (rightChild != NULL) {
			rightBalance = rightChild->getBalance();
		}
		else {
			rightBalance = 0;
		}
		
		if(rightBalance >= 0) {

			rotateLeft(node);
			
			if (rightBalance == 0) {
				node->setBalance(1);
			}
			else {
				node->setBalance(0);
			}

			if (rightBalance == 0) {
				rightChild->setBalance(-1);
			}
			else {
				rightChild->setBalance(0);
			}

			if(rightBalance == 0) return; 
		}
		else {

			AVLNode<Key, Value>* grandChild = rightChild->getLeft();
			int8_t grandBalance;
			if (grandChild != NULL) {
				grandBalance = grandChild->getBalance();
			}
			else {
				grandBalance = 0;
			}

 			rotateRight(rightChild);
			rotateLeft(node);
			
			if (grandBalance == 1) {
				node->setBalance(-1);
			}
			else {
				node->setBalance(0);
			}

			if (grandBalance == -1) {
				rightChild->setBalance(1);
			}
			else {
				rightChild->setBalance(0);
			}

			if(grandChild != NULL) grandChild->setBalance(0);
		}
	}

	AVLNode<Key, Value>* newRoot = node->getParent();
	if(newRoot != NULL) {
		
		AVLNode<Key, Value>* newParent = newRoot->getParent();
		if(newParent != NULL) {
			int nextDiff;

			if (newRoot == newParent->getLeft()) {
				nextDiff = 1;
			}
			else {
				nextDiff = -1;
			}

			removeHelper(newParent, nextDiff);
		}
	}
}

#endif
