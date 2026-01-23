//
// Auto-checker for HW1 Teams
//

#ifndef CS104_HW4_TEST_SUITE_LLREMDUP_TEST_UTILS_H
#define CS104_HW4_TEST_SUITE_LLREMDUP_TEST_UTILS_H

#include <rem_dup_lib.h>

#include <gtest/gtest.h>

#include <vector>
#include <limits>
#include <cstdlib>

// Utility functions for testing HW4 Problem 2


// generate a vector that has removed consecutive duplicates
std::vector<int> removeDuplicates(const std::vector<int>& content);

// build a list from the given vector of content
Item * makeList(std::vector<int> const & content);

// check that a list has the given values, even if they're in a different order
// NOTE: runs in O(nlogn), is able to handle duplicate values properly
testing::AssertionResult checkContent(Item * list, std::vector<int> const & content);

// check that the given list contains no duplicates
//bool checkNoDuplicates(Item * list);

// delete a list that was created by makeList()
void deleteList(Item * list);

#endif //CS104_HW2_TEST_SUITE_SPLIT_TEST_UTILS_H
