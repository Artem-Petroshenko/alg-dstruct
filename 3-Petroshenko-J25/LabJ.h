#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define FIRST_SIZE 3

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	unsigned int key;
	char* str;
	bool isFilled;
} Node;

typedef struct {
	unsigned int size;
	Node* nodes;
} HashTable;

HashTable* InitTable(unsigned int size);
bool Find(HashTable* table, char* str);
void TableDestroy(HashTable* table);
HashTable* Add(HashTable* table, char* str);
HashTable* Remove(HashTable* table, char* str);
int Solution(FILE* streamIn, FILE* streamOut);