#include "pch.h"
#include "LabD.h"

int A, B;

//Testing Input function, file contains "12345\n123"
TEST(ReadingFile, Expect_getting_right_number) {
	const char* testfile = "test_input.txt";
	ASSERT_TRUE(Input(testfile, &A, &B));
	EXPECT_EQ(A, 12345);
	EXPECT_EQ(B, 123);
}

//Functional tests
TEST(MinAMinB, Expect_printing_1_in_file) {
	remove("test.txt");
	A = 1;
	B = 1;
	int size = (2 * NumLen(A) + 1); //for digits, operations and \n
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "1\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_1, Expect_printing_389_273_in_file) {
	remove("test.txt");
	A = 389273;
	B = 662;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "389+273\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_2, Expect_printing_0_in_file) {
	remove("test.txt");
	A = 1234;
	B = 128;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "0\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_3, Expect_printing_23_34_566_in_file) {
	remove("test.txt");
	A = 2334566;
	B = 216;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "23*34-566\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_4, Expect_printing_0_in_file) {
	remove("test.txt");
	A = 123456;
	B = 123457;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "0\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_5, Expect_printing_111_1_1_2_in_file) {
	remove("test.txt");
	A = 111112;
	B = 112;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "111+1+1/2\n"));
	free(string);
	fclose(F);
}

TEST(SomeASomeB_6, Expect_printing_1_2_3_in_file) {
	remove("test.txt");
	A = 123;
	B = 6;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "1+2+3\n")); //1*2*3 fits too but we print first fit
	free(string);
	fclose(F);
}

TEST(SomeASomeB_7, Expect_printing_1_00_in_file) {
	remove("test.txt");
	A = 100;
	B = 1;
	int size = (2 * NumLen(A) + 1);
	char* string = (char*)malloc(size * sizeof(char));
	if (!string)
		ASSERT_TRUE(string);
	Algorythm(A, B, "test.txt");
	FILE* F = fopen("test.txt", "r");
	if (!F)
	{
		free(string);
		ASSERT_TRUE(F);
	}
	fgets(string, 2 * NumLen(A) + 1, F);
	EXPECT_TRUE(!strcmp(string, "1+00\n"));
	free(string);
	fclose(F);
}

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
	Full algorythm time: 32658 ms
	Total run time: 32659 ms
	Memory Costs: About 1 MB
*/

TEST(Stress_Test, Full_algorythm) {
	remove("stress_test_output.txt");
	const char* fileinput = "stress_test_input.txt";
	const char* fileoutput = "stress_test_output.txt";
	ASSERT_TRUE(Input(fileinput, &A, &B));
	FILE* F = fopen(fileoutput, "w");
	if (!F)
		ASSERT_TRUE(F);
	Algorythm(A, B, fileoutput);
	fclose(F);
}