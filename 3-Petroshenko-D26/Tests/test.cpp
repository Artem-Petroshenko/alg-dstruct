#include "pch.h"
#include "LabD.h"

#define MAX_LONG 19
#define MAX_SIZE 2 * MAX_LONG + 1

//Functional tests
TEST(MinAMinB, Expect_printing_1_in_file) {
	const char* input = "test_files/MinAMinB/input.txt";
	const char* output = "test_files/MinAMinB/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "1\n"));
	free(String);
}

TEST(SomeASomeB_1, Expect_printing_389_273_in_file) {
	const char* input = "test_files/SomeASomeB_1/input.txt";
	const char* output = "test_files/SomeASomeB_1/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "389+273\n"));
	free(String);
}

TEST(SomeASomeB_2, Expect_printing_0_in_file) {
	const char* input = "test_files/SomeASomeB_2/input.txt";
	const char* output = "test_files/SomeASomeB_2/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "0\n"));
	free(String);
}

TEST(SomeASomeB_3, Expect_printing_23_34_566_in_file) {
	const char* input = "test_files/SomeASomeB_3/input.txt";
	const char* output = "test_files/SomeASomeB_3/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "23*34-566\n"));
	free(String);
}

TEST(SomeASomeB_4, Expect_printing_0_in_file) {
	const char* input = "test_files/SomeASomeB_4/input.txt";
	const char* output = "test_files/SomeASomeB_4/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "0\n"));
	free(String);
}

TEST(SomeASomeB_5, Expect_printing_111_1_1_2_in_file) {
	const char* input = "test_files/SomeASomeB_5/input.txt";
	const char* output = "test_files/SomeASomeB_5/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "111+1+1/2\n"));
	free(String);
}

TEST(SomeASomeB_6, Expect_printing_1_2_3_in_file) {
	const char* input = "test_files/SomeASomeB_6/input.txt";
	const char* output = "test_files/SomeASomeB_6/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "1+2+3\n"));
	free(String);
}

TEST(SomeASomeB_7, Expect_printing_1_00_in_file) {
	const char* input = "test_files/SomeASomeB_7/input.txt";
	const char* output = "test_files/SomeASomeB_7/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "1+00\n"));
	free(String);
}

TEST(BigABigB, Expect_printing_9999999999999_9_in_file) {
	const char* input = "test_files/BigABigB/input.txt";
	const char* output = "test_files/BigABigB/output.txt";
	Algorythm(input, output);
	char* String = (char*)malloc(MAX_SIZE * sizeof(char));
	if (!String)
		ASSERT_TRUE(String);
	FILE* F = fopen(output, "r");
	if (!F)
	{
		free(String);
		ASSERT_TRUE(F);
	}
	fgets(String, MAX_SIZE, F);
	fclose(F);
	EXPECT_TRUE(!strcmp(String, "9999999999999+9\n"));
	free(String);
}