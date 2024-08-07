#pragma once
#include "adapter_concepts.h"

#include <concepts>
#include <iterator>


template <std::forward_iterator It, std::indirect_unary_predicate<It> Func>
class FilterForwardIterator: public std::iterator<std::forward_iterator_tag,
                                            std::iter_value_t<It>,
                                            std::iter_difference_t<It>,
                                            std::add_pointer_t<std::iter_value_t<It>>,
                                            std::iter_reference_t<It>> {
public:
    FilterForwardIterator(): predicate(), cur(), last(cur) {}

    FilterForwardIterator(It beg, It last, Func predicate)
      : predicate(predicate), cur(beg), last(last) {}

    FilterForwardIterator& operator++() {
        ++cur;
        while (cur != last && !predicate(*cur)) {
            ++cur;
        }
        return *this;
    }

    FilterForwardIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    std::iter_reference_t<It> operator*() const {
        return *cur;
    }

    friend bool operator==(const FilterForwardIterator& left, const FilterForwardIterator& right) {
        return left.cur == right.cur && left.last == right.last;
    }

    friend bool operator!=(const FilterForwardIterator& left, const FilterForwardIterator& right) {
        return !(left == right);
    }
private:
    It cur;
    It last;
    [[no_unique_address]] Func predicate;
};

template <std::bidirectional_iterator It, std::indirect_unary_predicate<It> Func>
class FilterBidirectionalIterator: public std::iterator<std::bidirectional_iterator_tag,
                                    std::iter_value_t<It>,
                                    std::iter_difference_t<It>,
                                    std::add_pointer_t<std::iter_value_t<It>>,
                                    std::iter_reference_t<It>> {
public:
    FilterBidirectionalIterator(): predicate(), cur(), last(cur) {}

    FilterBidirectionalIterator(It beg, It last, Func predicate)
      : predicate(predicate), cur(beg), last(last) {}

    FilterBidirectionalIterator& operator++() {
        ++cur;
        while (cur != last && !predicate(*cur)) {
            ++cur;
        }
        return *this;
    }

    FilterBidirectionalIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    FilterBidirectionalIterator& operator--() {
        --cur;
        while (!predicate(*cur)) {
            --cur;
        }
        return *this;
    }

    FilterBidirectionalIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }

    std::iter_reference_t<It> operator*() const {
        return *cur;
    }

    friend bool operator==(const FilterBidirectionalIterator& left, const FilterBidirectionalIterator& right) {
        return left.cur == right.cur && left.last == right.last;
    }

    friend bool operator!=(const FilterBidirectionalIterator& left, const FilterBidirectionalIterator& right) {
        return !(left == right);
    }
private:
    It cur;
    It last;
    [[no_unique_address]] Func predicate;
};

template <std::forward_iterator It, std::indirect_unary_predicate<It> Func>
class FilterView {
public:
    FilterView(It beg, It last, Func predicate)
      :beg(beg), last(last), predicate(predicate) {
        while (this->beg != last && !predicate(*this->beg)) {
            ++(this->beg);
        }
    }

    auto begin() {
        if constexpr (std::bidirectional_iterator<It>) {
            return FilterBidirectionalIterator(beg, last, predicate);
        } else {
            return FilterForwardIterator(beg, last, predicate);
        }
    }

    auto end() {
        if constexpr (std::bidirectional_iterator<It>) {
            return FilterBidirectionalIterator(last, last, predicate);
        } else {
            return FilterForwardIterator(last, last, predicate);
        }
    }
private:
    It beg;
    It last;
    [[no_unique_address]] Func predicate;
};


template <typename Func>
class Filter {
public:
    explicit Filter(Func predicate): predicate(predicate) {}

    Func get_predicate() const {
        return predicate;
    }
private:
    [[no_unique_address]] Func predicate;
};

template <View V, std::indirect_unary_predicate<decltype(std::declval<V>().begin())> Func>
auto operator|(V&& left, Filter<Func> filter) {
    return FilterView(left.begin(), left.end(), filter.get_predicate());
}