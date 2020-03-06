//
//  main.cpp
//  BST
//
//  Created by Dhaval Rao on 2/25/20.
//  Copyright © 2020 Dhaval Rao. All rights reserved.
//

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct node {
    int key;
    struct node *left;
    struct node *right;
};

struct node *newNode(int key){
    struct node *node = (struct node*)malloc(sizeof(struct node));
    
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printTree(struct node *node){
    if(node == NULL)
        return;
    else{
        printTree(node->left);
        cout << node->key << " ";
        printTree(node->right);
    }
}

struct node *insertRec(struct node *node, int key){ //insert rec
    if (node == NULL)
        return newNode(key);
    else{
        if(key < node->key)
            node->left = insertRec(node->left, key);
        else
            node->right = insertRec(node->right, key);
        return node;
    }
}

struct node *deleteRec(struct node *node, int key){ // delete rec
    if(node == NULL)
        return node;
    
    if(key < node->key)
        node->left = deleteRec(node->left, key);
    
    else if(key > node->key)
        node->right = deleteRec(node->right, key);
    
    else{
        if(node->left == NULL){
            struct node *temp = node->right;
            free(node);
            return temp;
        }
        else if(node->right == NULL){
            struct node *temp = node->left;
            free(node);
            return temp;
        }
        struct node *leftMost = node->right;
        while(leftMost->left != NULL)
            leftMost = leftMost->left;
        
        struct node *temp = leftMost;
        node->key = temp->key;
        node->right = deleteRec(node->right, temp->key);
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


struct node *insertIter(struct node *node, int key){ // insert iter
    
    struct node *newnode = newNode(key);
    struct node *current = node;
    struct node *parent = NULL;
    
    while (current != NULL){
        parent = current;
        if(key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    
    if(parent == NULL)
        parent = newnode;
    
    else if(key < parent->key)
        parent->left = newnode;
    
    else
        parent->right = newnode;
    
    return parent;
    
}

struct node* deleteIter(struct node* root, int key) {
    // find node to delete (node with key equal to the input parameter `key`
    struct node* currentNode = root;
    struct node* parentNode = NULL;
    while (currentNode) {
        if (currentNode->key == key) {
            break;
        }
        if (key > currentNode->key && currentNode->right != NULL) {
            parentNode = currentNode;
            currentNode = currentNode->right;
        }
        else if (key < currentNode->key && currentNode->left != NULL){
            parentNode = currentNode;
            currentNode = currentNode->left;
        }
        else {
            return root;
        }
    }
    while (true) {
        if (currentNode->left == NULL && currentNode->right == NULL) { // no child
            if (parentNode == NULL) {
                return NULL;
            }
            else if (currentNode == parentNode->left) {
                free(parentNode->left);
                parentNode->left = NULL;
                return root;
            }
            else {
                free(parentNode->right);
                parentNode->right = NULL;
                return root;
            }
        }
        
        else if (currentNode->left == NULL) { // only right child
            if (parentNode == NULL) {
                root = currentNode->right;
                free(currentNode);
                return root;
            }
            else {
                if (currentNode == parentNode->left) {
                    parentNode->left = currentNode->right;
                    free(currentNode);
                    return root;
                }
                else {
                    parentNode->right = currentNode->right;
                    free(currentNode);
                    return root;
                }
            }
        }
        
        else if (currentNode->right == NULL) { // only left child
            if (parentNode == NULL) {
                root = currentNode->left;
                free(currentNode);
                return root;
            }
            else {
                if (currentNode == parentNode->left) {
                    parentNode->left = currentNode->left;
                    free(currentNode);
                    return root;
                }
                else {
                    parentNode->right = currentNode->left;
                    free(currentNode);
                    return root;
                }
            }
        }
        // we have both children to deal with both children
        else {
            //pointing to right subtree of the current node
            struct node* inOrderSuccessor = currentNode->right;
            parentNode = currentNode;
            while(inOrderSuccessor->left) { // left most successor
                parentNode = inOrderSuccessor;
                inOrderSuccessor = inOrderSuccessor->left;
            }
            currentNode->key = inOrderSuccessor->key;
            currentNode = inOrderSuccessor;
        }
    }
    return root;
}

struct node *findNextIter(struct node *node, int key){ // find next iter
    struct node *temp = node;
    struct node *extra = NULL;
    
    while (temp && temp->key != key){
        if (key < temp->key)
            temp = temp->left;
        else if (key > temp->key)
            temp = temp->right;
    }
    
    if (temp->right){
        extra = temp->right;
            
        while (extra->left)
            extra = extra->left;
        return extra;
    }
    
    while (node){
        if (temp->key < node->key){
            extra = node;
            node = node->left;
        }
        else if (temp->key > node->key)
            node = node->right;
        break;
        }
    return extra;
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

int findMinIter(struct node *node){
    struct node *temp = node;
    while (temp->left != NULL)
        temp = temp->left;
    
    return (temp->key);
}

int findMaxIter(struct node *node){
    struct node *temp = node;
    while (temp->right != NULL)
        temp = temp->right;
    
    return temp->key;
}

/*int main() {
    struct node *root = NULL;
    root = insertRec(root, 100);
    insertRec(root, 300);
    insertRec(root, 250);
    insertRec(root, 400);
    insertRec(root, 350);
    insertRec(root, 200);
    insertRec(root, 150);
    
    cout << "BST: ";
    printTree(root);
    cout << " " << "\n";
    
    deleteRec(root, 200);
    cout<< "Rec ";
    printTree(root);
    cout << " " << "\n";
    
    deleteIter(root, 300);
    cout << " " << "\n";
    
    
    cout << "Next Rec: " << findNextRec(root, 300)->key << "\n";
    cout << "Next Iter: "  << findNextIter(root, 300)->key << "\n";
    
    cout << "Prev Rec: "  << findPrevRec(root, 400)->key << "\n";
    cout << "Prev Iter: "  << findPrevIter(root, 400)->key << "\n";
     
    cout << "Min Rec " << findMinRec(root) << "\n";
    cout << "Min Iter " << findMinIter(root) << "\n";
     
    cout << "Max Rec " << findMaxRec(root) << "\n";
    cout << "Max Iter " << findMaxIter(root) << "\n";
    printTree(root);
    return 0;
}*/
