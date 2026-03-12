#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
class MaxHeap {
private:
    std::vector<T> data;
public:
    void push(const T& item);
    // Removes the largest element in the heap
    void pop();
    // Returns the largest element in the heap (but does not remove it)
    T const& top() const;
    std::size_t size() const;
};

template <typename T>
void MaxHeap<T>::push(const T& item) {
    data.push_back(item);
    std::size_t index = data.size() - 1;
    while (index != 0) {
        std::size_t parent_index = (index - 1) / 2;
        T& current = data[index];
        T& parent = data[parent_index];
        if (current <= parent) {
            break;
        }
        std::swap(current, parent);
        index = parent_index;
    }
}

template <typename T>
void MaxHeap<T>::pop() {
    if (data.empty()) {
        throw std::out_of_range("heap is empty");
    }

    // TO BE COMPLETETED
    std::swap(data[0], data[data.size()-1]);
    data.pop_back();

    std::size_t idx = 0;
    std::size_t left_child, right_child, largest;

    while(2 * idx + 1 < data.size()) { //0-th based indexing
        left_child = 2 * idx + 1; //left child(idx) = 2idx + 1
        right_child = 2 * idx + 2; //right child(idx) = 2idx + 2
        largest = idx;

        if (left_child < data.size() && data[left_child] > data[largest]) {
            largest = left_child;
        }
        if (right_child < data.size() && data[right_child] > data[largest]) {
            largest = right_child;
        }
        if (largest == idx) {
            break;
        }
        std::swap(data[idx], data[largest]);
        idx = largest;
    }
}

template <typename T>
T const& MaxHeap<T>::top() const {
    if (data.empty()) {
        throw std::out_of_range("heap is empty");
    }
    return data[0];
}

template <typename T>
std::size_t MaxHeap<T>::size() const {
    return data.size();
}
