#pragma once
#include "adapter_concepts.h"

#include <iterator>

template <std::forward_iterator It>
class TakeView {
public:
    TakeView(It beg, It last, size_t n): beg(beg), last(last) {
        if constexpr (std::random_access_iterator<It>) {
            if (last - beg > n) {
                this->last = beg + n;
            }
        } else {
            auto new_last = beg;
            while (n > 0 && new_last != last) {
                --n;
                ++new_last;
            }
            this->last = new_last;
        }
    }

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

class Take {
public:
    const size_t n;
    explicit Take(size_t n): n(n) {}
};

template <View V>
auto operator|(V&& left, Take take) {
    return TakeView(left.begin(), left.end(), take.n);
}