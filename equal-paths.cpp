#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

void calculateMaxHeight(Node* root, int currentHeight, int& maxHeight)
{
    currentHeight++;
    
    if (root->left == nullptr && root->right == nullptr)
    {
        if (currentHeight > maxHeight) maxHeight = currentHeight;
        return;
    }
    
    
    if (root->left != nullptr) calculateMaxHeight(root->left, currentHeight, maxHeight);
    if (root->right != nullptr) calculateMaxHeight(root->right, currentHeight, maxHeight);
}

void calculateMinHeight(Node* root, int currentHeight, int& minHeight)
{
    currentHeight++;
    
    if (root->left == nullptr && root->right == nullptr)
    {
        if (currentHeight < minHeight) minHeight = currentHeight;
        return;
    }
    
    
    if (root->left != nullptr) calculateMinHeight(root->left, currentHeight, minHeight);
    if (root->right != nullptr) calculateMinHeight(root->right, currentHeight, minHeight);
}

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) return true;
    
    int maxHeight = 0, minHeight = 99999;
    calculateMaxHeight(root, 0, maxHeight);
    calculateMinHeight(root, 0, minHeight);
    
    if (maxHeight == minHeight) return true;
    else return false;
    

}

