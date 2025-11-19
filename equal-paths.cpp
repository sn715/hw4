#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalPathsHelper(Node * root, int current, int &expected);

bool equalPathsHelper(Node * root, int current, int &expected)
{

    //base case - empty subtree
    if (root == NULL) {
        return true;
    }

    //base case - leaf nod
    if (root->left == NULL && root->right == NULL) {
        if (expected == -1) {
            expected = current;
            return true;
        }
        else if (current == expected) {
            return true;
        }
        else {
            return false;
        }
    }

    //recursive case 
    bool left = true;
    bool right = true;

    if (root->left != NULL) {
        left = equalPathsHelper(root->left, current + 1, expected);
        if (!left) {
            return false;
        }
    }
    if (root->right != NULL) {
        right = equalPathsHelper(root->right, current + 1, expected);
        if (!right) {
            return false;
        }
    }
    return left && right;
}

bool equalPaths(Node * root)
{
		//base case
    if (root == NULL) {
        return true;
    }

    //recursive case
    int expected = -1; // not set yet
    int current = 0;
    return equalPathsHelper(root, current, expected);
}

