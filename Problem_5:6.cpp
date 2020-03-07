//
//  main.cpp
//  Random
//
//  Created by Dhaval Rao on 3/6/20.
//  Copyright © 2020 Dhaval Rao. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
//using namespace std;

struct node {
    int key;
    int height;
    struct node *left;
    struct node *right;
};

struct node *newNode(int key){
    struct node *node = (struct node*)malloc(sizeof(struct node));
    
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

void printTree(struct node *node){ // in order traverse
    if(node == NULL)
        return;
    else{
        printTree(node->left);
        std::cout << node->key << " ";
        printTree(node->right);
    }
}

int getHeight(struct node *node){
    if(node == NULL)
        return 0;
    
    return node->height;
    
}

int getMax(int x, int y){
    return (x>y) ? x:y;
}

void revise(struct node *node){ // updating height
    node->height = 1 + getMax(getHeight(node->right), getHeight(node->left));
}

int getBF(struct node *node){ //getting the balance factor
    int lHeight, rHeight;
    
    lHeight = getHeight(node->left);
    rHeight = getHeight(node->right);
    
    return lHeight - rHeight;
}

struct node *rotateL(struct node *node){ // left rotation
    
    struct node *base = node->right;
    struct node *base2 = node->left;
    
    base->left = node;
    node->right = base2;
    
    revise(node); // update the height
    revise(base); //update the height
    
    return base;
}

struct node *rotateR(struct node *node){ // right rotation
    
    struct node *base = node->left;
    struct node *base2 = node->right;
    
    node->left = base2;
    base->right = node;
    
    revise(node); // update the height
    revise(base); // update the height
    
    return base;
}

struct node *rotateLR(struct node *node){ // left right rotation
    node->left = rotateL(node->left);
    return rotateR(node);
}

struct node *rotateRL(struct node *node){ // right left rotation
    node->right = rotateR(node->right);
    return rotateL(node);
}

int getRandom(int n){
    int x;
    x = rand() % n + 1;
    return x;
}

int *getRandomArray(int n){
    
    int *numArr = (int *)malloc(sizeof(int) * n);
    
    for (int i=0; i<n; i++){
        int randNum = getRandom(n);
        for(int j=0; j<i; j++){
            if (j == randNum){
                randNum = getRandom(n);
            }
        }
        numArr[i] = randNum;
    }
    
    return numArr;
}

int *getSortedArray(int n){
    int *numArr = new int[n];
    for (int i=n; i>0; i--){
        numArr[i] = i;
    }
    return numArr;
    
}

struct node *insertRecBST(struct node *node, int key){ //insert rec
    if (node == NULL)
        return newNode(key);
    else{
        if(key < node->key)
            node->left = insertRecBST(node->left, key);
        else
            node->right = insertRecBST(node->right, key);
        return node;
    }
}

struct node *insertRecAVL(struct node *node, int key){ // insert rec
    if (node == NULL)
        return newNode(key);
    
    else{
        if (key > node->key)
            node->right = insertRecAVL(node->right, key);
        else
            node->left = insertRecAVL(node->left, key);
        
        revise(node);// update the height
        int BF = getBF(node); // get the balance factor
        
        if (BF < -1 && key > node->right->key) // right right graph
            return rotateL(node);
        
        if (BF > 1 && key < node->left->key) // left left graph
            return rotateR(node);
        
        if (BF < -1 && key < node->right->key){ // right left graph
            return rotateRL(node);
        }
        
        if(BF > 1 && key > node->left->key){ // left right graph
            return rotateLR(node);
        }
        
        return node;
    }
}

int count = 0; // for 6-a

struct node *insertIterBST(struct node* node, int key) { // insert iter
    // null pointer implies that the very first node (root) needs to be created.
    if (node == NULL) {
        return newNode(key);
    }
    //int count = 0;
    struct node* current = node;
    while (current != NULL) {
        if (key < current->key) {
            if (current->left == NULL) {
                current->left = newNode(key);
                break;
            }
            else {
                current = current->left;
                count += 1;
            }
        }
        else if (key > current->key) {
            if (current->right == NULL) {
                current->right = newNode(key);
                break;
            }
            else {
                current = current->right;
                count += 1;
            }
        }
    }
    return node;
}

int countA = 0; //for 6-a

struct node *insertIterAVL(struct node* node, int key) { // insert iter
    // null pointer implies that the very first node (root) needs to be created.
    if (node == NULL) {
        return newNode(key);
    }
    //int countA = 0;
    struct node* current = node;
    std::stack<struct node*> ancestors;
    while (current != NULL) {
        ancestors.push(current);
        if (key < current->key) {
            if (current->left == NULL) {
                current->left = newNode(key);
                break;
            }
            else {
                current = current->left;
                countA += 1;
            }
        }
        else if (key > current->key) {
            if (current->right == NULL) {
                current->right = newNode(key);
                break;
            }
            else {
                current = current->right;
                countA += 1;
            }
        }
    }
    while (!ancestors.empty()) {
        struct node* a = ancestors.top();
        ancestors.pop();
        revise(a); // update height
        int balance = getBF(a);
        if (balance > 1){
            int leftBalance = getBF(a->left);
            if(balance == -1){
                if(ancestors.size() == 0)
                    node = rotateLR(a);
                else{
                    struct node* b = ancestors.top();
                    ancestors.pop();
                    if(b->left == a)
                        b->left = rotateLR(a);
                    else
                        b->right = rotateLR(a);
                }
            }
            else if (leftBalance == 1){
                if (ancestors.size() == 0)
                    node = rotateR(a);
                else{
                    struct node* b = ancestors.top();
                    ancestors.pop();
                    if (b->left == a)
                        b->left = rotateR(a);
                    else
                        b->right = rotateR(a);
                }
            }
        }
        else if (balance < -1){
            int rightBalance = getBF(a->right);
            if (rightBalance == -1){
                if(ancestors.size() == 0)
                    node = rotateL(a);
                else{
                    struct node* b = ancestors.top();
                    ancestors.pop();
                    if(b->left == a)
                        b->left = rotateL(a);
                    else
                        b->right = rotateR(a);
                }
            }
            else if (rightBalance == 1){
                if (ancestors.size() == 0)
                    node = rotateRL(a);
                else{
                    struct node* b = ancestors.top();
                    ancestors.pop();
                    if(b->left == a)
                        b->left = rotateRL(a);
                    else
                        b->right = rotateRL(a);
                }
            }
        }
    }
    return node;
}

int main(){

    //Problem 5-a
//    int *arr = getRandomArray(10000);
//    struct node *root1 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root1 = insertRecBST(root1, arr[i]);
//    }
//    cout << "BST: ";
//    printTree(root1);
//    cout << endl;
//
//    struct node *root2 = newNode(arr[0]);
//    for (int i=1; i<sizeof(arr); i++){
//        root2 = insertRecAVL(root2, arr[i]);
//    }
//    cout << "AVL: ";
//    printTree(root2);
//    cout << endl;
    
    //Problem 5-b
//    int *arr = getRandomArray(10);
//    struct node *root1 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root1 = insertRecBST(root1, arr[i]);
//    }
//    cout << "BST: ";
//    printTree(root1);
//    cout << endl;
//
//    struct node *root2 = newNode(arr[0]);
//    for (int i=1; i<sizeof(arr); i++){
//        root2 = insertRecAVL(root2, arr[i]);
//    }
//    cout << "AVL: ";
//    printTree(root2);
//    cout << endl;
//
    //Problem 5-c/6-b
//    int *arr = getRandomArray(10000);
//    struct node *root1 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root1 = insertIterBST(root1, arr[i]);
//    }
//    std::cout << "BST: ";
//    printTree(root1);
//    std::cout << std::endl;
//    std::cout << "BST Count: " << count << std::endl;
//
//    struct node *root2 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root1 = insertIterAVL(root2, arr[i]);
//    }
//    std::cout << "AVL: ";
//    printTree(root2);
//    std::cout << std::endl;
//    std::cout << "AVL Count: " << countA << std::endl;
    
//    Problem 6-c
//    int *arr = getSortedArray(10000);
//    struct node *root1 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root1 = insertIterBST(root1, arr[i]);
//    }
//
//    std::cout << "BST: ";
//    printTree(root1);
//    std::cout << std::endl;
//    std::cout << "BST Count: " << count << std::endl;
//
//    struct node *root2 = newNode(arr[0]);
//    for(int i=1; i<sizeof(arr); i++){
//        root2 = insertIterAVL(root2, arr[i]);
//    }
//
//    std::cout << "AVL: ";
//    printTree(root2);
//    std::cout << std::endl;
//    std::cout << "AVL Count: " << countA << std::endl;
    
    return 0;
}
