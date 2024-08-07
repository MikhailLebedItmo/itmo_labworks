#pragma once
#include "adapter_concepts.h"
#include <iterator>
#include <type_traits>

template <size_t N, std::forward_iterator It>
class ValuesForwardIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using reference = decltype(std::get<N>(*std::declval<It>()));
    using value_type = std::remove_cvref<reference>;
    using pointer = value_type*;

    ValuesForwardIterator(): cur() {}

    explicit ValuesForwardIterator(It it): cur(it) {}

    ValuesForwardIterator& operator++() {
        ++cur;
        return *this;
    }

    ValuesForwardIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return std::get<N>(*cur);
    }

    friend bool operator==(const ValuesForwardIterator& left, const ValuesForwardIterator& right) {
        return left.cur == right.cur;
    }

    friend bool operator!=(const ValuesForwardIterator& left, const ValuesForwardIterator& right) {
        return left.cur != right.cur;
    }
private:
    It cur;
};

template <size_t N, std::bidirectional_iterator It>
class ValuesBidirectionalIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using reference = decltype(std::get<N>(*std::declval<It>()));
    using value_type = std::remove_cvref_t<reference>;
    using pointer = value_type*;

    ValuesBidirectionalIterator(): cur() {}

    explicit ValuesBidirectionalIterator(It it): cur(it) {}

    ValuesBidirectionalIterator& operator++() {
        ++cur;
        return *this;
    }

    ValuesBidirectionalIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return std::get<N>(*cur);
    }

    ValuesBidirectionalIterator& operator--() {
        --cur;
        return *this;
    }

    ValuesBidirectionalIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }

    friend bool operator==(const ValuesBidirectionalIterator& left, const ValuesBidirectionalIterator& right) {
        return left.cur == right.cur;
    }

    friend bool operator!=(const ValuesBidirectionalIterator& left, const ValuesBidirectionalIterator& right) {
        return left.cur != right.cur;
    }
private:
    It cur;
};

template <size_t N, std::random_access_iterator It>
class ValuesRandomAccessIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::iter_difference_t<It>;
    using distance_type = difference_type;
    using reference = decltype(std::get<N>(*std::declval<It>()));
    using value_type = std::remove_cvref_t<reference>;
    using pointer = value_type*;

    ValuesRandomAccessIterator(): cur() {}

    explicit ValuesRandomAccessIterator(It it): cur(it) {}

    ValuesRandomAccessIterator& operator++() {
        ++cur;
        return *this;
    }

    ValuesRandomAccessIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    reference operator*() const {
        return std::get<N>(*cur);
    }

    ValuesRandomAccessIterator& operator--() {
        --cur;
        return *this;
    }

    ValuesRandomAccessIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }

    ValuesRandomAccessIterator& operator+=(size_t n) {
        cur += n;
        return *this;
    }

    ValuesRandomAccessIterator& operator-=(size_t n)  {
        cur -= n;
        return *this;
    }

    reference operator[](size_t n) const {
        return std::get<N>(*(cur + n));
    }

    friend bool operator <=>(const ValuesRandomAccessIterator&, const ValuesRandomAccessIterator&) = default;

    friend ValuesRandomAccessIterator operator+(const ValuesRandomAccessIterator& left, size_t n) {
        auto copy = left;
        copy += n;
        return copy;
    }

    friend ValuesRandomAccessIterator operator+(size_t n, const ValuesRandomAccessIterator& left) {
        auto copy = left;
        copy += n;
        return copy;
    }

    friend ValuesRandomAccessIterator operator-(const ValuesRandomAccessIterator& left, size_t n) {
        auto copy = left;
        copy -= n;
        return copy;
    }

    friend ValuesRandomAccessIterator operator-(size_t n, const ValuesRandomAccessIterator& left) {
        auto copy = left;
        copy -= n;
        return copy;
    }
private:
    It cur;
};

template <size_t N = 1>
class Values {};

template <size_t N, std::forward_iterator It>
class ValuesView {
public:
    ValuesView(Values<N>, It beg, It last): beg(beg), last(last) {}

    auto begin() {
        if constexpr (std::random_access_iterator<It>) {
            return ValuesRandomAccessIterator<N, It>(beg);
        } else if constexpr (std::bidirectional_iterator<It>) {
            return ValuesBidirectionalIterator<N, It>(beg);
        } else {
            return ValuesForwardIterator<N, It>(beg);
        }
    }

    auto end() {
        if constexpr (std::random_access_iterator<It>) {
            return ValuesRandomAccessIterator<N, It>(last);
        } else if constexpr (std::bidirectional_iterator<It>) {
            return ValuesBidirectionalIterator<N, It>(last);
        } else {
            return ValuesForwardIterator<N, It>(last);
        }
    }
private:
    It beg;
    It last;
};

template <size_t N, TupleLikeView<N> V>
auto operator|(V&& left, Values<N> right) {
    return ValuesView(right, left.begin(), left.end());
}