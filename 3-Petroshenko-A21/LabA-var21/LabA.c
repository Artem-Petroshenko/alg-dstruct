#include "LabA.h"

List_t* ListCreate(const char* filename) {								//creating the list with the first element
	FILE* F;
	if ((F = fopen(filename, "r")) == NULL)								//checking for right opening the file
		return 0;
	char Word[N];														//we agreed that we have no such long words  
	int Key;

	fscanf(F, "%s %i", Word, &Key);
	Node_t* head = (Node_t*)malloc(sizeof(Node_t));
	if (!head)															//checking for right memory allocation
		return 0;
	head->data.Key = Key;
	head->data.Word = (char*)malloc(sizeof(char) * N);
	strcpy(head->data.Word, Word);
	head->next = NULL;
	List_t* List = (List_t*)malloc(sizeof(List_t));
	if (!List)
		return 0;
	List->head = head;

	fclose(F);
	return List;
}

void ListDestroy(List_t* List) {										//freeing the memory
	Node_t* temp_pointer = List->head;
	while (temp_pointer)
	{
		Node_t* deleted = temp_pointer;
		temp_pointer = temp_pointer->next;
		free(deleted->data.Word);
		free(deleted);
	}
	free(List);
}

void Fill(const char* filename, List_t* List) {							//just reading the elements from file
	char Word[N];														
	int Key;
	FILE* F;
	if ((F = fopen(filename, "r")) == NULL)								//checking for right opening the file
		return 0;

	fscanf(F, "%s %i", Word, &Key);
	while (fscanf(F, "%s %i", Word, &Key) != EOF)
	{
		Node_t* new_element = (Node_t*)malloc(sizeof(Node_t));
		if (!new_element)
			return;
		new_element->data.Word = (char*)malloc(sizeof(char) * N);
		if (!new_element->data.Word)
			return;
		strcpy(new_element->data.Word, Word);
		new_element->data.Key = Key;
		Add(List, new_element);
	}

	fclose(F);
	return;
}

Node_t* Search(List_t* List, int DataKey) {
	Node_t* pointer = List->head;
	if (pointer->data.Key > DataKey)
		return NULL;
	while (pointer->next && pointer->next->data.Key < DataKey)
	{
		pointer = pointer->next;
	}
	return pointer;
}

int Add(List_t* List, Node_t* new_element) {
	Node_t* place = Search(List, new_element->data.Key);
	if (!place)
	{
		if (new_element->data.Key == List->head->data.Key)
			return Error;
		new_element->next = List->head;
		List->head = new_element;
		return Add_to_begining;
	}
	else
	{
		if (place->data.Key == new_element->data.Key)
			return Error;
		if (place->next != NULL && place->next->data.Key == new_element->data.Key)
			return Error;
		new_element->next = place->next;
		place->next = new_element;
		return Add_not_to_begining;
	}
}

void Output(List_t* List) {
	Node_t* temp_pointer = List->head;
	while (temp_pointer)
	{
		printf("%s %i\n", temp_pointer->data.Word, temp_pointer->data.Key);
		temp_pointer = temp_pointer->next;
	}
	free(temp_pointer);
	return;
}

void MemoryLeaks(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}