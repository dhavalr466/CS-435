//
//  main.cpp
//  AVL
//
//  Created by Dhaval Rao on 3/3/20.
//  Copyright © 2020 Dhaval Rao. All rights reserved.
//

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

void printTree(struct node *node){ // pre order traverse
    if(node == NULL)
        return;
    else{
        cout << node->key << " ";
        printTree(node->left);
        
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
    
    base->right = node;
    node->left = base2;
    
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

struct node *insertRec(struct node *node, int key){ // insert rec
    if (node == NULL)
        return newNode(key);
    
    else{
        if (key > node->key)
            node->right = insertRec(node->right, key);
        else
            node->left = insertRec(node->left, key);
        
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

struct node *deleteRec(struct node *node, int key){ // delete rec
    if(node == NULL)
        return node;
    
    if(key > node->key)
        node->right = deleteRec(node->right, key);
    
    else if(key < node->key)
        node->left = deleteRec(node->left, key);
    
    else{
        if(node->left == NULL){ // only left child
            struct node *temp = node->right;
            free(node);
            return temp;
        }
        else if(node->right == NULL){ // only right child
            struct node *temp = node->left;
            free(node);
            return temp;
        }
        
        struct node *leftMost = node->right; //
        while(leftMost->left != NULL) // both children
            leftMost = leftMost->left;
        
        struct node *temp = leftMost;
        node->key = temp->key;
        node->right = deleteRec(node->right, temp->key);
    }
    
    revise(node);// update the height
    int BF = getBF(node); // get the balance factor
    
    if (BF < -1 && key > node->right->key) // right right graph
        return rotateL(node);
    
    if (BF > 1 && key < node->left->key) // left left graph
        return rotateR(node);
    
    if (BF < -1 && key < node->right->key){ // right left graph
        node->right = rotateR(node->right);
        return rotateL(node);
    }
    
    if(BF > 1 && key > node->left->key){ // left right graph
        node->left = rotateL(node->left);
        return rotateR(node);
    }
    
    return node;
}

struct node *findNextRec(struct node *node, int key, struct node *def = NULL){ // find next rec
    struct node *temp = node;
    
    if (node == NULL)
        return NULL;
    
    if (node->key == key){
        struct node *extra = NULL;
        
        if (temp->right){
            extra = temp->right;
            while (extra->left)
                extra = extra->left;
            return extra;
        }
        return def;
    }
    
    else if (key < node->key)
        return findNextRec(node->left, key, def = node);
    
    else
        return findNextRec(node->right, key, def = node);
    
}

struct node *findPrevRec(struct node *node, int key, struct node *def = NULL){ // find prev rec
    struct node *temp = node;
    
    if (node == def)
        return NULL;
    
    if (node->key == key){
        struct node *extra = NULL;
        
        if(temp->left){
            extra = temp->left;
            while (extra->right)
                extra = extra->right;
            return extra;
        }
        return def;
    }
    else if (key < node->key)
        return findPrevRec(node->left, key, def = node);
    else
        return findPrevRec(node->right, key, def = node);
}

int findMinRec(struct node *node){ // find min rec
    
    if (node->left == NULL)
        return node->key;
    return findMinRec(node->left);
}

int findMaxRec(struct node *node){ // find max rec
    if (node->right == NULL)
        return node->key;
    return findMaxRec(node->right);
}

struct node *insertIter(struct node* node, int key) { // insert iter
    // null pointer implies that the very first node (root) needs to be created.
    if (node == NULL) {
        return newNode(key);
    }
    int count = 0;
    struct node* current = node;
    stack<struct node*> ancestors;
    while (current != NULL) {
        ancestors.push(current);
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

struct node *deleteIter(struct node* node, int key) { // delete iter
    struct node* current = node;
    struct node* parentNode = NULL;
    stack<struct node*> ancestors;
    while (current) {
        ancestors.push(current);
        if (current->key == key) {
            break;
        }
        if (key > current->key && current->right != NULL) {
            parentNode = current;
            current = current->right;
        }
        else if (key < current->key && current->left != NULL){
            parentNode = current;
            current = current->left;
        }
        else {
            return node;
        }
    }
    while (current != NULL) {
        if (current->left == NULL && current->right == NULL) { // no child
            if (parentNode == NULL) {
                return NULL;
            }
            else if (current == parentNode->left) {
                free(parentNode->left);
                parentNode->left = NULL;
                return node;
            }
            else {
                free(parentNode->right);
                parentNode->right = NULL;
                return node;
            }
        }
        
        else if (current->left == NULL) { // only right child
            if (parentNode == NULL) {
                node = current->right;
                free(current);
                return node;
            }
            else {
                if (current == parentNode->left) {
                    parentNode->left = current->right;
                    free(current);
                    return node;
                }
                else {
                    parentNode->right = current->right;
                    free(current);
                    return node;
                }
            }
        }
        
        else if (current->right == NULL) { // only left child
            if (parentNode == NULL) {
                node = current->left;
                free(current);
                return node;
            }
            else {
                if (current == parentNode->left) {
                    parentNode->left = current->left;
                    free(current);
                    return node;
                }
                else {
                    parentNode->right = current->left;
                    free(current);
                    return node;
                }
            }
        }
        
        else { // both children
            //pointing to right subtree of the current node
            struct node* leftMost = current->right;
            ancestors.push(leftMost);
            parentNode = current;
            while(leftMost->left) { // left most successor
                parentNode = leftMost;
                leftMost = leftMost->left;
                ancestors.push(leftMost);
            }
            current->key = leftMost->key;
            current = leftMost;
        }
    }
    
    while (!ancestors.empty()) {
        struct node* a = ancestors.top();
        ancestors.pop();
        revise(a); // update height
        int balance = getBF(a);
        if (balance > 1){
            int leftBalance = getBF(a->left);
            if(balance < 0){
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
            else if (leftBalance >= 0){
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
            if (rightBalance <= 0){
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
            else if (rightBalance > 0){
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

struct node *find(struct node *node, int key){
    while (node != NULL){
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
        else
            return node;
    }
    return node;
}

struct node *findChild(struct node *node){
    while (node->left != NULL)
        node = node->left;
    return node;
}

struct node *findParent(struct node *node, int key){
    struct node *current = node;
    while (true){
        if ((current->left != NULL && node->left->key <= key) && current->key > key)
            return current;
        if (current->left != NULL && key < current->key)
            current = current->left;
        else if (current->right != NULL && key > current->key)
            current = current->right;
        else
            return NULL;
    }
}

struct node *findNextIter(struct node *node, int key){ // find next iter
    struct node* current = find(node, key);
    if(current == NULL)
        return NULL;
    else if (current->right == NULL)
        return findParent(node, key);
    return findChild(node->right);
}

struct node *findPrevIter(struct node *node, int key){ // find prev iter
    struct node *temp = node;
    struct node *extra = NULL ;
    struct node *prev = NULL;
    
    if (node == NULL)
        return node;
    
    while (temp && temp->key != key){
        if (key < temp->key)
            temp = temp->left;
        else if (key > temp->key){
            prev = temp;
            temp = temp->right;
        }
    }
    
    if (temp && temp->left){
        extra = temp->left;
        while (extra->right)
            extra = extra->right;
        return extra;
    }
    return prev;
    
}

int findMinIter(struct node *node){ // find min iter
    struct node *temp = node;
    while (temp->left != NULL)
        temp = temp->left;
    
    return (temp->key);
}

int findMaxIter(struct node *node){// find max iter
    struct node *temp = node;
    while (temp->right != NULL)
        temp = temp->right;
    
    return temp->key;
}

//int main(){
//    struct node *root = NULL;
//    root = insertRec(root, 10);
//    root = insertRec(root, 30);
//    root = insertRec(root, 25);
//    root = insertRec(root, 40);
//    root = insertRec(root, 35);
//    root = insertRec(root, 20);
//    root = insertRec(root, 15);
//
//    root = insertIter(root, 10);
//    root = insertIter(root, 30);
//    root = insertIter(root, 25);
//    root = insertIter(root, 40);
//    root = insertIter(root, 35);
//    root = insertIter(root, 20);
//    root = insertIter(root, 15);
//
//    cout << "AVL: ";
//    printTree(root);
//    cout << " " << "\n";
//
//    deleteRec(root, 10);
//    cout << "Delete Rec: ";
//    printTree(root);
//    cout << " " << "\n";
//
//    deleteIter(root, 25);
//    cout << "Delete Iter: ";
//    printTree(root);
//    cout << " " << "\n";
//
//    cout << "Next Rec: " << findNextRec(root, 30)->key << "\n";
//    cout << "Next Iter: "  << findNextIter(root, 30)->key << "\n";
//
//    cout << "Prev Rec: "  << findPrevRec(root, 20)->key << "\n";
//    cout << "Prev Iter: "  << findPrevIter(root, 20)->key << "\n";
//
//    cout << "Min Rec " << findMinRec(root) << "\n";
//    cout << "Min Iter " << findMinIter(root) << "\n";
//
//    cout << "Max Rec " << findMaxRec(root) << "\n";
//    cout << "Max Iter " << findMaxIter(root) << "\n";
//
//    return 0;
//}
