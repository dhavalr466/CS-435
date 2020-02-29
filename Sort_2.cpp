//
//  main.cpp
//  Sort
//  Problem 2
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

struct node *insert(struct node *node, int key){
    if (node == NULL)
        return newNode(key);
    else{
        if(key < node->key)
            node->left = insert(node->left, key);
        else
            node->right = insert(node->right, key);
        return node;
    }
}

int main() {
    struct node *root = NULL;
    root = insert(root, 20);    //make a list of unsorted elements
    insert(root, 35);
    insert(root, 69);
    insert(root, 46);
    insert(root, 19);
    insert(root, 26);
    insert(root, 99);
    
    cout << "Sorted list: ";
    printTree(root); //output a list of sorted elements
    cout << " " << endl;
    
    return 0;
}
