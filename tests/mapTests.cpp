// updated 30 April 2022

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "RandomGenerator.hpp"
#include "Map.hpp"
#include "AdventureException.hpp"

TEST(locationTests, constructor) {
    RandomGenerator g;
    Location l{g};
    ASSERT_NE(l.biome, "");

    Location l2{l.biome, l.here};
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }
}

TEST(mapTests, constructor) {
    Map m{"test"};
}

TEST(mapTests, copyConstructor) {
    Map m{"test"};
    Location l = m.get(Dir::NORTH);

    Map m2{m};
    Location l2 = m.get(Dir::NORTH);
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }

    Location l3 = m.get(Dir::WEST);
    Location l4 = m.get(-1, 0);
    ASSERT_THROW(m2.get(-1, 0), AdventureException);
}

TEST(mapTests, get) {
    Map m{"test"};
    Location l = m.get();
    ASSERT_NE(l.biome, "");

    Location l2 = m.get(0, 0);
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }

    Location l3 = m.get(Dir::NORTH);
    ASSERT_NE(l3.biome, "");
}

TEST(mapTests, saveload) {
    Map m{"test"};
    Location l = m.get(Dir::NORTH);
    m.save();
    std::string path = m.getPath();

    Map m2 = Map::load(path);
    Location l2 = m2.get(Dir::NORTH);

    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.here.size(), l2.here.size());
    for (int i = 0; i < l.here.size(); ++i) {
        ASSERT_EQ(l.here[i], l2.here[i]);
    }
}