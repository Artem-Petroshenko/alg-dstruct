#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#pragma warning(disable : 4996)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tree_t {
	char* String;
	int SubTree;
	struct tree_t* Left;
	struct tree_t* Right;
} tree_t;

tree_t* TreeInit(void);
void TreeDestroy(tree_t* Root);
int FillWidth(tree_t* Root);
void TreePrint(tree_t* Tree, int n);

#ifdef __cplusplus
}
#endif