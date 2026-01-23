//
// Tests for UnrolledLinkedList's pop operations
//

#include "list_utils.h"

#include <random_generator.h>

TEST(ListSet, OneItem)
{
	ULListStr list;

	list.push_back("a");

	EXPECT_EQ("a", list.get(0));

	list.set(0, "z");

	EXPECT_EQ("z", list.get(0));
}

std::vector<std::string> quickBrownFox = {"The", "quick", "brown", "fox", "jumped", "over", "the", "lazy", "dog", "."};

TEST(ListSet, First)
{
	ULListStr * list = makeList(quickBrownFox);

	list->set(0, "A");
	EXPECT_EQ("A", list->get(0));

	delete list;
}

TEST(ListSet, Last)
{
	ULListStr * list = makeList(quickBrownFox);

	list->set(9, "!");
	EXPECT_EQ("!", list->get(9));

	delete list;
}

TEST(ListSet, Middle)
{
	ULListStr * list = makeList(quickBrownFox);

	list->set(4, "perambulated");
	EXPECT_EQ("perambulated", list->get(4));

	delete list;
}

TEST(ListSet, 50x50RandomElements)
{
	const size_t numElements = 50;
	const size_t numTrials = 30;
	const RandomSeed origSeed = 9009;
	const size_t stringLength = 8;

	// generate list of random seeds and iterate through them
	for (RandomSeed seed : makeRandomSeedVector(numTrials, origSeed))
	{
		std::vector<std::string> contents(makeRandomAlphaStringVector(numElements, seed, stringLength, true));

		ULListStr *list = makeList(contents);

		for(size_t index = 0; index < numElements; index++)
		{
			list->set(index, std::to_string(index));
		}

		for(size_t index = 0; index < numElements; index++)
		{
			EXPECT_EQ(std::to_string(index), list->get(index));
		}


		delete list;
	}

}