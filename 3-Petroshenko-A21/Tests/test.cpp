#include "pch.h"
#include "LabA.h"

Node_t new_element;
List_t Test_List;
Node_t head;

TEST(Create_Element, ElementCreated_ExpectedEqualityofData) {
	new_element.data.Word = "test";
	new_element.data.Key = 1;
	new_element.next = NULL;
	Node_t* element2 = Create_Element("test", 1);
	EXPECT_TRUE(!strcmp(new_element.data.Word, element2->data.Word));
	EXPECT_EQ(new_element.data.Key, element2->data.Key);
	EXPECT_EQ(new_element.next, element2->next);
	free(element2->data.Word);
	free(element2);
}

TEST(ListCreate, ListCreated_ExpectedListHeadnotNULLanditsDataisRight) {
	List_t* Test_List = ListCreate("head", 1);
	EXPECT_TRUE(Test_List->head != NULL);
	EXPECT_TRUE(!strcmp(Test_List->head->data.Word, "head"));
	EXPECT_EQ(Test_List->head->data.Key, 1);
	ListDestroy(Test_List);
}

TEST(Search_place, TheSmallestKeyNumber_ExpectedreturnNULL) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
	int TestKey = 0;
	EXPECT_TRUE(Search_place(&Test_List, TestKey) == NULL);
}

TEST(Search_place, UsualKeyNumber_ExpectedreturnNotNULL) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
	int TestKey = 2;
	EXPECT_TRUE(Search_place(&Test_List, TestKey) != NULL);
}

TEST(Add, KeyNumbersAreEqual_ExpectedError) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
	new_element.data.Word = "error";
	new_element.data.Key = 1;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Error);
}

TEST(Add, AddingToTheBegining_ExpectedNewElementAsHeadofList) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
	new_element.data.Word = "zero";
	new_element.data.Key = 0;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Add_to_begining);
	EXPECT_TRUE(Test_List.head == &new_element);
}

TEST(Add, AddingNotToTheBegining_ExpectedNewElementAsHeadNext) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
	new_element.data.Word = "second";
	new_element.data.Key = 2;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Add_not_to_begining);
	EXPECT_TRUE(Test_List.head->next == &new_element);
}

TEST(Add, AddingElement_ExpectedAddingElementtotheRightPlace) {
	head.data.Word = "head";
	head.data.Key = 1;
	head.next = NULL;
	Test_List.head = &head;
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
	EXPECT_EQ(Add(&Test_List, &new_element), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element1), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element2), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element3), Add_to_begining);
	EXPECT_TRUE(Test_List.head->next->next->next == &new_element);
}