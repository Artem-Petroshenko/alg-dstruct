#pragma once
#ifndef _MY_H
#define _MY_H

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Node_t {
	int Data;
	struct Node_t* Next;
} Node_t;

typedef struct {
	Node_t* Top;
} Stack_t;

bool Input(const char* Filename, int* A, int* B);
int NumLen(int A);
int* ZeroMass(int size);
int* NumtoMass(int A);
char* GetExp(int* Nums, int* Ops, int sizeNums, int sizeOps);
int Perebor(const int B, int* Nums, int* Ops, int sizeNums, int sizeOps, int m, const char* Filename);
int* RPN(int* Nums, int* Ops, int sizeNums, int sizeOps);
bool Calculate(Stack_t* S, int operation);
int Algorythm(const int A, const int B, const char* fileoutput);

#ifdef __cplusplus
}
#endif

#endif