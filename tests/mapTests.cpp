// updated 9 April 2022

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "RandomGenerator.hpp"
#include "Map.hpp"

TEST(locationTests, randomConstructor) {
    RandomGenerator g;
    Location l{g};
    ASSERT_GT(l.here.size(), 1);
    ASSERT_NE(l.biome, "");
}