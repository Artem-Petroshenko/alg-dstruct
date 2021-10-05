#include "pch.h"
#include "LabA.h"

Node_t new_element;
List_t test_list;
Node_t head;

TEST(ElementCreate, ElementCreated_ExpectedEqualityofData) {
	new_element.data.Word = "test";
	new_element.data.Key = 1;
	new_element.next = NULL;
	Node_t* element2 = ElementCreate("test", 1);
	EXPECT_TRUE(!strcmp(new_element.data.Word, element2->data.Word));
	EXPECT_EQ(new_element.data.Key, element2->data.Key);
	EXPECT_EQ(new_element.next, element2->next);
	free(element2->data.Word);
	free(element2);
}

TEST(ListCreate, ListCreated_ExpectedListHeadnotNULLanditsDataisRight) {
	List_t* test_list = ListCreate("head", 1);
	EXPECT_TRUE(test_list->head != NULL);
	EXPECT_TRUE(!strcmp(test_list->head->data.Word, "head"));
	EXPECT_EQ(test_list->head->data.Key, 1);
	ListDestroy(test_list);
}

TEST(Search_place, TheSmallestKeyNumber_ExpectedreturnNULL) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	int test_key = 0;
	EXPECT_TRUE(Search_place(&test_list, test_key) == NULL);
}

TEST(Search_place, UsualKeyNumber_ExpectedreturnNotNULL) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	int test_key = 2;
	EXPECT_TRUE(Search_place(&test_list, test_key) == test_list.head);
}

TEST(Add, KeyNumbersAreEqual_ExpectedError) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	new_element.data.Word = "error";
	new_element.data.Key = 1;
	new_element.next = NULL;
	EXPECT_EQ(Add(&test_list, &new_element), Error);
}

TEST(Add, AddingToTheBegining_ExpectedNewElementAsHeadofList) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	new_element.data.Word = "zero";
	new_element.data.Key = 0;
	new_element.next = NULL;
	EXPECT_EQ(Add(&test_list, &new_element), Add_to_begining);
	EXPECT_TRUE(test_list.head == &new_element);
	EXPECT_TRUE(!strcmp(test_list.head->data.Word, "zero"));
	EXPECT_EQ(test_list.head->data.Key, 0);
	EXPECT_TRUE(test_list.head->next == &head);
	EXPECT_TRUE(!strcmp(test_list.head->next->data.Word, "head"));
	EXPECT_EQ(test_list.head->next->data.Key, 1);
}

TEST(Add, AddingNotToTheBegining_ExpectedNewElementAsHeadNext) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	new_element.data.Word = "second";
	new_element.data.Key = 2;
	new_element.next = NULL;
	EXPECT_EQ(Add(&test_list, &new_element), Add_not_to_begining);
	EXPECT_TRUE(test_list.head->next == &new_element);
	EXPECT_TRUE(!strcmp(test_list.head->next->data.Word, "second"));
	EXPECT_EQ(test_list.head->next->data.Key, 2);
	EXPECT_TRUE(test_list.head == &head);
	EXPECT_TRUE(!strcmp(test_list.head->data.Word, "head"));
	EXPECT_EQ(test_list.head->data.Key, 1);
}

TEST(Add, AddingElement_ExpectedAddingElementtotheRightPlace) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	test_list.head = &head;
	Node_t new_element1, new_element2, new_element3;
	new_element1.data.Word = "second";
	new_element1.data.Key = 2;
	new_element1.next = NULL;
	new_element2.data.Word = "5th";
	new_element2.data.Key = 5;
	new_element2.next = NULL;
	new_element3.data.Word = "zero";
	new_element3.data.Key = 0;
	new_element3.next = NULL;
	new_element.data.Word = "third";
	new_element.data.Key = 3;
	new_element.next = NULL;
	EXPECT_EQ(Add(&test_list, &new_element), Add_not_to_begining);
	EXPECT_EQ(Add(&test_list, &new_element1), Add_not_to_begining);
	EXPECT_EQ(Add(&test_list, &new_element2), Add_not_to_begining);
	EXPECT_EQ(Add(&test_list, &new_element3), Add_to_begining);
	EXPECT_TRUE(test_list.head->next->next->next == &new_element);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->next->data.Word, "third"));
	EXPECT_EQ(test_list.head->next->next->next->data.Key, 3);
	EXPECT_TRUE(test_list.head->next->next == &new_element1);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->data.Word, "second"));
	EXPECT_EQ(test_list.head->next->next->data.Key, 2);
	EXPECT_TRUE(test_list.head->next->next->next->next == &new_element2);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->next->next->data.Word, "5th"));
	EXPECT_EQ(test_list.head->next->next->next->next->data.Key, 5);
	EXPECT_TRUE(test_list.head == &new_element3);
	EXPECT_TRUE(!strcmp(test_list.head->data.Word, "zero"));
	EXPECT_EQ(test_list.head->data.Key, 0);
	EXPECT_TRUE(test_list.head->next == &head);
	EXPECT_TRUE(!strcmp(test_list.head->next->data.Word, "head"));
	EXPECT_EQ(test_list.head->next->data.Key, 1);
}

TEST(Fill, FillingList_ExpextedRightFilling) {
	const char* filename = "Test.txt";
	test_list = *Fill(filename);
	EXPECT_TRUE(!strcmp(test_list.head->data.Word, "zero"));
	EXPECT_EQ(test_list.head->data.Key, 0);
	EXPECT_TRUE(!strcmp(test_list.head->next->data.Word, "first"));
	EXPECT_EQ(test_list.head->next->data.Key, 1);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->data.Word, "second"));
	EXPECT_EQ(test_list.head->next->next->data.Key, 2);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->next->data.Word, "third"));
	EXPECT_EQ(test_list.head->next->next->next->data.Key, 3);
	EXPECT_TRUE(!strcmp(test_list.head->next->next->next->next->data.Word, "4th"));
	EXPECT_EQ(test_list.head->next->next->next->next->data.Key, 4);
}

TEST(Fill, FileisEmpty_ExpextedReturningNULL) {
	const char* filename = "Empty.txt";
	List_t* test_list = Fill(filename);
	EXPECT_TRUE(test_list == NULL);
}