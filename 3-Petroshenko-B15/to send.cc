#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "memallocator.h"

typedef enum {
	NoMerge,
	MergeRight,
	MergeLeft
}Merge;

#define Ok 1
#define Error 0

//pointer on the begining of the using heap
void* sMemory = NULL;
//size of the heap
int sizeofs = 0;
//pointer on list of empty blocks
void* sHead = NULL;

//next 4 funcs is extra for control data in memory blocks
int* GetSizePtr(void* descriptor) {
	return (int*)descriptor;
}

void** GetNextPtr(void* descriptor) {
	return (void**)((char*)descriptor + sizeof(int));
}

void** GetPrevPtr(void* descriptor) {
	return (void**)((char*)descriptor + sizeof(int) + sizeof(void*));
}

//Flag is contained in the end of the block
int* GetFlagPtr(void* descriptor) {
	return (int*)((char*)descriptor + *GetSizePtr(descriptor) - sizeof(int));
}

int meminit(void* pMemory, int size) {
	if (!pMemory)
		return Error;
	if (size <= memgetminimumsize())
		return Error;
	void* initdesc = pMemory;
	*GetSizePtr(initdesc) = size;
	*GetNextPtr(initdesc) = NULL;
	*GetPrevPtr(initdesc) = NULL;
	*GetFlagPtr(initdesc) = -size;
	sMemory = pMemory;
	sizeofs = size;
	sHead = initdesc;
	return Ok;
}

void memdone() {
	void* iterator = sMemory;
	while ((char*)iterator < (char*)sMemory + sizeofs)
	{
		//Positive value of flag means that this block is used by user
		if (*GetFlagPtr(iterator) > 0)
			fprintf(stderr, "MEMORY LEAK at 0x%p\n", (char*)iterator + sizeof(int) + sizeof(void*) + sizeof(void*));
		iterator = (void*)((char*)iterator + *GetSizePtr(iterator));
	}
	return;
}

void* memalloc(int size) {
	if (size <= 0)
		return NULL;
	if (size > sizeofs - memgetblocksize())
		return NULL;
	void* iterator = sHead;
	void* fitblock = NULL;
	int min = sizeofs + 1;
	//searching for best fit block
	while (iterator != NULL)
	{
		if (*GetSizePtr(iterator) >= size + memgetblocksize() && *GetSizePtr(iterator) < min)
		{
			fitblock = iterator;
			min = *GetSizePtr(iterator);
		}
		iterator = *GetNextPtr(iterator);
	}
	if (!fitblock)
		return NULL;
	//fragmentation:
	//if we have enough memory to create new descriptor for empty part
	if (*GetSizePtr(fitblock) > memgetblocksize() + size + memgetblocksize())
	{
		//right part of the block, i.e. empty part
		void* newdescriptor = (void*)((char*)fitblock + memgetblocksize() + size);
		*GetSizePtr(newdescriptor) = *GetSizePtr(fitblock) - size - memgetblocksize();
		*GetNextPtr(newdescriptor) = *GetNextPtr(fitblock);
		*GetPrevPtr(newdescriptor) = *GetPrevPtr(fitblock);
		*GetFlagPtr(newdescriptor) = -*GetSizePtr(newdescriptor);
		//if first block is the best fit
		if (!*GetPrevPtr(fitblock))
		{
			//if best fit block is not the only one in the list of empty blocks
			if (*GetNextPtr(fitblock))
				*GetPrevPtr(*GetNextPtr(fitblock)) = newdescriptor;
			sHead = newdescriptor;
		}
		else
		{
			//if the best fit is not the last
			if (*GetNextPtr(fitblock))
				*GetPrevPtr(*GetNextPtr(fitblock)) = newdescriptor;
			*GetNextPtr(*GetPrevPtr(fitblock)) = newdescriptor;
		}
		*GetSizePtr(fitblock) = memgetblocksize() + size;
	}
	//if we give all block to user
	else
	{
		//if this block is the first
		if (!*GetPrevPtr(fitblock))
		{
			//if this block is not the only one in the list of empty blocks
			if (*GetNextPtr(fitblock))
			{
				*GetPrevPtr(*GetNextPtr(fitblock)) = NULL;
				sHead = *GetNextPtr(fitblock);
			}
			else
				sHead = NULL;
		}
		else
		{
			//if this block is not the last
			if (*GetNextPtr(fitblock))
			{
				*GetPrevPtr(*GetNextPtr(fitblock)) = *GetPrevPtr(fitblock);
				*GetNextPtr(*GetPrevPtr(fitblock)) = *GetNextPtr(fitblock);
			}
			else
			{
				*GetNextPtr(*GetPrevPtr(fitblock)) = NULL;
				*GetPrevPtr(*GetNextPtr(fitblock)) = *GetPrevPtr(fitblock);
			}
		}
	}
	//left part of the block, i.e. user's part
	*GetNextPtr(fitblock) = NULL;
	*GetPrevPtr(fitblock) = NULL;
	*GetFlagPtr(fitblock) = *GetSizePtr(fitblock);
	//returning pointer exactly on data
	return (void*)((char*)fitblock + sizeof(int) + sizeof(void*) + sizeof(void*));
}

void memfree(void* p) {
	if (!p)
		return;
	void* pdescriptor = (void*)((char*)p - sizeof(int) - sizeof(void*) - sizeof(void*));
	void* leftblock = NULL;
	void* rightblock = NULL;
	//if we are going to free the first block in heap
	if ((char*)pdescriptor - 1 > (char*)sMemory)
		leftblock = (void*)((char*)pdescriptor - abs(*((int*)pdescriptor - 1)));
	//if we are going to free the last block in heap
	if ((char*)pdescriptor + *GetSizePtr(pdescriptor) < (char*)sMemory + sizeofs)
		rightblock = (void*)((char*)pdescriptor + *GetSizePtr(pdescriptor));
	Merge right = NoMerge, left = NoMerge;
	if (leftblock && *GetFlagPtr(leftblock) < 0)
		left = MergeLeft;
	if (rightblock && *GetFlagPtr(rightblock) < 0)
		right = MergeRight;
	//Start merging
	if (left == MergeLeft)
	{
		//it doen't matter if left block is the first/last or not
		*GetSizePtr(leftblock) += *GetSizePtr(pdescriptor);
		*GetFlagPtr(leftblock) = -*GetSizePtr(leftblock);
		pdescriptor = leftblock;
	}
	if (right == MergeRight)
	{
		//if we didn't merge with left block we merge p with right block and add it to the begining of the list of empty blocks
		if (left == NoMerge)
		{
			*GetNextPtr(pdescriptor) = sHead;
			*GetPrevPtr(sHead) = pdescriptor;
			*GetPrevPtr(pdescriptor) = NULL;
			sHead = pdescriptor;
		}
		//repoint next and prev pointers to each other not to lose them after merge
		if (*GetPrevPtr(rightblock))
			*GetNextPtr(*GetPrevPtr(rightblock)) = *GetNextPtr(rightblock);
		if (*GetNextPtr(rightblock))
			*GetPrevPtr(*GetNextPtr(rightblock)) = *GetPrevPtr(rightblock);
		*GetSizePtr(pdescriptor) += *GetSizePtr(rightblock);
		*GetFlagPtr(pdescriptor) = -*GetSizePtr(pdescriptor);
	}
	if (left == NoMerge && right == NoMerge)
	{
		//Adding freeing memory to the begining of the list of empty blocks
		*GetNextPtr(pdescriptor) = sHead;
		if (sHead)
			*GetPrevPtr(sHead) = pdescriptor;
		*GetPrevPtr(pdescriptor) = NULL;
		sHead = pdescriptor;
		*GetFlagPtr(pdescriptor) = -*GetSizePtr(pdescriptor);
	}
	return;
}

int memgetminimumsize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int memgetblocksize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}