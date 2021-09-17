#include "LabA.h"

int main(void)
{
	const char* filename = "Test.txt";
	List_t* List = ListCreate(filename);
	Fill(filename, List);
	Output(List);

	ListDestroy(List);
	return 0;
}