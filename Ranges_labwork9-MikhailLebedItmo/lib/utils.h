#pragma once
#include <iterator>

template<std::input_or_output_iterator It>
void my_advance(It& it, std::iter_difference_t<It> n) {
    if constexpr (std::random_access_iterator<It>) {
        it += n;}
    else {
        while (n > 0) {
            --n;
            ++it;
        }

        if constexpr (std::bidirectional_iterator<It>) {
            while (n < 0) {
                ++n;
                --it;
            }
        }
    }
}

template<std::input_or_output_iterator It, std::sentinel_for<It> S>
void my_advance(It& it, S bound) {
    if constexpr (std::assignable_from<It&, S>) {
        it = std::move(bound);
    }
    else if constexpr (std::sized_sentinel_for<S, It>) {
        my_advance(it, bound - it);
    }
    else {
        while (it != bound) {
            ++it;
        }
    }
}

template<std::input_or_output_iterator It, std::sentinel_for<It> End>
void my_advance(It& it, std::iter_difference_t<It> n, End bound) {
    if constexpr (std::sized_sentinel_for<End, It>) {
        if (auto dist = std::abs(bound - it) - std::abs(n); dist < 0) {
            my_advance(it, bound);
            return;
        }
        my_advance(it, n);
    }
    else {
        while (n > 0 && it != bound) {
            --n;
            ++it;
        }

        if constexpr (std::bidirectional_iterator<It>) {
            while (n < 0 && it != bound) {
                ++n;
                --it;
            }
        }
    }
}