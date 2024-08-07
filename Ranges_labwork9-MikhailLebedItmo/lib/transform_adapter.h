#pragma once
#include "adapter_concepts.h"

#include <concepts>
#include <iterator>

template <std::forward_iterator It, TransformFunc<It> Func>
class TransformForwardIterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = decltype(std::declval<Func>()(*std::declval<It>()));
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using pointer = value_type*;
    using reference = value_type;

    TransformForwardIterator(): transform(), cur() {}

    TransformForwardIterator(It it, Func transform): cur(it), transform(transform) {}

    TransformForwardIterator& operator++() {
        ++cur;
        return *this;
    }

    TransformForwardIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return transform(*cur);
    }

    friend bool operator==(const TransformForwardIterator& left, const TransformForwardIterator& right) {
        return left.cur == right.cur;
    }

    friend bool operator!=(const TransformForwardIterator& left, const TransformForwardIterator& right) {
        return left.cur != right.cur;
    }
private:
    It cur;
    [[no_unique_address]] Func transform;
};

template <std::bidirectional_iterator It, TransformFunc<It> Func>
class TransformBidirectionalIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = decltype(std::declval<Func>()(*std::declval<It>()));
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using pointer = value_type*;
    using reference = value_type;

    TransformBidirectionalIterator(): transform(), cur() {}

    TransformBidirectionalIterator(It it, Func transform)
            : transform(transform), cur(it) {}

    TransformBidirectionalIterator& operator++() {
        ++cur;
        return *this;
    }

    TransformBidirectionalIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return transform(*cur);
    }

    TransformBidirectionalIterator& operator--() {
        --cur;
        return *this;
    }

    TransformBidirectionalIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }

    friend bool operator==(const TransformBidirectionalIterator& left, const TransformBidirectionalIterator& right) {
        return left.cur == right.cur;
    }

    friend bool operator!=(const TransformBidirectionalIterator& left, const TransformBidirectionalIterator& right) {
        return left.cur != right.cur;
    }
private:
    It cur;
    [[no_unique_address]] Func transform;
};

template <std::random_access_iterator It, TransformFunc<It> Func>
class TransformRandomAccessIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = decltype(std::declval<Func>()(*std::declval<It>()));
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using pointer = value_type*;
    using reference = value_type;

    TransformRandomAccessIterator(): cur(), transform() {}

    TransformRandomAccessIterator(It it, Func transform)
            : cur(it), transform(transform) {}

    TransformRandomAccessIterator& operator++() {
        ++cur;
        return *this;
    }

    TransformRandomAccessIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return transform(*cur);
    }

    TransformRandomAccessIterator& operator--() {
        --cur;
        return *this;
    }

    TransformRandomAccessIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }

    TransformRandomAccessIterator& operator+=(size_t n) {
        cur += n;
        return *this;
    }

    TransformRandomAccessIterator& operator-=(size_t n)  {
        cur -= n;
        return *this;
    }

    reference operator[](size_t n) const {
        return transform(*(cur + n));
    }

    friend bool operator==(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur == right.cur;
    }

    friend bool operator!=(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur != right.cur;
    }

    friend bool operator<(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur < right.cur;
    }

    friend bool operator>(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur > right.cur;
    }

    friend bool operator<=(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur <= right.cur;
    }

    friend bool operator>=(const TransformRandomAccessIterator& left, const TransformRandomAccessIterator& right) {
        return left.cur >= right.cur;
    }

    friend TransformRandomAccessIterator operator+(const TransformRandomAccessIterator& left, size_t n) {
        auto copy = left;
        copy += n;
        return copy;
    }

    friend TransformRandomAccessIterator operator+(size_t n, const TransformRandomAccessIterator& left) {
        auto copy = left;
        copy += n;
        return copy;
    }

    friend TransformRandomAccessIterator operator-(const TransformRandomAccessIterator& left, size_t n) {
        auto copy = left;
        copy -= n;
        return copy;
    }

    friend TransformRandomAccessIterator operator-(size_t n, const TransformRandomAccessIterator& left) {
        auto copy = left;
        copy -= n;
        return copy;
    }
private:
    It cur;
    [[no_unique_address]] Func transform;
};

template <std::forward_iterator It, TransformFunc<It> Func>
class TransformView {
public:
    TransformView(It beg, It last, Func transform)
            :beg(beg), last(last), transform(transform) {}

    auto begin() {
        if constexpr (std::random_access_iterator<It>) {
            return TransformRandomAccessIterator(beg, transform);
        } else if constexpr (std::bidirectional_iterator<It>){
            return TransformBidirectionalIterator(beg, transform);
        } else {
            return TransformForwardIterator(beg, transform);
        }
    }

    auto end() {
        if constexpr (std::random_access_iterator<It>) {
            return TransformRandomAccessIterator(last, transform);
        } else if constexpr (std::bidirectional_iterator<It>){
            return TransformBidirectionalIterator(last, transform);
        } else {
            return TransformForwardIterator(last, transform);
        }
    }
private:
    It beg;
    It last;
    [[no_unique_address]] Func transform;
};

template <typename Func>
class Transform {
public:
    explicit Transform(Func transform): transform(transform) {}

    Func get_transform() const {
        return transform;
    }
private:
    [[no_unique_address]] Func transform;
};

template <View V, TransformFunc<decltype(std::declval<V>().begin())> Func>
auto operator|(V&& left, Transform<Func> filter) {
    return TransformView(left.begin(), left.end(), filter.get_transform());
}
