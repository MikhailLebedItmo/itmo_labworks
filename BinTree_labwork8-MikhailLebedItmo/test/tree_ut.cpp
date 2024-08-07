#include "gtest/gtest.h"
#include <exception>
#include <gtest/gtest.h>
#include "../lib/tree.h"
#include <gmock/gmock.h>
#include <vector>

using pre_o = BinaryTree<int, WalkType::PreOrder>;
using post_o = BinaryTree<int, WalkType::PostOrder>;
using in_o = BinaryTree<int, WalkType::InOrder>;

TEST(Init, Full) {
    ASSERT_NO_THROW(pre_o({1, 2, 3, 4, 5}));
    ASSERT_NO_THROW(post_o({3, 2, 10, 4, 5}));
    ASSERT_NO_THROW(in_o({5, 4, 3, 2, 1}));
}

TEST(RangeFor, InOrdered) {
    in_o iotree{5, 1, 9, 10, -1, 15, 9, 33};
    std::vector<int> correct_io_res = {-1, 1, 5, 9, 10, 15, 33};
    std::vector<int> io_res;
    for (auto x: iotree) {
        io_res.push_back(x);
    }
    ASSERT_EQ(correct_io_res, io_res);
}

TEST(RangeFor, PostOrdered) {
    post_o post_o_tree{5, 3, 1, 4, 7, 10, 6};
    std::vector<int> correct_post_o_res = {1, 4, 3, 6, 10, 7, 5};
    std::vector<int> post_o_res;
    for (auto x: post_o_tree) {
        post_o_res.push_back(x);
    }
    ASSERT_EQ(correct_post_o_res, post_o_res);
}

TEST(RangeFor, PreOrdered) {
    pre_o pre_o_tree{5, 3, 1, 4, 7, 10, 6};
    std::vector<int> correct_pre_o_res = {5, 3, 1, 4, 7, 6, 10};
    std::vector<int> pre_o_res;
    for (auto x: pre_o_tree) {
        pre_o_res.push_back(x);
    }
    ASSERT_EQ(correct_pre_o_res, pre_o_res);
}

TEST(CopyConstructor, PreOrdered) {
    pre_o tree{-1, 3, 1, 4, 10, 6};
    const pre_o copy_tree(tree);
    ASSERT_EQ(tree, copy_tree);
}

TEST(CopyConstructor, InOrdered) {
    in_o tree{-1, 3, 1, 4, 10, 6, 20, 91, 11, 54, 12};
    const in_o copy_tree(tree);
    ASSERT_EQ(tree, copy_tree);
}

TEST(CopyConstructor, PostOrdered) {
    in_o tree{20, 91, 11, 54, 12};
    const in_o copy_tree(tree);
    ASSERT_EQ(tree, copy_tree);
}

TEST(InsertEraseFind, PreOrdered) {
    std::vector<int> data = {-1, 3, 1, 4, 10, 6};
    pre_o tree_1(data.begin(), data.end());
    pre_o tree_2;
    for (auto x: data) {
        tree_2.insert(x);
    }
    ASSERT_EQ(tree_2, tree_1);
    ASSERT_EQ(*tree_2.find(4), 4);
    tree_2.erase(4);
    ASSERT_EQ(tree_2.find(4), tree_2.end());
    ASSERT_EQ(*tree_2.find(1), 1);
    tree_2.erase(1);
    ASSERT_EQ(tree_2.find(1), tree_2.end());
}

TEST(InsertEraseFind, PostOrdered) {
    std::vector<int> data = {20, 1, 11, 54, 12};
    post_o tree_1(data.begin(), data.end());
    post_o tree_2;
    for (auto x: data) {
        tree_2.insert(x);
    }
    ASSERT_EQ(tree_2, tree_1);
    ASSERT_EQ(*tree_2.find(20), 20);
    tree_2.erase(20);
    ASSERT_EQ(tree_2.find(20), tree_2.end());
    ASSERT_EQ(*tree_2.find(11), 11);
    tree_2.erase(11);
    ASSERT_EQ(tree_2.find(11), tree_2.end());
}

TEST(InsertEraseFind, InOrdered) {
    std::vector<int> data = {20, 1, 11, 54, 12};
    post_o tree_1(data.begin(), data.end());
    post_o tree_2;
    for (auto x: data) {
        tree_2.insert(x);
    }
    ASSERT_EQ(tree_2, tree_1);
    ASSERT_EQ(*tree_2.find(20), 20);
    tree_2.erase(20);
    ASSERT_EQ(tree_2.find(20), tree_2.end());
    ASSERT_EQ(*tree_2.find(11), 11);
    tree_2.erase(11);
    ASSERT_EQ(tree_2.find(11), tree_2.end());
}

TEST(Compare, Unless) {
    std::vector<int> data = {1, 20, 54, 11, 56, 81, -11, 1};
    auto unless_compare = [](const int& x, const int& y) {return x > y;};
    std::set<int, decltype(unless_compare)> s(data.begin(), data.end());
    BinaryTree<int, WalkType::InOrder, decltype(unless_compare)> t(data.begin(), data.end(), unless_compare);
    auto it_s = s.begin();
    auto it_t = t.begin();
    while (it_s != s.end() && it_t != t.end()) {
        ASSERT_EQ(*(it_s++), *(it_t++));
    }
    ASSERT_EQ(it_s, s.end());
    ASSERT_EQ(it_t, t.end());
}