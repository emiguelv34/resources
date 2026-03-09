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
    std::swap(data.front(), data.back());
    data.pop_back();
    if (data.empty()) {
        return;
    }
    std::size_t index = 0;
    while (true) {
        std::size_t left_index = index * 2 + 1;
        std::size_t right_index = left_index + 1;
        T& current_item = data[index];
        size_t max_index = index;
        if (left_index < data.size()) {
            max_index = left_index;
        }
        if (right_index < data.size() && data[right_index] > data[left_index]) {
            max_index = right_index;
        }
        if (max_index == index || data[max_index] <= current_item) {
            break;
        }
        std::swap(current_item, data[max_index]);
        index = max_index;
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
