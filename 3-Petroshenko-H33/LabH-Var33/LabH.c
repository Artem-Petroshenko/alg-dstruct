#include "LabH.h"

treaps_pair_t Split(treap_t* T, int Key) {
	if (!T)
		return (treaps_pair_t) { NULL, NULL };
	if (Key > T->X) {
		treaps_pair_t Splited = Split(T->Right, Key);
		T->Right = Splited.T1;
		return (treaps_pair_t) { T, Splited.T2 };
	}
	else {
		treaps_pair_t Splited = Split(T->Left, Key);
		T->Left = Splited.T2;
		return (treaps_pair_t) { Splited.T1, T };
	}
}

treap_t* Merge(treap_t* T1, treap_t* T2) {
	if (!T2)
		return T1;
	if (!T1)
		return T2;
	if (T1->C > T2->C) {
		T1->Right = Merge(T1->Right, T2);
		return T1;
	}
	else {
		T2->Left = Merge(T1, T2->Left);
		return T2;
	}
}

treap_t* Find(treap_t* T, int Key) {
	if (!T)
		return NULL;
	if (T->X == Key)
		return T;
	if (T->X > Key)
		return Find(T->Left, Key);
	else 
		return Find(T->Right, Key);
}

treap_t* Insert(treap_t* T, int Key, int Priority) {
	if (Find(T, Key))
		return T;
	treap_t* NewElement = (treap_t*)malloc(sizeof(treap_t));
	if (!NewElement)
		return T;
	NewElement->Left = NULL;
	NewElement->Right = NULL;
	NewElement->X = Key;
	NewElement->C = Priority;

	treaps_pair_t Splited = Split(T, Key);
	Splited.T1 = Merge(Splited.T1, NewElement);
	return Merge(Splited.T1, Splited.T2);
}

treap_t* Remove(treap_t* T, int Key) {
	if (!Find(T, Key))
		return T;
	treaps_pair_t Splited = Split(T, Key);
	treaps_pair_t T2Splited = Split(Splited.T2, Key + 1);
	free(T2Splited.T1);
	return Merge(Splited.T1, T2Splited.T2);
}

void TreapDestroy(treap_t* T) {
	if (T) {
		TreapDestroy(T->Left);
		TreapDestroy(T->Right);
		free(T);
	}
}

int MaxPriority(treap_t* T, int Key1, int Key2) {
	if (!T)
		return 0;
	if (T->X >= Key1 && T->X < Key2)
		return T->C;
	if (T->X >= Key2)
		return MaxPriority(T->Left, Key1, Key2);
	if (T->X < Key1)
		return MaxPriority(T->Right, Key1, Key2);
}

int Solution(FILE* StreamIn, FILE* StreamOut) {
	char LineBuffer[16] = " ";
	char Action;
	int Number;
	treap_t* T = NULL;
	while (fgets(LineBuffer, 16, StreamIn)) {
		sscanf(LineBuffer, "%c%i", &Action, &Number);
		switch (Action) {
		case 'a':
			T = Insert(T, Number, rand());
			break;
		case 'r':
			T = Remove(T, Number);
			break;
		case 'f':
			if (Find(T, Number))
				fprintf(StreamOut, "yes\n");
			else
				fprintf(StreamOut, "no\n");
			break;
		default:
			TreapDestroy(T);
			return 0;
		}
	}
	TreapDestroy(T);
	return 0;
}