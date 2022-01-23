// updated 22 January 2022

#include <gtest/gtest.h>
#include <vector>
#include "RandomGenerator.hpp"
#include "AdventureException.hpp"

TEST(RandomGeneratorTests, constructor) {
    RandomGenerator gen;
}

TEST(RandomGeneratorTests, getRandInt) {
    RandomGenerator gen;
    int x = gen.getRandInt(1, 10);
    ASSERT_GE(x, 1);
    ASSERT_LE(x, 10);

    int total = 0;
    for (int i = 0; i < 10; ++i) {
        total += gen.getRandInt(1, 5);
    }
    ASSERT_GE(total, 10);
    ASSERT_LE(total, 50);

    ASSERT_THROW(gen.getRandInt(5, 4), AdventureException);
}

TEST(RandomGeneratorTests, getRandIntVector) {
    RandomGenerator gen;
    std::vector<int> v = gen.getRandIntVector(1, 2, 5);
    ASSERT_EQ(v.size(), 5);
    
    int sum = 0;
    for (int i : v) {
        sum += i;
    }
    ASSERT_LE(sum, 10);
    ASSERT_GE(sum, 5);

    v = gen.getRandIntVector(1, 1, 5);
    ASSERT_EQ(v.size(), 5);
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(v[i], 1);
    }

    ASSERT_THROW(gen.getRandIntVector(3, 2, 5), AdventureException);
}

TEST(RandomGeneratorTests, getRandDoubleVector) {
    RandomGenerator gen;
    std::vector<double> v = gen.getRandDoubleVector(5, 15, 5);
    ASSERT_EQ(v.size(), 5);

    double sum = 0;
    for (double i : v) {
        sum += i;
    }
    ASSERT_LE(sum, 7.5);
    ASSERT_GE(sum, 2.5);

    v = gen.getRandDoubleVector(5, 5, 5);
    ASSERT_EQ(v.size(), 5);
    double total = 0;
    for (double i : v) {
        total += i;
    }
    ASSERT_EQ(total, 2.5);

    ASSERT_THROW(gen.getRandDoubleVector(4, 3, 5), AdventureException);
}

TEST(RandomGeneratorTests, getRandBool) {
    RandomGenerator gen;
    bool flag = true;
    for (int i = 0; i < 10; ++i) {
        bool b = gen.getRandBool();
        if (!b) flag = b;
    }
    ASSERT_FALSE(flag);    
}