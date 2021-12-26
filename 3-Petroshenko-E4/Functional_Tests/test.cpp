#include "pch.h"
#include "LabE.h"

int CompareFilesTest(tree_t* Root, char* ExpectedFile, char* ResultFile) {
	char ExpectedSymbol, ResultSymbol;
	FILE* Result = fopen(ResultFile, "w");
	if (!Result)
		return -1;
	TreePrint(Root, 0, Result);
	fclose(Result);
	FILE* Expect = fopen(ExpectedFile, "r");
	if (!Expect)
		return -1;
	Result = fopen(ResultFile, "r");
	if (!Result) {
		fclose(Expect);
		return -1;
	}
	while (fscanf(Expect, "%c", &ExpectedSymbol) != EOF && fscanf(Result, "%c", &ResultSymbol) != EOF) {
		if (ExpectedSymbol != ResultSymbol) {
			fclose(Expect);
			fclose(Result);
			return 0;
		}
	}
	fclose(Expect);
	fclose(Result);
	return 1;
}

TEST(OneNodeTree, ExpectRightFill) {
	tree_t* Node = TreeInit();
	if (!Node)
		ASSERT_TRUE(Node);
	Node->String = "primary";
	FillWidth(Node);
	EXPECT_EQ(Node->SubTree, 7);
	TreeDestroy(Node);
}

TEST(TwoNodeLeftTree, ExpectRightFill) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	Node1->String = "primary";
	Node2->String = "left";
	Node1->Left = Node2;
	FillWidth(Node1);
	EXPECT_EQ(Node1->SubTree, 11);
	EXPECT_EQ(Node2->SubTree, 4);
	TreeDestroy(Node1);
}

TEST(TwoNodeRightTree, ExpectRightFill) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	Node1->String = "primary";
	Node2->String = "right";
	Node1->Right = Node2;
	FillWidth(Node1);
	EXPECT_EQ(Node1->SubTree, 12);
	EXPECT_EQ(Node2->SubTree, 5);
	TreeDestroy(Node1);
}

TEST(TwoLevelFullTree, ExpectRightFill) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	tree_t* Node3 = TreeInit();
	if (!Node3) {
		free(Node1);
		free(Node2);
		ASSERT_TRUE(Node3);
	}
	Node1->String = "primary";
	Node2->String = "left";
	Node3->String = "right";
	Node1->Left = Node2;
	Node1->Right = Node3;
	FillWidth(Node1);
	EXPECT_EQ(Node1->SubTree, 16);
	EXPECT_EQ(Node2->SubTree, 4);
	EXPECT_EQ(Node3->SubTree, 5);
	TreeDestroy(Node1);
}

TEST(TwoLevelFullTreeWithIncorrectWidth, ExpectCorrectingWidth) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	tree_t* Node3 = TreeInit();
	if (!Node3) {
		free(Node1);
		free(Node2);
		ASSERT_TRUE(Node3);
	}
	Node1->String = "primary";
	Node1->SubTree = 1;
	Node2->String = "left";
	Node2->SubTree = 1;
	Node3->String = "right";
	Node3->SubTree = 1;
	Node1->Left = Node2;
	Node1->Right = Node3;
	FillWidth(Node1);
	EXPECT_EQ(Node1->SubTree, 16);
	EXPECT_EQ(Node2->SubTree, 4);
	EXPECT_EQ(Node3->SubTree, 5);
	TreeDestroy(Node1);
}

TEST(TwoLevelFullTreeWithCorrectWidth, ExpectNoChanges) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	tree_t* Node3 = TreeInit();
	if (!Node3) {
		free(Node1);
		free(Node2);
		ASSERT_TRUE(Node3);
	}
	Node1->String = "primary";
	Node1->SubTree = 16;
	Node2->String = "left";
	Node2->SubTree = 4;
	Node3->String = "right";
	Node3->SubTree = 5;
	Node1->Left = Node2;
	Node1->Right = Node3;
	FillWidth(Node1);
	EXPECT_EQ(Node1->SubTree, 16);
	EXPECT_EQ(Node2->SubTree, 4);
	EXPECT_EQ(Node3->SubTree, 5);
	TreeDestroy(Node1);
}

TEST(TwoLevelFullTree_ComparingWithExpectFile, ExpectMatchingSymbols) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	tree_t* Node3 = TreeInit();
	if (!Node3) {
		free(Node1);
		free(Node2);
		ASSERT_TRUE(Node3);
	}
	Node1->String = "primary";
	Node2->String = "left";
	Node3->String = "right";
	Node1->Left = Node2;
	Node1->Right = Node3;
	FillWidth(Node1);
	EXPECT_TRUE(CompareFilesTest(Node1, "expect1.txt", "result.txt") == 1);
	TreeDestroy(Node1);
}

TEST(TwoNodeLeftTree_ComparingWithExpectFile, ExpectMatchingSymbols) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	Node1->String = "primary";
	Node2->String = "left";
	Node1->Left = Node2;
	FillWidth(Node1);
	EXPECT_TRUE(CompareFilesTest(Node1, "expect2.txt", "result2.txt") == 1);
	TreeDestroy(Node1);
}

TEST(TwoNodeRightTree_ComparingWithExpectFile, ExpectMatchingSymbols) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	Node1->String = "primary";
	Node2->String = "right";
	Node1->Right = Node2;
	FillWidth(Node1);
	EXPECT_TRUE(CompareFilesTest(Node1, "expect3.txt", "result3.txt") == 1);
	TreeDestroy(Node1);
}

TEST(OneNodeTree_ComparingWithExpectFile, ExpectMatchingSymbols) {
	tree_t* Node1 = TreeInit();
	if (!Node1)
		ASSERT_TRUE(Node1);
	tree_t* Node2 = TreeInit();
	if (!Node2) {
		free(Node1);
		ASSERT_TRUE(Node2);
	}
	Node1->String = "primary";
	FillWidth(Node1);
	EXPECT_TRUE(CompareFilesTest(Node1, "expect4.txt", "result4.txt") == 1);
	TreeDestroy(Node1);
}