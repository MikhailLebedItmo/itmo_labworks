#pragma once
#include <iterator>

template <typename T>
concept View = requires(T x) {
    x.begin();
    x.end();
    {x.begin()} -> std::forward_iterator;
    {x.end()} -> std::forward_iterator;
    {x.begin() != x.end()} -> std::convertible_to<bool>;
};

template <typename T>
concept BidirectionalView = View<T> && requires(T x) {
    {x.begin()} -> std::bidirectional_iterator;
    {x.end()} -> std::bidirectional_iterator;
};

template <typename V, size_t N>
concept TupleLikeView = View<V> && requires(V v) {
    std::get<N>(*v.begin());
};

template<typename T>
concept is_void_type = std::is_void_v<T>;

template<typename T>
concept is_not_void_type = !is_void_type<T>;

template <typename F, typename Iter>
concept TransformFunc = requires(Iter it, F func) {
    {func(std::declval<const typename std::iterator_traits<Iter>::value_type &>())} -> is_not_void_type;
};

