#include <gtest/gtest.h>

#include <format>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "book_database.hpp"
#include "filters.hpp"

namespace bookdb {

TEST(FilterFactoriesTest, Test1) {
    const auto pred = YearBetween(2000, 2010);

    EXPECT_TRUE(pred(Book{"Автор", "Книга 2000", 2000, Genre::Fiction, 4.0, 10}));
    EXPECT_TRUE(pred(Book{"Автор", "Книга 2010", 2010, Genre::Fiction, 4.0, 10}));
    EXPECT_FALSE(pred(Book{"Автор", "Книга 1999", 1999, Genre::Fiction, 4.0, 10}));
    EXPECT_FALSE(pred(Book{"Автор", "Книга 2011", 2011, Genre::Fiction, 4.0, 10}));
}

TEST(FilterFactoriesTest, Test2) {
    const auto pred = YearBetween(2025, 2020);

    EXPECT_FALSE(pred(Book{"Автор", "Книга", 2022, Genre::Fiction, 4.0, 10}));
}

TEST(FilterFactoriesTest, Test3) {
    const auto pred = RatingAbove(4.5);

    EXPECT_FALSE(pred(Book{"Автор", "Ровно порог", 2020, Genre::Fiction, 4.5, 10}));
    EXPECT_TRUE(pred(Book{"Автор", "Выше порога", 2020, Genre::Fiction, 4.6, 10}));
}

TEST(FilterFactoriesTest, Test4) {
    const auto pred = GenreIs(Genre::Mystery);

    EXPECT_TRUE(pred(Book{"Автор", "Детектив", 2020, Genre::Mystery, 4.1, 10}));
    EXPECT_FALSE(pred(Book{"Автор", "Фантастика", 2020, Genre::SciFi, 4.1, 10}));
}

TEST(FilterCompositionTest, Test5) {
    BookDatabase<std::vector<Book>> db{
        Book{"Автор 1", "Подходит", 2021, Genre::Fiction, 4.8, 100},
        Book{"Автор 2", "Рейтинг низкий", 2021, Genre::Fiction, 4.2, 100},
        Book{"Автор 3", "Жанр другой", 2021, Genre::Mystery, 4.9, 100},
        Book{"Автор 4", "Год другой", 2015, Genre::Fiction, 4.9, 100},
    };

    const auto filtered = filterBooks(
        db.begin(),
        db.end(),
        all_of(YearBetween(2020, 2022), RatingAbove(4.5), GenreIs(Genre::Fiction)));

    ASSERT_EQ(filtered.size(), 1U);
    EXPECT_EQ(filtered.front().get().title, "Подходит");
}

TEST(FilterCompositionTest, Test6) {
    BookDatabase<std::vector<Book>> db{
        Book{"Автор 1", "По рейтингу", 2021, Genre::Fiction, 4.9, 100},
        Book{"Автор 2", "По жанру", 2010, Genre::Mystery, 3.0, 100},
        Book{"Автор 3", "Не подходит", 2010, Genre::Biography, 3.0, 100},
    };

    const auto filtered = filterBooks(
        db.begin(),
        db.end(),
        any_of(RatingAbove(4.8), GenreIs(Genre::Mystery)));

    ASSERT_EQ(filtered.size(), 2U);
    EXPECT_EQ(filtered[0].get().title, "По рейтингу");
    EXPECT_EQ(filtered[1].get().title, "По жанру");
}
}  // namespace bookdb