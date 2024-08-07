#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <forward_list>
#include <list>
#include <ranges>
#include <algorithm>

#include "../lib/adapters.h"




TEST(Drop, Empty) {
    const int shift = 3;
    std::forward_list<int> data;
    std::vector<int> my_res;
    for (auto x: data | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Drop, ForwardIterator) {
    const int shift = 3;
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Drop, RandomAccessIterator) {
    const int shift = 3;
    std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Drop, LargeShiftForwardIterator) {
    const int shift = 100;
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Drop, LargeShiftRandomAccess) {
    const int shift = 100;
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Drop, AlgorithmStd) {
    const int shift = 5;
    std::vector<int> my_data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    auto std_data = my_data;
    auto my_view = my_data | Drop(shift);
    std::sort(my_view.begin(), my_view.end());
    auto std_view = std_data | std::views::drop(shift);
    std::sort(std_view.begin(), std_view.end());
    ASSERT_EQ(my_data, std_data);
}

TEST(Filter, Empty) {
    auto predicate = [](int x){return x == 1;};
    std::forward_list<int> data;
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::filter(predicate)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Filter, ForwardIterator) {
    auto predicate = [](int x){return x % 2 == 0;};
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::filter(predicate)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Filter, BidirectionalIterator) {
    auto predicate = [](int x){return x % 2 == 0;};
    std::list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Filter(predicate);
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Filter, RandomAccessIterator) {
    auto predicate = [](int x){return x % 2 == 0;};
    std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Filter(predicate);
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Filter, ConstContainer) {
    auto predicate = [](int x){return x % 2 == 0;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::filter(predicate)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Filter, AlgorithmStd) {
    auto predicate = [](int x){return x % 2 == 0;};
    std::vector<int> my_data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 2, 2, 1, 1};
    std::vector<int> real_data = my_data;
    auto my_view = my_data | Filter(predicate);
    std::reverse(my_view.begin(), my_view.end());
    auto std_view = real_data | std::views::filter(predicate);
    std::reverse(std_view.begin(), std_view.end());
    ASSERT_EQ(my_data, real_data);
}

TEST(Reverse, Empty) {
    std::list<int> data;
    std::vector<int> my_res;
    for (auto x: data | Reverse()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::reverse) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Reverse, BidirectionalIterator) {
    std::list<int> data = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> my_res;
    for (auto x: data | Reverse()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::reverse) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Reverse, RandomAccessIterator) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> my_res;
    for (auto x: data | Reverse()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::reverse) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Reverse, AlgorithmStd) {
    std::vector<int> data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 2, 2, 2, 2};
    auto my_view = data | Reverse();
    auto my_res = std::count(my_view.begin(), my_view.end(), 2);
    auto std_view = data | std::views::reverse;
    auto real_res = std::count(std_view.begin(), std_view.end(), 2);
    ASSERT_EQ(my_res, real_res);
}

TEST(Transform, Empty) {
    auto trans = [](int x){return x * 2;};
    std::list<int> data;
    std::vector<int> my_res;
    for (auto x: data | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(trans)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Transform, ForwardIterator) {
    auto trans = [](int x){return x * 2;};
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(trans)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Transform, BidirectionalIterator) {
    auto trans = [](int x){return x * 2;};
    std::list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Transform(trans);
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Transform, RandomAccessIterator) {
    auto trans = [](int x){return x * 2;};
    std::vector<int> data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto my_it = (data | Transform(trans)).begin();
    auto range_it = (data | std::views::transform(trans)).begin();
    ASSERT_EQ(my_it[0], range_it[0]);
    ASSERT_EQ(my_it[5], range_it[5]);
    ASSERT_EQ(my_it[10], range_it[10]);
    std::vector<int> my_res;
    for (auto x: data | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(trans)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Transform, ConstContainer) {
    auto trans = [](const int& x){return x * 2;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(trans)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Transform, AlgorithmStd) {
    auto trans = [](int x){return x * 2;};
    std::vector<int> data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 2, 2, 2, 2};
    auto my_view = data | Transform(trans);
    auto my_res = std::count(my_view.begin(), my_view.end(), 2);
    auto std_view = data | std::views::transform(trans);
    auto real_res = std::count(std_view.begin(), std_view.end(), 2);
    ASSERT_EQ(my_res, real_res);
}

TEST(Take, Empty) {
    size_t n = 3;
    std::list<int> data;
    std::vector<int> my_res;
    for (auto x: data | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::take(n)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Take, ForwardIterator) {
    size_t n = 3;
    std::forward_list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::take(n)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Take, BidirectionalIterator) {
    size_t n = 3;
    std::list<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Take(n);
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Take, RandomAccessIterator) {
    size_t n = 9;
    std::vector<int> data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto my_it = (data | Take(n)).begin();
    auto range_it = (data | std::views::take(n)).begin();
    ASSERT_EQ(my_it[0], range_it[0]);
    ASSERT_EQ(my_it[5], range_it[5]);
    ASSERT_EQ(my_it[8], range_it[8]);
    std::vector<int> my_res;
    for (auto x: data | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::take(n)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Take, ConstContainer) {
    size_t n = 3;
    const std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::vector<int> my_res;
    for (auto x: data | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::take(n)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Take, AlgorithmStd) {
    size_t n = 3;
    std::vector<int> data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 2, 2, 2, 2};
    auto my_view = data | Take(n);
    auto my_res = std::count(my_view.begin(), my_view.end(), 2);
    auto std_view = data | std::views::take(n);
    auto real_res = std::count(std_view.begin(), std_view.end(), 2);
    ASSERT_EQ(my_res, real_res);
}

TEST(Values, Empty) {
    std::forward_list<std::pair<int, int>> data;
    std::vector<int> my_res;
    for (auto x: data | Values()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::values) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Values, ForwardIterator) {
    std::forward_list<std::pair<int, int>> data{{1, 2}, {3, 4}, {5, 6}};
    std::vector<int> my_res;
    for (auto x: data | Values()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::values) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Values, BidirectionalIterator) {
    std::map<int, int> data{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    std::vector<int> my_res;
    for (auto x: data | Values()) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Values();
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Values, ConstContainer) {
    const std::map<int, int> data{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    std::vector<int> my_res;
    for (auto x: data | Values()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::values) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Values, AlgorithmStd) {
    std::map<int, int> data{{1, 2}, {3, 4}, {5, 2}, {7, 8}, {9, 2}};
    auto my_view = data | Values();
    auto my_res = std::count(my_view.begin(), my_view.end(), 2);
    auto std_view = data | std::views::values;
    auto real_res = std::count(std_view.begin(), std_view.end(), 2);
    ASSERT_EQ(my_res, real_res);
}

TEST(Keys, BidirectionalIterator) {
    std::map<int, int> data{{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    std::vector<int> my_res;
    for (auto x: data | Keys()) {
        my_res.push_back(x);
    }
    std::vector<int> my_res_reverse;
    auto my_range = data | Keys();
    for (auto it = std::reverse_iterator(my_range.end()); it != std::reverse_iterator(my_range.begin()); ++it) {
        my_res_reverse.push_back(*it);
    }
    ASSERT_TRUE(std::ranges::equal(my_res, my_res_reverse | std::views::reverse));
}

TEST(Keys, RandomAccessIterator) {
    std::vector<std::tuple<int, int, int>> data{{1, 2, 3}, {3, 4, 10}, {5, 6, 11}, {7,  12, 14}, {9, 10, 13}};
    auto my_it = (data | Keys()).begin();
    auto range_it = (data | std::views::keys).begin();
    ASSERT_EQ(my_it[0], range_it[0]);
    ASSERT_EQ(my_it[5], range_it[5]);
    ASSERT_EQ(my_it[10], range_it[10]);
    std::vector<int> my_res;
    for (auto x: data | Keys()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::keys) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, ValuesTransform) {
    auto transform = [](int x){return x * 2 + 7;};
    std::map<int, int> data{{1, 2}, {3, 4} , {5, 2}, {7, 8}, {9, 2}};
    std::vector<int> my_res;
    for (auto x: data | Values() | Transform(transform)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::values | std::views::transform(transform)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, TransformValues) {
    auto transform = [](std::pair<const int, int> x)
            {return std::pair<const int, int>(x.first * 2 + 7, x.second * 3 + 2);
    };
    std::map<int, int> data{{1, 2}, {3, 4} , {5, 2}, {7, 8}, {9, 2}};
    std::vector<int> my_res;
    for (auto x: data | Transform(transform) | Values()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(transform) | std::views::values) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, ValuesTake) {
    size_t n = 3;
    std::map<int, int> data{{1, 2}, {3, 4} , {5, 2}, {7, 8}, {9, 2}};
    std::vector<int> my_res;
    for (auto x: data | Values() | Take(n)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::values | std::views::take(n))  {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, DropFilter) {
    const size_t shift = 2;
    auto predicate = [](int x){return x % 2 == 0;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> my_res;
    for (auto x: data | Drop(shift) | Filter(predicate)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::drop(shift) | std::views::filter(predicate)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, FilterDrop) {
    const size_t shift = 2;
    auto predicate = [](int x){return x % 2 == 0;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> my_res;
    for (auto x: data | Filter(predicate) | Drop(shift)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::filter(predicate) | std::views::drop(shift)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, ReverseTransform) {
    auto trans = [](int x){return x * 2;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> my_res;
    for (auto x: data | Reverse() | Transform(trans)) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::reverse | std::views::transform(trans)) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}

TEST(Conveyor, TransformReverse) {
    auto trans = [](int x){return x * 2;};
    const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> my_res;
    for (auto x: data | Transform(trans) | Reverse()) {
        my_res.push_back(x);
    }
    std::vector<int> real_res;
    for (auto x: data | std::views::transform(trans) | std::views::reverse) {
        real_res.push_back(x);
    }
    ASSERT_EQ(my_res, real_res);
}