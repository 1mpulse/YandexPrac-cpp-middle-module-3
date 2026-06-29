#include <gtest/gtest.h>

#include <vector>

#include "comparators.hpp"
#include "statsistics.hpp"

namespace bookdb {

TEST(StatisticsTest, Test1) {
    BookDatabase<std::vector<Book>> db{
        Book{"Иван Иванов", "Тень", 2021, Genre::Fiction, 4.8, 190},
        Book{"Алексей Иванов", "Пароход", 2023, Genre::Fiction, 4.7, 143},
        Book{"Дмитрий Иванов", "Сад", 2020, Genre::Fiction, 4.6, 120},
    };

    const auto histogram = buildAuthorHistogramFlat(db);

    ASSERT_EQ(histogram.size(), 2U);
    EXPECT_EQ(histogram[0].first, "Алексей Иванов");
    EXPECT_EQ(histogram[0].second, 2U);
    EXPECT_EQ(histogram[1].first, "Иван Иванов");
    EXPECT_EQ(histogram[1].second, 1U);
}

TEST(StatisticsTest, Test2) {
    BookDatabase<std::vector<Book>> db{
        Book{"Иван Иванов", "Тень", 2021, Genre::Fiction, 4.7, 190},
        Book{"Алексей Иванов", "Пароход", 2023, Genre::SciFi, 4.2, 143},
        Book{"Дмитрий Иванов", "Сад", 2020, Genre::Fiction, 4.1, 120},
        Book{"Анна Чапман", "Тайны Чапман", 2021, Genre::Mystery, 4.4, 150},
    };

    const auto ratings = calculateGenreRatings(db.begin(), db.end());

    EXPECT_DOUBLE_EQ(FindValueByGenre(ratings, Genre::Fiction), 4.4);
    EXPECT_DOUBLE_EQ(FindValueByGenre(ratings, Genre::Mystery), 4.4);
    EXPECT_DOUBLE_EQ(FindValueByGenre(ratings, Genre::SciFi), 4.2);
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 4.35);
}

TEST(StatisticsTest, Test3) {
    BookDatabase<std::vector<Book>> db{
        Book{"Захар Петров", "Три танкиста и собака", 2024, Genre::Fiction, 4.9, 178},
        Book{"Александр Иванов", "Космос", 2022, Genre::Fiction, 4.8, 110},
        Book{"Алексей Петров", "Звезда", 2022, Genre::Mystery, 4.5, 105},
        Book{"Марина Иванова", "Сад", 2020, Genre::Fiction, 4.6, 120},
    };

    const auto sampled = sampleRandomBooks(db, 2);
    EXPECT_EQ(sampled.size(), 2U);

    const auto top_books = getTopNBy(db, 2, comp::LessByRating{});
    EXPECT_EQ(top_books.size(), 2U);
    EXPECT_EQ(top_books[0].get().title, "Три танкиста и собака");
    EXPECT_EQ(top_books[1].get().title, "Космос");
}

TEST(StatisticsTest, Test4) {
    BookDatabase<std::vector<Book>> db;

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 0.0);
    EXPECT_TRUE(sampleRandomBooks(db, 3).empty());
}

TEST(StatisticsTest, Test5) {
    BookDatabase<std::vector<Book>> db{
        Book{"Иван Иванов", "Тень", 2021, Genre::Fiction, 4.8, 190},
        Book{"Алексей Иванов", "Пароход", 2023, Genre::Fiction, 4.7, 143},
    };

    const auto sampled = sampleRandomBooks(db, 10);

    EXPECT_EQ(sampled.size(), 2U);
}

}  // namespace bookdb