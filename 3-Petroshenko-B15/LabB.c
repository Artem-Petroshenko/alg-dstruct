#include "LabB.h"
#define TEST_BLOCKS_COUNT 1000

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

int MemInit(void* pMemory, int size) {
	if (!pMemory)
		return Error;
	if (size <= MemGetMinimumSize())
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

void MemDone() {
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

void* MemAlloc(int size) {
	if (size <= 0)
		return NULL;
	if (size > sizeofs - MemGetBlockSize())
		return NULL;
	void* iterator = sHead;
	void* fitblock = NULL;
	int min = sizeofs + 1;
	//searching for best fit block
	while (iterator)
	{
		if (*GetSizePtr(iterator) >= size + MemGetBlockSize() && *GetSizePtr(iterator) < min)
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
	if (*GetSizePtr(fitblock) > MemGetBlockSize() + size + MemGetBlockSize())
	{
		//right part of the block, i.e. empty part
		void* newdescriptor = (void*)((char*)fitblock + MemGetBlockSize() + size);
		*GetSizePtr(newdescriptor) = *GetSizePtr(fitblock) - size - MemGetBlockSize();
		*GetNextPtr(newdescriptor) = *GetNextPtr(fitblock);
		*GetPrevPtr(newdescriptor) = *GetPrevPtr(fitblock);
		*GetFlagPtr(newdescriptor) = -*GetSizePtr(newdescriptor);
		//if first block is the best fit
		if (!*GetPrevPtr(fitblock))
			sHead = newdescriptor;
		else
			*GetNextPtr(*GetPrevPtr(fitblock)) = newdescriptor;
	}
	//we need to allocate all memory block for user
	else
	{
		if (!*GetPrevPtr(fitblock))
			sHead = *GetNextPtr(fitblock);
		else
			*GetNextPtr(*GetPrevPtr(fitblock)) = *GetNextPtr(fitblock);
	}
	//left part of the block, i.e. user's part
	*GetNextPtr(fitblock) = NULL;
	*GetPrevPtr(fitblock) = NULL;
	*GetSizePtr(fitblock) = MemGetBlockSize() + size;
	*GetFlagPtr(fitblock) = *GetSizePtr(fitblock);
	//returning pointer exactly on data
	return (void*)((char*)fitblock + sizeof(int) + sizeof(void*) + sizeof(void*));
}

void MemFree(void* p) {
	void* pdescriptor = (void*)((char*)p - sizeof(int) - sizeof(void*) - sizeof(void*));
	void* leftblock = NULL;
	void* rightblock = NULL;
	//if we are going to free the first block in heap
	if ((char*)pdescriptor - 1 > (char*)sMemory)
		leftblock = (void*)((char*)pdescriptor - abs(*((char*)pdescriptor - sizeof(int))));
	//if we are going to free the last block in heap
	if ((char*)pdescriptor + *GetSizePtr(pdescriptor) < (char*)sMemory + sizeofs)
		rightblock = (void*)((char*)pdescriptor + *GetSizePtr(pdescriptor));
	Merge right = NoMerge, left = NoMerge;
	if (leftblock && *GetFlagPtr(leftblock) < 0)
		left = MergeLeft;
	if (rightblock && *GetFlagPtr(rightblock) < 0)
		right = MergeRight;
	//Start merging
	if (right == MergeRight && left == MergeLeft)
	{
		//if right block is the first
		if (!*GetPrevPtr(rightblock))
		{
			*GetNextPtr(*GetPrevPtr(leftblock)) = *GetNextPtr(leftblock);
			//if left block is the last, pointer "next" is NULL
			if (*GetNextPtr(leftblock))
				*GetPrevPtr(*GetNextPtr(leftblock)) = *GetPrevPtr(leftblock);
			*GetNextPtr(leftblock) = *GetNextPtr(rightblock);
			*GetPrevPtr(leftblock) = NULL;
			sHead = leftblock;
			*GetSizePtr(leftblock) += *GetSizePtr(pdescriptor) + *GetSizePtr(rightblock);
			*GetFlagPtr(leftblock) = -*GetSizePtr(leftblock);
		}
		//if left block is the first
		else if (!*GetPrevPtr(leftblock))
		{
			*GetNextPtr(*GetPrevPtr(rightblock)) = *GetNextPtr(rightblock);
			//if right block is the last, pointer "next" is NULL
			if(*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(rightblock)) = *GetPrevPtr(rightblock);
			*GetSizePtr(leftblock) += *GetSizePtr(pdescriptor) + *GetSizePtr(rightblock);
			*GetFlagPtr(leftblock) = -*GetSizePtr(leftblock);
		}
		//if left and right blocks are not the first
		else if (*GetPrevPtr(leftblock) && *GetPrevPtr(rightblock))
		{
			*GetNextPtr(*GetPrevPtr(rightblock)) = *GetNextPtr(rightblock);
			if (*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(rightblock)) = *GetPrevPtr(rightblock);
			*GetSizePtr(leftblock) += *GetSizePtr(pdescriptor) + *GetSizePtr(rightblock);
			*GetFlagPtr(leftblock) = -*GetSizePtr(leftblock);
		}
	}
	if (left == MergeLeft && right == NoMerge)
	{
		//it doen't matter if left block is the first/last or not
		*GetSizePtr(leftblock) += *GetSizePtr(pdescriptor);
		*GetFlagPtr(leftblock) = -*GetSizePtr(leftblock);
	}
	if (right == MergeRight && left == NoMerge)
	{
		*GetNextPtr(pdescriptor) = *GetNextPtr(rightblock);
		*GetPrevPtr(pdescriptor) = *GetPrevPtr(rightblock);
		//if right block is the first
		if (!*GetPrevPtr(rightblock))
		{
			//if right block is not the only one empty block
			if (*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(pdescriptor)) = pdescriptor;
			sHead = pdescriptor;
		}
		//if right block is not the first
		else
		{
			//if the right block is not the last
			if (*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(pdescriptor)) = pdescriptor;
			*GetNextPtr(*GetPrevPtr(pdescriptor)) = pdescriptor;
		}
		*GetSizePtr(pdescriptor) += *GetSizePtr(rightblock);
		*GetFlagPtr(pdescriptor) = -*GetSizePtr(pdescriptor);
	}
	if (left == NoMerge && right == NoMerge)
	{
		//Adding freeing memory to the begining of the list of empty blocks
		*GetNextPtr(pdescriptor) = sHead;
		*GetPrevPtr(pdescriptor) = NULL;
		if (sHead)
			*GetPrevPtr(sHead) = pdescriptor;
		sHead = pdescriptor;
		*GetFlagPtr(pdescriptor) *= -1;
	}
}

int MemGetMinimumSize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int MemGetBlockSize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int main() {
	/*const int TEST_BLOCK_SIZE = 16;
	const int TEST_MEMORY_SIZE = TEST_BLOCKS_COUNT * TEST_BLOCK_SIZE;
	const int TEST_MEMORY_SIZE_INIT = 2 * TEST_BLOCKS_COUNT * (TEST_BLOCK_SIZE + MemGetBlockSize()); //multiplied by 2 so fragmentation will not fail the test
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	int bytes_init = MemInit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		return -1;
	}
	void** blocks = (void**)malloc(TEST_BLOCKS_COUNT * sizeof(void*));
	if (!blocks) {
		free(ptr);
		return -1;
	}
	for (int i = 0; i < TEST_BLOCKS_COUNT; i++) {
		blocks[i] = (char*)MemAlloc(1);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		blocks[2 * i] = (char*)MemAlloc(1);
	}
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i + 1]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i]);
	}
	MemDone();
	free(blocks);
	free(ptr);*/
	return 0;
}