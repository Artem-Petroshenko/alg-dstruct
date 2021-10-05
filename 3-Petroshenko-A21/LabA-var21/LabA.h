#pragma once
#ifndef _MY_H
#define _MY_H

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#ifdef __cplusplus
extern "C"
{
#endif
 
#define N 50
#define Error -1
#define Add_to_begining 1
#define Add_not_to_begining 2

typedef struct {
	char* Word;
	int Key;
}data_t;

typedef struct Node_t {
	data_t data;
	struct Node_t* next;
}Node_t;

typedef struct {
	Node_t* head;
}List_t;

List_t* ListCreate(char* Word, int Key);
Node_t* ElementCreate(char* Word, int Key);
void ListDestroy(List_t* List);
void ElementDestroy(Node_t* element);
List_t* Fill(const char* filename);
Node_t* Search_place(List_t* List, int DataKey);
int Add(List_t* List, Node_t* new_element);
int Search_Word(List_t* List, int DataKey);
void Output(List_t* List);
void MemoryLeaks(void);

#ifdef __cplusplus
}
#endif

#endif