// updated 26 April 2022

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "RandomGenerator.hpp"
#include "Map.hpp"

#include <iostream>

TEST(locationTests, constructor) {
    RandomGenerator g;
    Location l{g};
    ASSERT_GT(l.here.size(), 1);
    ASSERT_NE(l.biome, "");

    Location l2{l.biome, l.here};
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }
}

TEST(mapTests, constructor) {
    Map m;
}

TEST(mapTests, get) {
    Map m;
    Location l = m.get();
    ASSERT_GT(l.here.size(), 1);
    ASSERT_NE(l.biome, "");

    Location l2 = m.get(0, 0);
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }

    Location l3 = m.get(Dir::NORTH);
    ASSERT_GT(l3.here.size(), 1);
    ASSERT_NE(l3.biome, "");
}

TEST(mapTests, save) {

}

TEST(mapTests, load) {
    
}