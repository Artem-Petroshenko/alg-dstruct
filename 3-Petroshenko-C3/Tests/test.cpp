#include "pch.h"
#include "LabC.h"

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Home, Version 20H2, Build 19042.1288
PC configuration:
	CPU: Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz 2.59 GHz
	Motherboard: TIMI TM1806
	RAM: 16 GB, DDR4, 2400 MHz
	SSD: SSD M.2 1TB
*/

/*
Lab C, Variant 3 (breadth-first traversal with list of contiguous vertices):
Results:
    Graph creation time: 22844 ms
    Traversal of the graph time: 47149 ms
    Total run time: 69996 ms
    Memory Costs: About 450 MB
*/

const char* Filename = "StressTest.txt";
const int VerticesCount = 1000000;

TEST(BFS_StressTest, GraphCreation) {
    FILE* F = fopen(Filename, "w");
    if (!F) {
        printf("File didn't open for writting\n");
        ASSERT_TRUE(F);
    }
    fprintf(F, "%i\n", VerticesCount);
    for (int CurVert = 0; CurVert < VerticesCount - 1; CurVert++) {
        fprintf(F, "%i ", CurVert);
        for (int ContVert = CurVert + 1 + rand() % (VerticesCount - 1); ContVert < VerticesCount; ContVert += 1 + rand() % (VerticesCount - 1))
            fprintf(F, "%i ", ContVert);
        fprintf(F, "\n");
    }
    fprintf(F, "%i \n", VerticesCount - 1);
    fclose(F);
}

TEST(BFS_StressTest, GraphTraversal) {
    FILE* F = fopen(Filename, "r");
    if (!F) {
        printf("File F didn't open for reading\n");
        ASSERT_TRUE(F);
    }
    FILE* Output = fopen("Output.txt", "w");
    if (!Output) {
        printf("File Output didn't open for writting\n");
        fclose(F);
        ASSERT_TRUE(Output);
    }
    Graph_t* Graph = GetGraph(F);
    if (!Graph)
    {
        printf("Error in reading Graph\n");
        fclose(F);
        fclose(Output);
        ASSERT_TRUE(Graph);
    }
    fclose(F);
    int Result = BFS(Output, Graph);
    if (!Result)
    {
        printf("Error in BFS\n");
        fclose(Output);
        FreeGraph(Graph);
        ASSERT_TRUE(Result);
    }
    fclose(Output);
}