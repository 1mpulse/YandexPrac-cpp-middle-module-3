#pragma once

#include <concepts>
#include <iterator>
#include <utility>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T cont)
{
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { cont.begin() } -> std::input_or_output_iterator;
    { cont.end() };
    { cont.cbegin() } -> std::input_or_output_iterator;
    { cont.cend() };
    { cont.size() } -> std::convertible_to<std::size_t>;
    { cont.clear() };
} && std::same_as<typename T::value_type, Book>;

template <typename T>
concept BookIterator = std::input_iterator<T> && std::same_as<std::remove_cvref_t<std::iter_value_t<T>>, Book>;

template <typename S, typename I>
concept BookSentinel = BookIterator<I> && std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P pred, const Book& book)
{
    { pred(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comp, const Book& lhs, const Book& rhs)
{
    { comp(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb