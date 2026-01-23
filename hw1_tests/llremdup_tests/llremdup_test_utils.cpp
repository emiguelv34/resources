
#include "llremdup_test_utils.h"

#include <iostream>
#include <algorithm>

std::vector<int> removeDuplicates(const std::vector<int>& content)
{
  std::vector<int> vec(content);
  vec.erase(
	    std::unique(vec.begin(), vec.end()),
	    vec.end());
  return vec;
}

Item * makeList(std::vector<int> const & content)
{
	Item * head = nullptr;

	if(!content.empty())
	{
		// create head
		head = new Item(content[0], nullptr);

		// create rest of list
		Item * currTail = head;
		std::vector<int>::const_iterator nextValIter(content.cbegin());
		++nextValIter;

		for(; nextValIter != content.cend(); ++nextValIter)
		{
			Item * newItem = new Item(*nextValIter, nullptr);
			currTail->next = newItem;

			currTail = newItem; 
		}
	}

	return head;

}


testing::AssertionResult checkContent(Item * list, std::vector<int> const & content)
{

  size_t cnt = 0;
  Item* temp = list;
  while(temp != nullptr) {
    cnt++;
    temp = temp->next;
  }
  
  if(cnt != content.size()){
    return testing::AssertionFailure() << "Expected list of size " << content.size() << " but actual list size is " << cnt;
  }
  size_t ei = 0;
  while(list != nullptr) {
    if(content[ei] != list->val){
      return testing::AssertionFailure() << "Data mismatch: expected " << content[ei] << " / actual " << list->val;
    }
    ei++;
    list = list->next;
  }
  
  return testing::AssertionSuccess();
}


void deleteList(Item * list)
{
	Item * currElement = list;

	while(currElement != nullptr)
	{
		Item * nextElement = currElement->next;

		delete currElement;

		currElement = nextElement;
	}
}
