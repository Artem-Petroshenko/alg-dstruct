#include "LabA.h"

int main() {
	int DataKey;
	char Word[N];
	int Key;
	const char* filename = "Base.txt";
	FILE* F;
	if (!(F = fopen(filename, "r")))
	{
		printf("File has not been opened!\n");
	}
	if (fscanf(F, "%s %i", Word, &Key) == 0)
	{
		printf("File is empty or data in the file is wrong!\n");
		return 0;
	}
	List_t* List = ListCreate(Word, Key);
	while (fscanf(F, "%s %i", Word, &Key) != EOF)
	{
		Node_t* new_element = ElementCreate(Word, Key);
		if (Add(List, new_element) == Error)
		{
			ElementDestroy(new_element);
		}
	}
	Output(List);
	scanf("%i", &DataKey);
	if (Search_Word(List, DataKey))
		printf("There is such a word\n");
	else
		printf("There is no such a word\n");

	ListDestroy(List);
	fclose(F);
	MemoryLeaks();
	return 0;
}