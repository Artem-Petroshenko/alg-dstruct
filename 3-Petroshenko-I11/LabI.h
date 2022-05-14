#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree_t {
	struct tree_t* parent;
	struct tree_t* left;
	struct tree_t* mid;
	struct tree_t* right;
	int height;
	int key1, key2;
	bool full;
} tree_t;

tree_t* InitNode(tree_t* brother, int key, tree_t* pointer);
tree_t* SearchNode(tree_t* root, int key);
tree_t* SplitParent(tree_t* son, int key);
tree_t* Insert(tree_t* root, int key, tree_t* pointer);
bool FindNode(tree_t* tree, int key);
void TreeDestroy(tree_t* root);
tree_t* Remove(tree_t* root, int key);
void PrintTree(tree_t* t, int offset);
tree_t* SplitTree(tree_t* tree, int key, tree_t** pLeft, tree_t** pRight);
tree_t* MergeTree(tree_t* left, tree_t* right);