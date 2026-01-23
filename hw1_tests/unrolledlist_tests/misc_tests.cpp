//
// Miscellaneous tests for UnrolledLinkedList
//

#include "list_utils.h"

TEST(UnrolledList, ConstructionDestruction)
{
	ULListStr list;
}

TEST(UnrolledList, NoElements)
{
	ULListStr list;

	EXPECT_TRUE(list.empty());
	EXPECT_EQ(0, list.size());
}

TEST(UnrolledList, Front_PushBack)
{
	std::vector<std::string> content = {"sup", "er", "cal", "i", "frag", "il", "ist", "ic", "exp", "i", "alo", "doc", "ious"};

	ULListStr * list = makeList(content);

	EXPECT_EQ(list->front(), "sup");

	delete list;
}

TEST(UnrolledList, Front_PushFront)
{
	std::vector<std::string> content = {"sup", "er", "cal", "i", "frag", "il", "ist", "ic", "exp", "i", "alo", "doc", "ious"};

	ULListStr * list = makeList(content, false);

	EXPECT_EQ(list->front(), "sup");

	delete list;
}

TEST(UnrolledList, Back_PushBack)
{
	std::vector<std::string> content = {"sup", "er", "cal", "i", "frag", "il", "ist", "ic", "exp", "i", "alo", "doc", "ious"};

	ULListStr * list = makeList(content, false);

	EXPECT_EQ(list->back(), "ious");

	delete list;
}

TEST(UnrolledList, Back_PushFront)
{
	std::vector<std::string> content = {"sup", "er", "cal", "i", "frag", "il", "ist", "ic", "exp", "i", "alo", "doc", "ious"};

	ULListStr * list = makeList(content, false);

	EXPECT_EQ(list->back(), "ious");

	delete list;
}
