#include "list_utils.h"

#include <iostream>
#include <sstream>

ULListStr * makeList(std::vector<std::string> const & values, bool pushBack)
{
	// circular list is not copyable so we have to pass it by pointer
	ULListStr * list = new ULListStr();

	if(pushBack)
	{
		for(std::vector<std::string>::const_iterator valuesIter = values.cbegin(); valuesIter != values.cend(); ++valuesIter)
		{
			list->push_back(*valuesIter);
		}
	}
	else
	{
		// push values in reverse from the front
		for(std::vector<std::string>::const_reverse_iterator valuesIter = values.crbegin(); valuesIter != values.crend(); ++valuesIter)
		{
			list->push_front(*valuesIter);
		}
	}

	return list;
}


testing::AssertionResult checkListContent(ULListStr *const list, std::vector<std::string> const &expectedValues)
{
	if(list->size() != expectedValues.size())
	{
		return testing::AssertionFailure() << "Size mismatch! Expected " << expectedValues.size() << " elements, got " << list->size() << " elements.";
	}

	// aggregate all mismatches into one failure message
	std::stringstream failureMessage;
	failureMessage << "List content mismatch: ";
	bool contentMismatch = false;

	for(size_t index = 0; index < list->size(); ++index)
	{
		if(list->get(index) != expectedValues[index])
		{
			failureMessage << "Expected value at index " << index << " was " << expectedValues[index] << ", but element was actually " << list->get(index) << std::endl;
			contentMismatch = true;
		}
	}

	if(contentMismatch)
	{
		return testing::AssertionFailure() << failureMessage.str();
	}

	return testing::AssertionSuccess();
}

