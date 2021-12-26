#ifndef _MY_H
#define _MY_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node_t {
	long long Data;
	struct Node_t* Next;
} Node_t;

typedef struct {
	Node_t* Top;
} Stack_t;

bool Input(const char* Filename, long long* A, long long* B);
int NumLen(long long A);
int* ZeroArray(int Size);
int* NumtoArray(long long A);
char* GetExp(int* Nums, int* Ops, int SizeNums, int SizeOps);
int Enumeration(long long B, int* Nums, int* Ops, int SizeNums, int SizeOps, int m, const char* Filename);
long long RPN(int* Nums, int* Ops, int SizeNums, int SizeOps);
bool Calculate(Stack_t* S, int Operation);
int Algorithm(const char* FileInput, const char* FileOutput);

#ifdef __cplusplus
}
#endif

#endif