#include "stones.h"
#include "max_heap.h"

int lastStoneWeight(std::vector<int>& stones) {
    MaxHeap<int> heap;
    for (int w : stones) {
        heap.push(w);
    }
    while (true) {
        if (heap.size() == 0) {
            return 0;
        }
        if (heap.size() == 1) {
            return heap.top();
        }
        int heavier = heap.top();
        heap.pop();
        int lighter = heap.top();
        heap.pop();
        int remain = heavier - lighter;
        if (remain > 0) {
            heap.push(remain);
        }
    }
}
