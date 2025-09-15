#include <gtest/gtest.h>
#include "../Container.h"

using namespace stl_containers;

class CircularListTest : public ::testing::Test {
protected:
    CircularList<int> list;
};

// --- Constructors ---
TEST_F(CircularListTest, DefaultConstructor) {
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

TEST_F(CircularListTest, InitializerListConstructor) {
    CircularList<int> l{1, 2, 3};
    EXPECT_EQ(l.size(), 3u);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST_F(CircularListTest, CopyConstructor) {
    list = {10, 20};
    CircularList<int> l2(list);
    EXPECT_EQ(l2.size(), 2u);
    EXPECT_EQ(l2.front(), 10);
    EXPECT_EQ(l2.back(), 20);
}

TEST_F(CircularListTest, MoveConstructor) {
    list = {1, 2};
    CircularList<int> l2(std::move(list));
    EXPECT_EQ(l2.size(), 2u);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 2);
}

TEST_F(CircularListTest, CopyAssignment) {
    list = {1, 2, 3};
    CircularList<int> l2;
    l2 = list;
    EXPECT_EQ(l2.size(), 3u);
    EXPECT_EQ(l2.front(), 1);
    EXPECT_EQ(l2.back(), 3);
}

TEST_F(CircularListTest, MoveAssignment) {
    list = {5, 6, 7};
    CircularList<int> l2;
    l2 = std::move(list);
    EXPECT_EQ(l2.size(), 3u);
    EXPECT_EQ(l2.front(), 5);
    EXPECT_EQ(l2.back(), 7);
}

// --- push/pop ---
TEST_F(CircularListTest, PushBack) {
    list.push_back(10);
    EXPECT_EQ(list.size(), 1u);
    EXPECT_EQ(list.back(), 10);
}

TEST_F(CircularListTest, PushFront) {
    list.push_back(20);
    list.push_front(10);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 20);
}

TEST_F(CircularListTest, PopBack) {
    list = {1, 2, 3};
    list.pop_back();
    EXPECT_EQ(list.back(), 2);
    EXPECT_EQ(list.size(), 2u);
}

TEST_F(CircularListTest, PopFront) {
    list = {1, 2, 3};
    list.pop_front();
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2u);
}

TEST_F(CircularListTest, PopFrontThrowsWhenEmpty) {
    EXPECT_THROW(list.pop_front(), std::out_of_range);
}

TEST_F(CircularListTest, PopBackThrowsWhenEmpty) {
    EXPECT_THROW(list.pop_back(), std::out_of_range);
}

// --- insert/emplace/erase ---
TEST_F(CircularListTest, InsertByCopy) {
    list = {2, 3};
    auto it = list.insert(list.begin(), 1);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(list.front(), 1);
}

TEST_F(CircularListTest, InsertByMove) {
    list = {1, 3};
    int x = 2;
    auto it = list.insert(++list.begin(), std::move(x));
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(list.size(), 3u);
}

TEST_F(CircularListTest, Emplace) {
    auto it = list.emplace(list.end(), 42);
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(list.back(), 42);
}

TEST_F(CircularListTest, EraseSingleElement) {
    list = {1, 2, 3};
    auto it = list.erase(++list.begin());
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(list.size(), 2u);
}

TEST_F(CircularListTest, EraseThrowsOnEmpty) {
    EXPECT_THROW(list.erase(list.begin()), std::out_of_range);
}

TEST_F(CircularListTest, EraseThrowsOnEndIterator) {
    list.push_back(1);
    EXPECT_THROW(list.erase(list.end()), std::out_of_range);
}

// --- clear ---
TEST_F(CircularListTest, Clear) {
    list = {1, 2, 3};
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

// --- front/back ---
TEST_F(CircularListTest, FrontAndBack) {
    list = {10, 20, 30};
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 30);
}

TEST_F(CircularListTest, FrontThrowsIfEmpty) {
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST_F(CircularListTest, BackThrowsIfEmpty) {
    EXPECT_THROW(list.back(), std::out_of_range);
}

// --- size/empty ---
TEST_F(CircularListTest, SizeAndEmpty) {
    EXPECT_TRUE(list.empty());
    list.push_back(1);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1u);
}

// --- iterators ---
TEST_F(CircularListTest, ForwardIteration) {
    list = {1, 2, 3};
    std::vector<int> values;
    for (auto it = list.begin(); it != list.end(); ++it) {
        values.push_back(*it);
    }
    EXPECT_EQ(values, (std::vector<int>{1, 2, 3}));
}

TEST_F(CircularListTest, ReverseIteration) {
    list = {1, 2, 3};
    std::vector<int> values;
    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        values.push_back(*it);
    }
    EXPECT_EQ(values, (std::vector<int>{3, 2, 1}));
}

TEST_F(CircularListTest, IteratorDereferenceThrowsOnEnd) {
    list.push_back(42);
    auto it = list.end();
    EXPECT_THROW(*it, std::out_of_range);
    EXPECT_THROW(it.operator->(), std::out_of_range);
}

TEST_F(CircularListTest, ConstIterator) {
    list = {7, 8, 9};
    const auto& clist = list;
    auto it = clist.begin();
    EXPECT_EQ(*it, 7);
    ++it;
    EXPECT_EQ(*it, 8);
}

TEST_F(CircularListTest, IteratorIncrementAndDecrement) {
    list = {1, 2, 3};
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
    --it;
    EXPECT_EQ(*it, 2);
    it--;
    EXPECT_EQ(*it, 1);
}

TEST_F(CircularListTest, IteratorEqualityAndInequality) {
    list = {1};
    auto it1 = list.begin();
    auto it2 = list.begin();
    auto it3 = list.end();
    EXPECT_TRUE(it1 == it2);
    EXPECT_TRUE(it1 != it3);
}
