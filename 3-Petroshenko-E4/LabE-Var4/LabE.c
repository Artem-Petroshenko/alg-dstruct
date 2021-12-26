#include "LabE.h"

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

void TreePrint(tree_t* Tree, int n, FILE* Stream) {
	if (Tree)
	{
		TreePrint(Tree->Right, n + 2, Stream);
		for (int i = 0; i < n; i++) 
			fprintf(Stream, " ");
		fputs(Tree->String, Stream);
		fprintf(Stream, "\n");
		for (int i = 0; i < n; i++) 
			fprintf(Stream, " ");
		fprintf(Stream, "%i\n", Tree->SubTree);
		TreePrint(Tree->Left, n + 2, Stream);
	}
}

//Possibly needed function
int CompareFilesTest(tree_t* Root, char* ExpectedFile, char* ResultFile) {
	char ExpectedSymbol, ResultSymbol;
	FILE* Result = fopen(ResultFile, "w");
	if (!Result)
		return -1;
	TreePrint(Root, 0, Result);
	fclose(Result);
	FILE* Expect = fopen(ExpectedFile, "r");
	if (!Expect)
		return -1;
	Result = fopen(ResultFile, "r");
	if (!Result) {
		fclose(Expect);
		return -1;
	}
	while (fscanf(Expect, "%c", &ExpectedSymbol) != EOF && fscanf(Result, "%c", &ResultSymbol) != EOF) {
		if (ExpectedSymbol != ResultSymbol) {
			fclose(Expect);
			fclose(Result);
			return 0;
		}
	}
	fclose(Expect);
	fclose(Result);
	return 1;
}