#include "LabA.h"

List_t* ListCreate(char* Word, int Key) {								//creating the list with the first element
	Node_t* head = Create_Element(Word, Key);
	List_t* List = (List_t*)malloc(sizeof(List_t));
	if (!List)
	{
		free(head->data.Word);
		free(head);
		return 0;
	}
	List->head = head;
	return List;
}

Node_t* Create_Element(char* Word, int Key) {
	Node_t* new_element = (Node_t*)malloc(sizeof(Node_t));
	if (!new_element)
		return NULL;
	new_element->data.Word = (char*)malloc(sizeof(char) * (strlen(Word) + 1));
	if (!new_element->data.Word)
	{
		free(new_element);
		return NULL;
	}
	strcpy(new_element->data.Word, Word);
	new_element->data.Key = Key;
	new_element->next = NULL;
	return new_element;
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

Node_t* Search_place(List_t* List, int DataKey) {
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
	Node_t* place = Search_place(List, new_element->data.Key);
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

int Search_Word(List_t* List, int DataKey) {								// returns 1 if there is such a word and returns 0 if not
	Node_t* pointer = List->head;
	while (pointer)
	{
		if (pointer->data.Key == DataKey)
			return 1;
		pointer = pointer->next;
	}
	return 0;
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