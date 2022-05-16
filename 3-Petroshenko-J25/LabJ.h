#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define FIRST_SIZE 987453

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
bool Find(HashTable* table, unsigned int key);
void TableDestroy(HashTable* table, int index);
HashTable* Add(HashTable* table, unsigned int key, char* str);
HashTable* Remove(HashTable* table, unsigned int key);
int Solution(FILE* streamIn, FILE* streamOut);