#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "book_database.hpp"

using namespace std::literals;

namespace bookdb {

TEST(BookDatabaseTest, Test1) {
    BookDatabase<std::vector<Book>> db{
        Book{"Платон Иванов", "Курсы по С++", 2020, Genre::Fiction, 4.8, 190},
        Book{"Иван Иванов", "Яндекс Практикум", 2026, Genre::Fiction, 4.9, 156},
    };

    EXPECT_EQ(db.size(), 2U);
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.begin()->title, "Курсы по С++");
    EXPECT_EQ(db.back().title, "Яндекс Практикум");

    const auto books = db.GetBooks();
    EXPECT_EQ(books.size(), 2U);
    EXPECT_EQ(books[1].author, "Иван Иванов");
}

TEST(BookDatabaseTest, Test2) {
    BookDatabase<std::vector<Book>> db;

    db.PushBack(Book{"Иван Иванов", "Тень", 2021, Genre::Fiction, 4.8, 190});
    db.EmplaceBack("Алексей Иванов", "Пароход", 2023, Genre::Fiction, 4.7, 143);
    db.EmplaceBack("Дмитрий Иванов", "Сад", 2020, Genre::Fiction, 4.6, 120);

    EXPECT_EQ(db.size(), 3U);
    EXPECT_EQ(db.GetAuthors().size(), 3U);
    EXPECT_NE(db.GetAuthors().find("Иван Иванов"), db.GetAuthors().end());
    EXPECT_NE(db.GetAuthors().find("Алексей Иванов"), db.GetAuthors().end());
    EXPECT_NE(db.GetAuthors().find("Дмитрий Иванов"), db.GetAuthors().end());
}

TEST(BookDatabaseTest, Test3) {
    BookDatabase<std::vector<Book>> db;

    db.EmplaceBack("Иван Иванов", "Три танкиста и собака", 2024, Genre::Fiction, 4.9, 178);
    db.EmplaceBack("Анна Чапман", "Тайны Чапман", 2025, Genre::Fiction, 4.8, 110);

    db.Clear();

    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0U);
    EXPECT_TRUE(db.GetBooks().empty());
    EXPECT_TRUE(db.GetAuthors().empty());
    EXPECT_EQ(db.begin(), db.end());
}
}  // namespace bookdb
