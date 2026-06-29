#pragma once

#include <initializer_list>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> init) {
        for (const auto &book : init) {
            PushBack(book);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    BookContainer::iterator begin() {
        return books_.begin();
    }

    BookContainer::const_iterator begin() const {
        return books_.begin();
    }

    BookContainer::const_iterator cbegin() const {
        return books_.cbegin();
    }

    BookContainer::iterator end() {
        return books_.end();
    }

    BookContainer::const_iterator end() const {
        return books_.end();
    }

    BookContainer::const_iterator cend() const {
        return books_.cend();
    }

    [[nodiscard]] bool empty() const {
        return books_.empty();
    }

    [[nodiscard]] BookContainer::size_type size() const {
        return books_.size();
    }

    BookContainer::reference front() {
        return books_.front();
    }

    BookContainer::const_reference front() const {
        return books_.front();
    }

    BookContainer::reference back() {
        return books_.back();
    }

    BookContainer::const_reference back() const {
        return books_.back();
    }

    void PushBack(const Book &book) {
        const std::string_view author = StoreAuthor(book.author);
        books_.push_back(Book(author, book.title, book.year, book.genre, book.rating, book.read_count));
    }

    void PushBack(Book &&book) {
        const std::string_view author = StoreAuthor(book.author);
        book.author = author;
        books_.push_back(std::move(book));
    }

    template <typename... Args>
        requires std::constructible_from<Book, Args...>
    void EmplaceBack(Args &&...args) {
        Book book(std::forward<Args>(args)...);
        PushBack(std::move(book));
    }

    std::span<const Book> GetBooks() const {
        return std::span<const Book>(books_.data(), books_.size());
    }

    std::span<Book> GetBooks() {
        return std::span<Book>(books_.data(), books_.size());
    }

    const AuthorContainer &GetAuthors() {
        return authors_;
    }

    const AuthorContainer &GetAuthors() const {
        return authors_;
    }

private:
    std::string_view StoreAuthor(std::string_view author) {
        auto [it, inserted] = authors_.emplace(author);
        (void)inserted;
        return *it;
    }

    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}):\n", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std