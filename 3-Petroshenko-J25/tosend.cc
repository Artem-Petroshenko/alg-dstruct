//File for testing system

#define _CRT_SECURE_NO_WARNINGS

#define FIRST_SIZE 4567890

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

char* MyStrcpy(const char* str) {
	char* newStr = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	if (!newStr)
		return NULL;
	strcpy(newStr, str);
	return newStr;
}

HashTable* InitTable(unsigned int size) {
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	if (!table)
		return NULL;
	table->size = size;
	table->nodes = (Node*)malloc(sizeof(Node) * table->size);
	if (!table->nodes) {
		free(table);
		return NULL;
	}
	for (int i = 0; i < table->size; i++) {
		table->nodes[i].str = NULL;
		table->nodes[i].isFilled = false;
	}
	return table;
}

unsigned int Str2i(char* str) {
	unsigned int key = 0;
	int i = 0;
	while (str[i]) {
		key = key * 10 + (str[i] - '0');
		i++;
	}
	return key;
}

bool Find(HashTable* table, char* str) {
	if (!table)
		return false;
	unsigned int key = Str2i(str);
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	for (int i = 0; i < table->size; i++) {
		int index = (x + i * y) % table->size;
		if (table->nodes[index].isFilled) {
			if (table->nodes[index].key == key && !strcmp(table->nodes[index].str, str))
				return true;
		}
		else
			return false;
	}
	return false;
}

void TableDestroy(HashTable* table) {
	for (int i = 0; i < table->size; i++)
		if (table->nodes[i].str) {
			free(table->nodes[i].str);
			table->nodes[i].str = NULL;
		}
	free(table->nodes);
	free(table);
	return;
}

HashTable* Add(HashTable* table, char* str) {
	unsigned int key = Str2i(str);
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	int index;
	if (Find(table, str))
		return table;
	for (int i = 0; i < table->size; i++) {
		index = (x + i * y) % table->size;
		if (!table->nodes[index].isFilled) {
			table->nodes[index].key = key;
			table->nodes[index].str = MyStrcpy(str);
			if (!table->nodes[index].str)
				return NULL;
			table->nodes[index].isFilled = true;
			return table;
		}
	}
	return table;
}

HashTable* Remove(HashTable* table, char* str) {
	unsigned int key = Str2i(str);
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	if (!Find(table, str))
		return table;
	for (int i = 0; i < table->size; i++) {
		int index = (x + i * y) % table->size;
		if (table->nodes[index].isFilled) {
			if (table->nodes[index].key == key && !strcmp(table->nodes[index].str, str)) {
				free(table->nodes[index].str);
				table->nodes[index].str = NULL;
				table->nodes[index].isFilled = false;
				return table;
			}
		}
	}
}

int Solution(FILE* streamIn, FILE* streamOut) {
	char lineBuffer[16] = " ";
	char action;
	char str[10000] = { 0 };
	HashTable* t = InitTable(FIRST_SIZE);
	if (!t)
		return 0;
	while (fgets(lineBuffer, 16, streamIn)) {
		sscanf(lineBuffer, "%c%s", &action, &str);
		switch (action) {
		case 'a':
			t = Add(t, str);
			break;
		case 'r':
			t = Remove(t, str);
			break;
		case 'f':
			if (Find(t, str))
				fprintf(streamOut, "yes\n");
			else
				fprintf(streamOut, "no\n");
			break;
		default:
			TableDestroy(t);
			return 0;
		}
	}
	TableDestroy(t);
	return 0;
}

int main() {
	Solution(stdin, stdout);
	return 0;
}