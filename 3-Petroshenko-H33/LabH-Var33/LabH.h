#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct treap_t {
	int X;
	int C;
	struct treap_t* Left;
	struct treap_t* Right;
} treap_t;

typedef struct {
	treap_t* T1;
	treap_t* T2;
} treaps_pair_t;

treaps_pair_t Split(treap_t* T, int Key);
treap_t* Merge(treap_t* T1, treap_t* T2);
treap_t* Find(treap_t* T, int Key);
treap_t* Insert(treap_t* T, int Key, int Priority);
treap_t* Remove(treap_t* T, int Key);
void TreapDestroy(treap_t* T);
int MaxPriority(treap_t* T, int Key1, int Key2);
int Solution(FILE* StreamIn, FILE* StreamOut);