#include "pch.h"
#include "LabE.h"

TEST(OneNodeTree, ExpectRightFill) {
	tree_t* Node = TreeInit();
	if (!Node)
		ASSERT_TRUE(Node);
	Node->String = "asdfer";
	FillWidth(Node);
	EXPECT_EQ(Node->SubTree, 6);
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

TEST(ComparingWithExpectFile, ExpectMatchingSymbols) {
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
	EXPECT_TRUE(CompareFilesTest(Node1, "expect.txt", "result.txt") == 1);
	TreeDestroy(Node1);
}