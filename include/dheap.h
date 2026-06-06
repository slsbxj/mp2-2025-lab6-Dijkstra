#ifndef DHEAP_H
#define DHEAP_H

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::less<T>> class DHeap {
private:
  std::vector<T> heap;
  int d;
  Compare comp;

  int parent(int i) const { return (i - 1) / d; }
  int child(int i, int k) const { return i * d + k + 1; }

  void siftUp(int i) {
    while (i > 0 && comp(heap[i], heap[parent(i)])) {
      std::swap(heap[i], heap[parent(i)]);
      i = parent(i);
    }
  }

  void siftDown(int i) {
    while (true) {
      int best = i;
      for (int k = 0; k < d; ++k) {
        int ch = child(i, k);
        if (ch < (int)heap.size() && comp(heap[ch], heap[best])) {
          best = ch;
        }
      }
      if (best == i)
        break;
      std::swap(heap[i], heap[best]);
      i = best;
    }
  }

public:
  DHeap(int degree = 2) : d(degree > 1 ? degree : 2) {}

  void push(const T &value) {
    heap.push_back(value);
    siftUp((int)heap.size() - 1);
  }

  void pop() {
    if (empty())
      throw std::out_of_range("Heap is empty");
    heap[0] = heap.back();
    heap.pop_back();
    if (!empty())
      siftDown(0);
  }

  const T &top() const {
    if (empty())
      throw std::out_of_range("Heap is empty");
    return heap[0];
  }

  bool empty() const { return heap.empty(); }
  size_t size() const { return heap.size(); }
  void clear() { heap.clear(); }
  void setDegree(int degree) {
    if (degree > 1)
      d = degree;
  }
  int getDegree() const { return d; }
};

#endif