#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50

typedef struct {
	char* Word;
	int Key;
}data_t;

typedef struct node_t {
	data_t data;
	struct node_t* next;
}node_t;

typedef struct {
	node_t* head;
}List_t;

List_t* ListCreate(const char* filename);
void ListDestroy(List_t* List);
void Fill(const char* filename, List_t* List);
void Add_and_Sort(List_t* List, node_t* new_element);
void Output(List_t* List);