#include "LabC.h"

//Func for calculating length of number
int NumLen(int num) {
	int k = 0;
	while (num > 0)
	{
		num /= 10;
		k++;
	}
	return k;
}

void FreeGraph(Graph_t* Graph) {
	for(int i = 0; i < Graph->VertsCount; i++)
		free(Graph->VertsArray[i].ContVerts);
	free(Graph->VertsArray);
	free(Graph);
	return;
}

//Filling the Graph
Graph_t* GetGraph(FILE* Stream) {
	Graph_t* OurGraph = (Graph_t*)malloc(sizeof(Graph_t));
	if (!OurGraph)
		return NULL;
	//Pointer for getting the strings
	char* LineBuf = (char*)malloc(MAX_NUM_LENGTH * sizeof(char));
	//Pointer to iterate over the string 
	char* BufIterator = NULL;
	fgets(LineBuf, MAX_NUM_LENGTH, Stream);
	if (!LineBuf)
	{
		free(OurGraph);
		return NULL;
	}
	OurGraph->VertsCount = atoi(LineBuf);
	if (!OurGraph->VertsCount)
	{
		FreeGraph(OurGraph);
		free(LineBuf);
		return NULL;
	}
	//Calculating max length of string
	int MaxLineLen = OurGraph->VertsCount * (NumLen(OurGraph->VertsCount - 1) + 1) + 1;
	//Reallocating memory for new length
	char* tmp = (char*)realloc(LineBuf, sizeof(char) * MaxLineLen);
	if (!tmp)
	{
		FreeGraph(OurGraph);
		free(LineBuf);
		return NULL;
	}
	LineBuf = tmp;
	//Allocating memory for array of vertices
	OurGraph->VertsArray = (Vertex_t*)malloc(sizeof(Vertex_t) * OurGraph->VertsCount);
	if (!OurGraph->VertsArray)
	{
		free(OurGraph);
		free(LineBuf);
		return NULL;
	}
	//Initialising VertsArray with 0
	for (int k = 0; k < OurGraph->VertsCount; k++)
	{
		OurGraph->VertsArray[k].ContVertsCount = 0;
		OurGraph->VertsArray[k].ContVerts = (int*)malloc(1);
		if (!OurGraph->VertsArray[k].ContVerts)
		{
			FreeGraph(OurGraph);
			free(LineBuf);
			return NULL;
		}
	}
	//i - current vertex, j - contiguous vertex to i
	int i, j;
	for (int k = 0; k < OurGraph->VertsCount; k++)
	{
		fgets(LineBuf, MaxLineLen, Stream);
		BufIterator = LineBuf;
		sscanf(BufIterator, "%i", &i);
		BufIterator += NumLen(i) + 1;
		while (sscanf(BufIterator, "%i", &j) > 0)
		{
			BufIterator += NumLen(j) + 1;
			OurGraph->VertsArray[i].ContVertsCount++;
			OurGraph->VertsArray[j].ContVertsCount++;
			int* tmp1 = (int*)realloc(OurGraph->VertsArray[i].ContVerts, sizeof(int) * OurGraph->VertsArray[i].ContVertsCount);
			int* tmp2 = (int*)realloc(OurGraph->VertsArray[j].ContVerts, sizeof(int) * OurGraph->VertsArray[j].ContVertsCount);
			if (!tmp1 || !tmp2)
			{
				if (tmp1)
					free(tmp1);
				FreeGraph(OurGraph);
				free(BufIterator);
				free(LineBuf);
				return NULL;
			}
			OurGraph->VertsArray[i].ContVerts = tmp1;
			OurGraph->VertsArray[j].ContVerts = tmp2;
			OurGraph->VertsArray[i].ContVerts[OurGraph->VertsArray[i].ContVertsCount - 1] = j;
			OurGraph->VertsArray[j].ContVerts[OurGraph->VertsArray[j].ContVertsCount - 1] = i;
		}
	}
	free(LineBuf);
	return OurGraph;
}

Queue_t* InitQueue(void) {
	Queue_t* Q = (Queue_t*)malloc(sizeof(Queue_t));
	Q->back = NULL;
	Q->front = NULL;
	return Q;
}

int isEmpty(Queue_t* Queue) {
	return !Queue->front ? YES : NO;
}

void Push(Queue_t* Queue, int vertex) {
	Node_t* Element = (Node_t*)malloc(sizeof(Node_t));
	Element->vertex = vertex;
	Element->next = NULL;
	if (isEmpty(Queue))
	{
		Queue->back = Element;
		Queue->front = Element;
	}
	else
	{
		Queue->back->next = Element;
		Queue->back = Element;
	}
	return;
}

void Pop(Queue_t* Queue) {
	Node_t* ToFree = Queue->front;
	Queue->front = Queue->front->next;
	free(ToFree);
	return;
}

//Func for getting the first element in the queue
int Front(Queue_t* Queue) {
	return Queue->front->vertex;
}

int BFS(FILE* Stream, Graph_t* Graph) {
	if (!Graph)
		return 0;
	Queue_t* Q = InitQueue();
	if (!Q)
	{
		FreeGraph(Graph);
		return 0;
	}
	int* Used = (int*)malloc(sizeof(int) * Graph->VertsCount);
	if (!Used)
	{
		FreeGraph(Graph);
		free(Q);
		return 0;
	}
	Push(Q, 0);
	Used[0] = YES;
	while (!isEmpty(Q))
	{
		//Pre-oder
		fprintf(Stream, "%i ", Front(Q));
		for (int i = 0; i < Graph->VertsArray[Front(Q)].ContVertsCount; i++)
		{
			if (Used[Graph->VertsArray[Front(Q)].ContVerts[i]] != YES)
			{
				Push(Q, Graph->VertsArray[Front(Q)].ContVerts[i]);
				Used[Graph->VertsArray[Front(Q)].ContVerts[i]] = YES;
			}
		}
		Pop(Q);
	}
	FreeGraph(Graph);
	free(Q);
	free(Used);
	return 1;
}