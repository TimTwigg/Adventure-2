// updated 9 August 2023

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "RandomGenerator.hpp"
#include "Map.hpp"
#include "AdventureException.hpp"
#include "Animal.hpp"

TEST(locationTests, constructor) {
    RandomGenerator g;
    Location l{g};
    ASSERT_NE(l.biome, "");

    Location l2{l.biome, l.save()};
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.miscHere.size(), l2.miscHere.size());
    ASSERT_EQ(l.thingsHere.size(), l2.thingsHere.size());
}

TEST(locationTests, addRemoveThing) {
    RandomGenerator g;
    Location l{"plains", {}};
    ASSERT_EQ(l.thingsHere.size(), 0);
    l.addThing(std::shared_ptr<Thing>(new Animal("snake")));
    ASSERT_EQ(l.thingsHere.size(), 1);
    ASSERT_NO_THROW(std::shared_ptr<Thing> t = l.accessThing("snake"));
    std::shared_ptr<Thing> t = l.accessThing("snake");
    ASSERT_EQ(l.thingsHere.size(), 1);
    ASSERT_EQ(t->getName(), "snake");
    ASSERT_NO_THROW(l.removeThing(t->getName()));
    ASSERT_EQ(l.thingsHere.size(), 0);
}

TEST(mapTests, constructor) {
    Map m{"saves\\test"};
}

TEST(mapTests, copyConstructor) {
    Map m{"saves\\test"};
    Location l = m.get(Dir::NORTH);

    Map m2{m};
    Location l2 = m.get(Dir::NORTH);
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.miscHere.size(), l2.miscHere.size());
    ASSERT_EQ(l.thingsHere.size(), l2.thingsHere.size());
    
    Location l3 = m.get(Dir::WEST);
    Location l4 = m.get(-1, 0);
    ASSERT_THROW(m2.get(-1, 0), AdventureException);
}

TEST(mapTests, get) {
    Map m{"saves\\test"};
    Location l = m.get();
    ASSERT_NE(l.biome, "");

    Location l2 = m.get(0, 0);
    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.miscHere.size(), l2.miscHere.size());
    ASSERT_EQ(l.thingsHere.size(), l2.thingsHere.size());

    Location l3 = m.get(Dir::NORTH);
    ASSERT_NE(l3.biome, "");
}

TEST(mapTests, saveload) {
    Map m{"saves\\test"};
    Location l = m.get(Dir::NORTH);
    m.save();
    std::string path = m.getPath();

    Map* m2 = Map::load(path);
    Location l2 = m2->get(Dir::NORTH);
    delete m2;

    ASSERT_EQ(l.biome, l2.biome);
    ASSERT_EQ(l.miscHere.size(), l2.miscHere.size());
    ASSERT_EQ(l.thingsHere.size(), l2.thingsHere.size());
}