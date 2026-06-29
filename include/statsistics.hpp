#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <string_view>
#include <vector>

#include "book_database.hpp"

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::vector<std::pair<std::string_view, std::size_t>> histogram;

    for (const auto &book : cont.GetBooks()) {
        bool found = false;

        for (auto &entry : histogram) {
            if (!comp(entry.first, book.author) && !comp(book.author, entry.first)) {
                ++entry.second;
                found = true;
                break;
            }
        }

        if (!found) {
            histogram.push_back({book.author, 1});
        }
    }

    std::sort(
        histogram.begin(),
        histogram.end(),
        [&comp](const auto &lhs, const auto &rhs) {
            return comp(lhs.first, rhs.first);
        });

    return histogram;
}

template <typename Container>
double FindValueByGenre(const Container &ratings, Genre genre) {
    for (const auto &entry : ratings) {
        if (entry.first == genre) {
            return entry.second;
        }
    }

    return 0.0;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last) {
    std::vector<std::pair<Genre, std::pair<double, std::size_t>>> sums;

    for (; first != last; ++first) {
        bool found = false;

        for (auto &entry : sums) {
            if (entry.first == first->genre) {
                entry.second.first += first->rating;
                ++entry.second.second;
                found = true;
                break;
            }
        }

        if (!found) {
            sums.push_back({first->genre, {first->rating, 1}});
        }
    }

    std::vector<std::pair<Genre, double>> result;

    for (const auto &[genre, data] : sums) {
        if (data.second != 0) {
            result.push_back({genre, data.first / static_cast<double>(data.second)});
        }
    }

    std::sort(
        result.begin(),
        result.end(),
        [](const auto &lhs, const auto &rhs) {
            return static_cast<int>(lhs.first) < static_cast<int>(rhs.first);
        });

    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    if (books.empty()) {
        return 0.0;
    }

    const double sum = std::accumulate(
        books.begin(),
        books.end(),
        0.0,
        [](double current_sum, const Book &book) {
            return current_sum + book.rating;
        });

    return sum / static_cast<double>(books.size());
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num) {
    std::vector<std::reference_wrapper<const Book>> all_books;
    std::vector<std::reference_wrapper<const Book>> result;
    const std::size_t actual_num = std::min(num, cont.size());

    all_books.reserve(cont.size());
    result.reserve(actual_num);

    for (const auto &book : cont) {
        all_books.push_back(std::cref(book));
    }

    std::mt19937 generator(std::random_device{}());
    std::ranges::sample(all_books,
        std::back_inserter(result),
        actual_num,
        generator);

    return result;
}

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp) {
    std::vector<std::reference_wrapper<const Book>> result;
    const std::size_t actual_num = std::min(n, cont.size());

    std::sort(cont.begin(), cont.end(), comp);

    result.reserve(actual_num);

    for (std::size_t i = 0; i < actual_num; i++) {
        const std::size_t index = cont.size() - 1 - i;
        result.push_back(std::cref(cont.GetBooks()[index]));
    }

    return result;
}

}  // namespace bookdb
