#pragma once
#include "adapter_concepts.h"

#include <concepts>
#include <iterator>

template <std::bidirectional_iterator It>
class ReverseView {
public:
    ReverseView(It beg, It last): beg(beg), last(last) {}

    It begin() {
        return beg;
    }

    It end() {
        return last;
    }
private:
    It beg;
    It last;
};

class Reverse {};

template <BidirectionalView V>
auto operator|(V&& left, Reverse right) {
    auto rev_end = std::reverse_iterator(left.begin());
    auto rev_begin = std::reverse_iterator(left.end());

    return ReverseView(rev_begin, rev_end);
}

