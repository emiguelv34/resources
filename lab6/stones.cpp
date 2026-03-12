#include "stones.h"
#include "max_heap.h"

int lastStoneWeight(std::vector<int>& stones) {
    MaxHeap<int> heap;
    
    // TO BE COMPLETED
    for (size_t i = 0; i < stones.size(); i++) {
        heap.push(stones[i]);
    }

    while (heap.size() > 1) {
        int y = heap.top(); heap.pop(); // heaviest
        int x = heap.top(); heap.pop(); // second heaviest

        if (x != y) {
            heap.push(y - x);
        }
    }
    if(heap.size() == 1){
        return heap.top();
    }
    return 0;
}
