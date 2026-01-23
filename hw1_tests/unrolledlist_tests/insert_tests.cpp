//
// Insertion tests for UnrolledLinkedList
//

#include "list_utils.h"

#include <random_generator.h>

// add one item and check that it's there
TEST(ListInsertBack, OneItemAdd)
{
	ULListStr list;

	list.push_back("a");

	EXPECT_EQ("a", list.get(0));
	EXPECT_EQ(false, list.empty());
}

TEST(ListInsertFront, OneItemAdd)
{
	ULListStr list;

	list.push_front("a");

	EXPECT_EQ("a", list.get(0));
	EXPECT_EQ(false, list.empty());
}


TEST(ListInsert, EmptyString)
{
	ULListStr list;

	list.push_back("");

	EXPECT_EQ(1, list.size());
	EXPECT_EQ("", list.get(0));
}

// add three items and check that they're there
TEST(ListInsertBack, ThreeItemAdd)
{
	// first we create a vector to hold our data
	std::vector<std::string> contents{"768", "1024", "1536"};

	// then we create a new list with that data
	ULListStr * populatedList = makeList(contents);

	// then we assert that the list contains that data
	EXPECT_TRUE(checkListContent(populatedList, contents));

	delete populatedList;
}

// add three items and check that they're there
TEST(ListInsertFront, ThreeItemAdd)
{
	// first we create a vector to hold our data
	std::vector<std::string> contents{"768", "1024", "1536"};

	// then we create a new list with that data
	ULListStr * populatedList = makeList(contents, false);

	// then we assert that the list contains that data
	EXPECT_TRUE(checkListContent(populatedList, contents));

	delete populatedList;
}

TEST(ListInsert, FrontThenBack)
{
	ULListStr list;

	list.push_front("fred");
	list.push_back("bob");

	EXPECT_TRUE(checkListContent(&list, {"fred", "bob"}));
}

TEST(ListInsert, BackThenFront)
{
	ULListStr list;

	list.push_back("bob");
	list.push_front("fred");

	EXPECT_TRUE(checkListContent(&list, {"fred", "bob"}));
}

TEST(ListInsertBack, ARRSIZEPlusOne)
{
	std::vector<std::string> contents = makeRandomAlphaStringVector(ARRSIZE + 1, 574, 16, false);

	ULListStr * populatedList = makeList(contents);

	EXPECT_TRUE(checkListContent(populatedList, contents));

	delete populatedList;
}


TEST(ListInsertFront, ARRSIZEPlusOne)
{
	std::vector<std::string> contents = makeRandomAlphaStringVector(ARRSIZE + 1, 37, 16, false);

	ULListStr * populatedList = makeList(contents, false);

	EXPECT_TRUE(checkListContent(populatedList, contents));

	delete populatedList;
}

TEST(ListInsertBack, 50RandomElements)
{
	const size_t numElements = 50;
	const size_t numTrials = 30;
	const RandomSeed origSeed = 70;
	const size_t stringLength = 8;

	// generate list of random seeds and iterate through them
	for (RandomSeed seed : makeRandomSeedVector(numTrials, origSeed))
	{
		std::vector<std::string> contents(makeRandomAlphaStringVector(numElements, seed, stringLength, true));

		ULListStr *list = makeList(contents);

		EXPECT_TRUE(checkListContent(list, contents));

		delete list;
	}

}

TEST(ListInsertFront, 50RandomElements)
{
	const size_t numElements = 50;
	const size_t numTrials = 30;
	const RandomSeed origSeed = 689;
	const size_t stringLength = 8;

	// generate list of random seeds and iterate through them
	for (RandomSeed seed : makeRandomSeedVector(numTrials, origSeed))
	{
		std::vector<std::string> contents(makeRandomAlphaStringVector(numElements, seed, stringLength, true));

		ULListStr *list = makeList(contents, false);

		EXPECT_TRUE(checkListContent(list, contents));

		delete list;
	}

}

TEST(ListInsertBack, 5x1000RandomElements)
{
	const size_t numElements = 1000;
	const size_t numTrials = 30;
	const RandomSeed origSeed = 463948;
	const size_t stringLength = 10;

	// generate list of random seeds and iterate through them
	for (RandomSeed seed : makeRandomSeedVector(numTrials, origSeed))
	{
		std::vector<std::string> contents(makeRandomAlphaStringVector(numElements, seed, stringLength, true));

		ULListStr *list = makeList(contents);

		EXPECT_TRUE(checkListContent(list, contents));

		delete list;
	}

}

TEST(ListInsertFront, 5x1000RandomElements)
{
	const size_t numElements = 1000;
	const size_t numTrials = 30;
	const RandomSeed origSeed = 232;
	const size_t stringLength = 10;

	// generate list of random seeds and iterate through them
	for (RandomSeed seed : makeRandomSeedVector(numTrials, origSeed))
	{
		std::vector<std::string> contents(makeRandomAlphaStringVector(numElements, seed, stringLength, true));

		ULListStr *list = makeList(contents, false);

		EXPECT_TRUE(checkListContent(list, contents));

		delete list;
	}

}