#include "LabE.h"

int IntLen(int Number) {
	int i = 0;
	while (Number) {
		i++;
		Number /= 10;
	}
	return i;
}

tree_t* TreeInit(void) {
	tree_t* Tree = (tree_t*)malloc(sizeof(tree_t));
	if (!Tree)
		return NULL;
	Tree->Left = NULL;
	Tree->Right = NULL;
	return Tree;
}

void TreeDestroy(tree_t* Root) {
	if (!Root)
		return;
	else {
		TreeDestroy(Root->Left);
		TreeDestroy(Root->Right);
	}
	free(Root);
	return;
}

int FillWidth(tree_t* Root) {
	int Own = 0;
	if (!Root)
		return Own;
	else {
		int Left = FillWidth(Root->Left);
		Own += Left;
		int Right = FillWidth(Root->Right);
		Own += Right;
		Own += strlen(Root->String);
		Root->SubTree = Own;
	}
	return Own;
}

void TreePrint(tree_t* Tree, int n) {
	if (Tree)
	{
		TreePrint(Tree->Right, n + 2);
		for (int i = 0; i < n; i++) putchar(' ');
		puts(Tree->String);
		for (int i = 0; i < n; i++) putchar(' ');
		printf("%i\n", Tree->SubTree);
		TreePrint(Tree->Left, n + 2);
	}
}