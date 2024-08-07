#pragma once
#include "utils.h"
#include "adapter_concepts.h"

#include <iterator>


template <std::input_iterator It>
class DropView{
public:
    using reference = std::iter_reference_t<It>;
    using value_type = std::iter_value_t<It>;

    DropView(It beg, It last): beg(beg), last(last) {}

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

class Drop {
public:
    const size_t shift;
    explicit Drop(size_t shift): shift(shift) {}
};

template <View V>
auto operator|(V&& left, Drop right) {
    auto new_begin = left.begin();
    my_advance(new_begin, right.shift, left.end());
    return DropView(new_begin, left.end());
}