#include "LabB.h"
#pragma warning(disable:4996)

#define TEST_BLOCKS_COUNT 10000
#define ARR_SIZE 1024
#define BLOCK_MAX 16
#define RUN_COUNT 25

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
	while (iterator != NULL)
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
		*GetSizePtr(fitblock) = MemGetBlockSize() + size;
	}
	//if we give all block to user
	else
	{
		//if this block is the first
		if (!*GetPrevPtr(fitblock))
		{
			//if this block is the only one in the list of empty blocks
			if (!*GetNextPtr(fitblock))
				sHead = NULL;
			else
			{
				*GetPrevPtr(*GetNextPtr(fitblock)) = NULL;
				sHead = *GetNextPtr(fitblock);
			}
		}
		else
		{
			//if this block is the last
			if (!*GetNextPtr(fitblock))
				*GetNextPtr(*GetPrevPtr(fitblock)) = NULL;
			else
			{
				*GetPrevPtr(*GetNextPtr(fitblock)) = *GetPrevPtr(fitblock);
				*GetNextPtr(*GetPrevPtr(fitblock)) = *GetNextPtr(fitblock);
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

void MemFree(void* p) {
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
		//if right block is the first
		if (!*GetPrevPtr(rightblock))
		{
			//if right block is not the only one
			if (*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(rightblock)) = *GetPrevPtr(rightblock); // = NULL
			sHead = *GetNextPtr(rightblock);
		}
		else
		{
			//if right block is not the last
			if (*GetNextPtr(rightblock))
				*GetPrevPtr(*GetNextPtr(rightblock)) = *GetPrevPtr(rightblock);
			*GetNextPtr(*GetPrevPtr(rightblock)) = *GetNextPtr(rightblock);// maybe NULL
		}
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

int MemGetMinimumSize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int MemGetBlockSize() {
	return sizeof(int) + sizeof(void*) + sizeof(void*) + sizeof(int);
}

int main(int argc, char* argv[]) {
	/*int seed;
	if (argc > 1) {
		sscanf(argv[1], "%d", &seed);
	}
	else {
		srand(time(0));
		seed = rand() % 1024;
		printf("seed: %d\n", seed);
	}*/
	srand(180);
	void* arr[ARR_SIZE];
	char* ones[BLOCK_MAX];
	int sizes[ARR_SIZE];
	memset(ones, '1', BLOCK_MAX * sizeof(ones[0]));
	void* buf = malloc(1000000);
	MemInit(buf, 1000000);
	for (int r = 0; r < RUN_COUNT; r++) {
		for (int i = 0; i < ARR_SIZE; i++) {
			int size = 1 + rand() % BLOCK_MAX;
			arr[i] = MemAlloc(size);
			sizes[i] = size;
			memset(arr[i], '1', size);
		}
		for (int i = 0; i < ARR_SIZE; i++) {
			int index = rand() % (ARR_SIZE - i);
			if (memcmp(arr[index], ones, sizes[index])) {
				printf("memory contents changed\n");
				errno = 33;
			}
			MemFree(arr[index]);
			if (errno) {
				errno = 0;
				printf("run: %d, iteration: %d\n", r, i);
			}
			arr[index] = arr[ARR_SIZE - 1 - i];
			sizes[index] = sizes[ARR_SIZE - 1 - i];
		}
	}
	MemDone();
	free(buf);
	return 0;
}

/*int main() {
	const int TEST_BLOCK_SIZE = 16;
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
		int size = rand() % (TEST_BLOCK_SIZE - 1) + 1;
		blocks[i] = (char*)MemAlloc(size);
		memset(blocks[i], 'a', size);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		blocks[2 * i] = (char*)MemAlloc(TEST_BLOCK_SIZE);
		memset(blocks[2 * i], 'a', TEST_BLOCK_SIZE);
	}
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i + 1]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		MemFree(blocks[2 * i]);
	}
	MemDone();
	free(blocks);
	free(ptr);
	return 0;
}*/