#ifndef __MEMALLOCATOR_H__E71A34CB
#define __MEMALLOCATOR_H__E71A34CB
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	NoMerge,
	MergeRight,
	MergeLeft
}Merge;

#define Ok 1
#define Error 0

int* GetSizePtr(void* descriptor);
void** GetNextPtr(void* descriptor);
void** GetPrevPtr(void* descriptor);
int* GetFlagPtr(void* descriptor);
int GetAbs(int x);

// Init memory system with memory block pMemory.
int MemInit(void* pMemory, int size);

// You can implement memory leak checks here
void MemDone();

// Allocate memory block of size 'size'.
// Returns pointer to memory block is success, 0 otherwise
void* MemAlloc(int size);

// Free memory previously allocated by memalloc
void MemFree(void* p);

// Return minimum size in bytes of the memory pool to allocate 0-bytes block
// use case:
// void *p   = 0;
// void *ptr = malloc(memgetminimumsize() + 1);
// meminit(ptr, memgetminimumsize() + 1)
// p = memalloc(1); // Success!
// memfree(p);
// memdone();
// free(ptr);
int MemGetMinimumSize();


// Returns size in bytes of additional information per allocation
// use case:
// void *p1 = 0, *p2 = 0;
// int  memsize = memgetminimumsize() + memgetblocksize() + 2;
// void *ptr = malloc(memsize);
// meminit(ptr, memsize);
// p1 = memalloc(1); // Success!
// p2 = memalloc(1); // Success!
// memfree(p2);
// memfree(p1);
// memdone();
// free(ptr);
int MemGetBlockSize();


#ifdef __cplusplus
}
#endif

#endif