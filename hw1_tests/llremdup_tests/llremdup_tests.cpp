//
// Test suite for HW2 Duplicates
//

#include "llremdup_test_utils.h"

#include <random_generator.h>

TEST(Duplicates, Empty)
{
	Item* list = nullptr;

	removeConsecutive(list);

	EXPECT_EQ(nullptr, list);
}

TEST(Duplicates, OneValueNoDuplicate)
{
	Item* list = makeList({2});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {2}));

	deleteList(list);
}

TEST(Duplicates, OneValueOneDuplicate)
{
        Item* list = makeList({1, 1});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1}));

	deleteList(list);
}

TEST(Duplicates, TwoValuesNoDuplicate)
{
	Item* list = makeList({7, 8});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {7,8}));

	deleteList(list);
}

TEST(Duplicates, TwoValueOneDuplicateA)
{
	Item* list = makeList({1, 1, 2});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2}));

	deleteList(list);
}

TEST(Duplicates, TwoValueOneDuplicateB)
{
	Item* list = makeList({1, 2, 2});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2}));

	deleteList(list);
}

TEST(Duplicates, ThreeValuesDuplicateStart)
{
        Item* list = makeList({1, 1, 2, 3});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2, 3}));

	deleteList(list);
}

TEST(Duplicates, ThreeValuesDuplicateMid)
{
        Item* list = makeList({1, 2, 2, 3});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2, 3}));

	deleteList(list);
}

TEST(Duplicates, ThreeValuesDuplicateEnd)
{
        Item* list = makeList({1, 2, 3, 3});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2, 3}));

	deleteList(list);
}

TEST(Duplicates, TenNumbers)
{
        Item* list = makeList({1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1}));

	deleteList(list);
}

TEST(Duplicates, ManyDuplicateSequences)
{
        Item* list = makeList({1, 1, 1, -2, -2, -2, 333333, 333333, 333333, 333333, 333333});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, -2, 333333}));

	deleteList(list);
}



TEST(Duplicates, DuplicatesMixed)
{
        Item* list = makeList({1, 2, 3, 1, 2, 3, 1, 2, 3});

	removeConsecutive(list);

	EXPECT_TRUE(checkContent(list, {1, 2, 3, 1, 2, 3, 1, 2, 3}));

	deleteList(list);
}

TEST(Duplicates, RandomStress500x10ele)
{
	const size_t numElements = 500;
	const size_t numTrials = 10;
	const RandomSeed masterSeed = 279;

	std::vector<RandomSeed> trialSeeds = makeRandomSeedVector(numTrials, masterSeed);

	for(RandomSeed seed : trialSeeds)
	{
  	        std::vector<int> data = makeRandomNumberVector<int>(numElements, 1, 10, seed, true);

		Item* list = makeList(data);

		removeConsecutive(list);

		// get the expected values
		std::vector<int> expected = removeDuplicates(data);
		

		EXPECT_TRUE(checkContent(list, expected));

		deleteList(list);
	}

}

TEST(Concatenate, BothEmpty)
{
        Item* list1 = nullptr;
        Item* list2 = nullptr;

	Item* list = concatenate(list1, list2);

	EXPECT_EQ(nullptr, list);
}

TEST(Concatenate, FirstEmptyOneVal)
{
        Item* list1 = nullptr;
        Item* list2 = makeList({1});

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list2->val = -1;
	
	EXPECT_TRUE(checkContent(list, {1}));

	deleteList(list2);
	deleteList(list);
}

TEST(Concatenate, SecondEmptyOneVal)
{
        Item* list1 = makeList({1});
        Item* list2 = nullptr;

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list1->val = -1;
	
	EXPECT_TRUE(checkContent(list, {1}));

	deleteList(list1);
	deleteList(list);
}

TEST(Concatenate, BothNonEmptyOneVal)
{
        Item* list1 = makeList({1});
        Item* list2 = makeList({2});

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list1->val = -1;
	list2->val = -2;
	
	EXPECT_TRUE(checkContent(list, {1, 2}));

	deleteList(list1);
	deleteList(list2);
	deleteList(list);
}

TEST(Concatenate, FirstEmptyMultipleVals)
{
        Item* list1 = nullptr;
        Item* list2 = makeList({1, 1, 1});

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list2->val = -1;
	
	EXPECT_TRUE(checkContent(list, {1, 1, 1}));

	deleteList(list2);
	deleteList(list);
}

TEST(Concatenate, SecondEmptyMultipleVals)
{
        Item* list1 = makeList({1, 1, 1});
        Item* list2 = nullptr;

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list1->val = -1;
	
	EXPECT_TRUE(checkContent(list, {1, 1, 1}));

	deleteList(list1);
	deleteList(list);
}

TEST(Concatenate, BothNonEmptyMultipleVals)
{
        Item* list1 = makeList({1, 1, 1});
        Item* list2 = makeList({2, 2, 2});

	Item* list = concatenate(list1, list2);

	// change list2 to ensure deep copy
	list1->val = -1;
	list2->val = -2;
	
	EXPECT_TRUE(checkContent(list, {1, 1, 1, 2, 2, 2}));

	deleteList(list1);
	deleteList(list2);
	deleteList(list);
}
