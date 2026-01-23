#ifndef CS104_HW2_TEST_SUITE_LIST_UTILS_H
#define CS104_HW2_TEST_SUITE_LIST_UTILS_H

#include <ulliststr.h>

#include <gtest/gtest.h>

#include <string>
#include <vector>

// Utilities for testing hw2's unrolled linked list.

// build a list from a vector of values.
// The second parameter determines whether it uses push_back or push_front
ULListStr * makeList(std::vector<std::string> const & values, bool pushBack = true);

// check that a list has the given content.
testing::AssertionResult checkListContent(ULListStr *const list, std::vector<std::string> const &expectedValues);

#endif //CS104_HW2_TEST_SUITE_LIST_UTILS_H
