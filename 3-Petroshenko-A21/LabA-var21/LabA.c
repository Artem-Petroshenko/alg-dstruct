#include "LabA.h"

List_t* ListCreate(const char* filename) {								//creating the list with the first element
	FILE* F;
	if ((F = fopen(filename, "r")) == NULL)								//checking for right opening the file
		return 0;
	char Word[N];														//we agreed that we have no such long words  
	int Key;

	fscanf(F, "%s %i", Word, &Key);
	node_t* head = malloc(sizeof(node_t));
	if (!head)															//checking for right memory allocation
		return 0;
	head->data.Key = Key;
	head->data.Word = (char*)malloc(sizeof(char) * N);
	strcpy(head->data.Word, Word);
	head->next = NULL;
	List_t* List = malloc(sizeof(List_t));
	if (!List)
		return 0;
	List->head = head;

	fclose(F);
	return List;
}

void ListDestroy(List_t* List) {										//freeing the memory
	node_t* temp_pointer = List->head;
	while (temp_pointer)
	{
		node_t* deleted = temp_pointer;
		temp_pointer = temp_pointer->next;
		free(deleted);
	}
	free(List);
}

void Fill(const char* filename, List_t* List) {							//just reading the elements from file
	char Word[N];														
	int Key;
	FILE* F = fopen(filename, "r");
	if (F == NULL)
		return;
	fscanf(F, "%s %i", Word, &Key);
	while (fscanf(F, "%s %i", Word, &Key) != EOF)
	{
		node_t* new_element = malloc(sizeof(node_t));
		if (!new_element)
			return;
		new_element->data.Word = (char*)malloc(sizeof(char) * N);
		strcpy(new_element->data.Word, Word);
		new_element->data.Key = Key;
		Add_and_Sort(List, new_element);
	}

	fclose(F);
	return;
}

void Add_and_Sort(List_t* List, node_t* new_element) {					//adding the new element in the right place in the list
	node_t* temp_pointer = List->head;										
	while (temp_pointer->next)
	{
		if (temp_pointer->data.Key == new_element->data.Key)			//checking error cases
		{
			puts("ERROR: The key numbers are equal!\n");
			return;
		}
		else if (temp_pointer->data.Key < new_element->data.Key)		//going through the list
			temp_pointer = temp_pointer->next;
		else if (temp_pointer->data.Key > new_element->data.Key)
		{
			if (temp_pointer == List->head)								//in case of finding the smallest current key-number 
			{
				new_element->next = temp_pointer;
				List->head = new_element;
				return;
			}
			else
			{
				new_element->next = temp_pointer->next;
				temp_pointer->next = new_element;
				data_t temp_data = temp_pointer->data;
				temp_pointer->data = new_element->data;
				new_element->data = temp_data;
				return;
			}
		}
	}
	new_element->next = temp_pointer->next;						//comparing with the last element
	temp_pointer->next = new_element;							//it's more comfortable because of
	if (temp_pointer->data.Key > new_element->data.Key)			//NULL pointering
	{
		data_t temp_data = temp_pointer->data;
		temp_pointer->data = new_element->data;
		new_element->data = temp_data;
	}
	return;
}

void Output(List_t* List) {
	node_t* temp_pointer = List->head;
	while (temp_pointer)
	{
		printf("%s %i\n", temp_pointer->data.Word, temp_pointer->data.Key);
		temp_pointer = temp_pointer->next;
	}
	return;
}