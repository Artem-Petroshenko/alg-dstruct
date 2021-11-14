#pragma once
#ifndef _MY_H
#define _MY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#ifdef __cplusplus
extern "C"
{
#endif

#define YES 1
#define NO 0
#define MAX_NUM_LENGTH 10

typedef struct Node_t {
	int vertex;
	struct Node_t* next;
} Node_t;

typedef struct {
	Node_t* front, * back;
} Queue_t;

typedef struct {
	int ContVertsCount;
	int* ContVerts;
} Vertex_t;

typedef struct {
	int VertsCount;
	Vertex_t* VertsArray;
} Graph_t;

int NumLen(int num);
Graph_t* GetGraph(FILE* stream);
void FreeGraph(Graph_t* Graph);
int BFS(FILE* Stream, Graph_t* Graph);
Queue_t* InitQueue(void);
int isEmpty(Queue_t* Queue);
int Push(Queue_t* Queue, int vertex);
void Pop(Queue_t* Queue);
int Front(Queue_t* Queue);

#ifdef __cplusplus
}
#endif

#endif