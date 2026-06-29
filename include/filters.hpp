#pragma once

#include <functional>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

constexpr auto YearBetween(int start, int end)
{
    return [start, end](const Book& book)
    {
        return book.year >= start && book.year <= end;
    };
}

constexpr auto RatingAbove(double min_rating)
{
    return [min_rating](const Book& book)
    {
        return book.rating > min_rating;
    };
}

constexpr auto GenreIs(Genre genre)
{
    return [genre](const Book& book)
    {
        return book.genre == genre;
    };
}

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred)
{
    std::vector<std::reference_wrapper<const Book>> result;

    for (; first != last; ++first) {
        if (pred(*first)) {
            result.push_back(std::cref(*first));
        }
    }

    return result;
}

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds)
{
    return [=](const Book& book)
    {
        return (... && preds(book));
    };
}

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds)
{
    return [=](const Book& book)
    {
        return (... || preds(book));
    };
}
}  // namespace bookdb