#include "dheap.h"
#include "gtest.h"

TEST(DHeapTest, PushAndTop) {
  DHeap<int> heap;
  heap.push(5);
  heap.push(3);
  heap.push(8);
  heap.push(1);

  EXPECT_EQ(heap.top(), 1);
}

TEST(DHeapTest, Pop) {
  DHeap<int> heap;
  heap.push(5);
  heap.push(3);
  heap.push(8);
  heap.push(1);

  heap.pop();
  EXPECT_EQ(heap.top(), 3);

  heap.pop();
  EXPECT_EQ(heap.top(), 5);

  heap.pop();
  EXPECT_EQ(heap.top(), 8);
}

TEST(DHeapTest, Empty) {
  DHeap<int> heap;
  EXPECT_TRUE(heap.empty());

  heap.push(1);
  EXPECT_FALSE(heap.empty());

  heap.pop();
  EXPECT_TRUE(heap.empty());
}

TEST(DHeapTest, Size) {
  DHeap<int> heap;
  EXPECT_EQ(heap.size(), 0u);

  heap.push(1);
  heap.push(2);
  heap.push(3);
  EXPECT_EQ(heap.size(), 3u);

  heap.pop();
  EXPECT_EQ(heap.size(), 2u);
}

TEST(DHeapTest, WithPairs) {
  DHeap<std::pair<int, int>, std::less<std::pair<int, int>>> heap;

  heap.push(std::make_pair(5, 1));
  heap.push(std::make_pair(3, 2));
  heap.push(std::make_pair(8, 3));
  heap.push(std::make_pair(1, 4));

  EXPECT_EQ(heap.top().first, 1);
  EXPECT_EQ(heap.top().second, 4);
  heap.pop();

  EXPECT_EQ(heap.top().first, 3);
  EXPECT_EQ(heap.top().second, 2);
  heap.pop();

  EXPECT_EQ(heap.top().first, 5);
  EXPECT_EQ(heap.top().second, 1);
  heap.pop();

  EXPECT_EQ(heap.top().first, 8);
  EXPECT_EQ(heap.top().second, 3);
}

TEST(DHeapTest, DifferentDegrees) {
  DHeap<int> heap(3); // 3-куча

  heap.push(10);
  heap.push(5);
  heap.push(7);
  heap.push(3);
  heap.push(1);

  EXPECT_EQ(heap.top(), 1);
  EXPECT_EQ(heap.getDegree(), 3);
}

TEST(DHeapTest, Clear) {
  DHeap<int> heap;
  heap.push(1);
  heap.push(2);
  heap.push(3);

  heap.clear();
  EXPECT_TRUE(heap.empty());
  EXPECT_EQ(heap.size(), 0u);
}

TEST(DHeapTest, PopOnEmptyThrows) {
  DHeap<int> heap;
  EXPECT_THROW(heap.pop(), std::out_of_range);
  EXPECT_THROW(heap.top(), std::out_of_range);
}