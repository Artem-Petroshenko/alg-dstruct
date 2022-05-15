#include "LabJ.h"

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

bool Find(HashTable* table, unsigned int key) {
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	for (int i = 0; i < table->size; i++) {
		int index = (x + i * y) % table->size;
		if (table->nodes[index].isFilled) {
			if (table->nodes[index].key == key)
				return true;
		}
		else
			return false;
	}
	return false;
}

void TableDestroy(HashTable* table, int index) {
	for (int i = 0; i < index; i++) 
		if (table->nodes[i].str) {
			free(table->nodes[i].str);
			table->nodes[i].str = NULL;
		}
	free(table->nodes);
	free(table);
	return;
}

HashTable* Resize(HashTable* table) {
	HashTable* newTable = InitTable(table->size * 2);
	if (!newTable)
		return NULL;
	int key = 0;
	int x, y;
	for (int i = 0; i < table->size; i++) {
		if (table->nodes[i].isFilled) {
			key = table->nodes[i].key;
			x = key % newTable->size;
			y = 1 + key % (newTable->size - 1);
			for (int j = 0; j < newTable->size; j++) {
				int index = (x + j * y) % newTable->size;
				if (!newTable->nodes[index].isFilled) {
					newTable->nodes[index].key = key;
					newTable->nodes[index].str = MyStrcpy(table->nodes[i].str);
					if (!newTable->nodes[index].str) {
						TableDestroy(table, index);
						return NULL;
					}
					newTable->nodes[index].isFilled = true;
					break;
				}
			}
		}
	}
	return newTable;
}

HashTable* Add(HashTable* table, unsigned int key, char* str) {
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	if (Find(table, key))
		return table;
	for (int i = 0; i < table->size; i++) {
		int index = (x + i * y) % table->size;
		if (!table->nodes[index].isFilled) {
			table->nodes[index].key = key;
			table->nodes[index].str = MyStrcpy(str);
			if (!table->nodes[index].str)
				return NULL;
			table->nodes[index].isFilled = true;
			return table;
		}
	}
	table = Resize(table);
	table = Add(table, key, str);
	return table;
}

HashTable* Remove(HashTable* table, unsigned int key) {
	int x = key % table->size;
	int y = 1 + key % (table->size - 1);
	if (!Find(table, key))
		return table;
	for (int i = 0; i < table->size; i++) {
		int index = (x + i * y) % table->size;
		if (table->nodes[index].isFilled) {
			if (table->nodes[index].key == key) {
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
	unsigned int number;
	char someStr[10] = "asfbjfs";
	HashTable* t = InitTable(FIRST_SIZE);
	while (fgets(lineBuffer, 16, streamIn)) {
		sscanf(lineBuffer, "%c%d", &action, &number);
		switch (action) {
		case 'a':
			t = Add(t, number, someStr);
			break;
		case 'r':
			t = Remove(t, number);
			break;
		case 'f':
			if (Find(t, number))
				fprintf(streamOut, "yes\n");
			else
				fprintf(streamOut, "no\n");
			break;
		default:
			TableDestroy(t, t->size);
			return 0;
		}
	}
	TableDestroy(t, t->size);
	return 0;
}

int main() {
	Solution(stdin, stdout);
	return 0;
}