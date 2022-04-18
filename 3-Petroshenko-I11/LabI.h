#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct tree_t {
	struct tree_t* parent;
	struct tree_t* left;
	struct tree_t* mid;
	struct tree_t* right;
	struct tree_t* extraChild;
	int key1, key2;
	int length;
} tree_t;

tree_t* InitNode(int k);
tree_t* SearchNode(tree_t* root, int x);
int MaxKey(tree_t* t);
void Swap(tree_t** t1, tree_t** t2);
tree_t* SortSons(tree_t* t);
tree_t* UpdateKeys(tree_t* t);
tree_t* SplitParent(tree_t* t);
tree_t* Insert(tree_t* root, int x);
void TreeDestroy(tree_t* root);
tree_t* Remove(tree_t* root, int x);
void PrintTree(tree_t* t, int n);