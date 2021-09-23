#include "pch.h"
#include "LabA.h"

//in all tests Test.txt contains string "first" and key 1
TEST(ListCreate, ListCreated_ExpectedListHeadFillsRight) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	char* test_word = "first";
	EXPECT_TRUE(!strcmp(Test_List->head->data.Word, test_word));
	EXPECT_TRUE(Test_List->head->data.Key == 1);
	EXPECT_TRUE(Test_List->head->next == NULL);
	ListDestroy(Test_List);
}

TEST(Search, TheSmallestKeyNumber_ExpectedreturnNULL) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	int TestKey = 0;
	EXPECT_TRUE(Search(Test_List, TestKey) == NULL);
	ListDestroy(Test_List);
}

TEST(Search, UsualKeyNumber_ExpectedreturnNotNULL) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	int TestKey = 2;
	EXPECT_TRUE(Search(Test_List, TestKey) != NULL);
	ListDestroy(Test_List);
}

TEST(Add, KeyNumbersAreEqual_ExpectedError) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	Node_t* new_element = (Node_t*)malloc(sizeof(Node_t));
	new_element->data.Word = "error";
	new_element->data.Key = 1;
	new_element->next = NULL;
	EXPECT_EQ(Add(Test_List, new_element), Error);
	ListDestroy(Test_List);
}

TEST(Add, AddingToTheBegining_ExpectedreturnAdd_to_begining) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	Node_t* new_element = (Node_t*)malloc(sizeof(Node_t));
	new_element->data.Word = "zero";
	new_element->data.Key = 0;
	new_element->next = NULL;
	EXPECT_EQ(Add(Test_List, new_element), Add_to_begining);
	//ListDestroy(Test_List);
}

TEST(Add, AddingNotToTheBegining_ExpectedreturnAdd_not_to_begining) {
	char* testfile = "Test.txt";
	List_t* Test_List = ListCreate(testfile);
	Node_t* new_element = (Node_t*)malloc(sizeof(Node_t));
	new_element->data.Word = "second";
	new_element->data.Key = 2;
	new_element->next = NULL;
	EXPECT_EQ(Add(Test_List, new_element), Add_not_to_begining);
	//ListDestroy(Test_List);
}