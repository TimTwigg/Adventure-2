// updated 31 May 2022

#include <gtest/gtest.h>
#include "Animal.hpp"
#include "AdventureException.hpp"

TEST(animalTests, constructor) {
    Animal a{"chicken"};
    Animal b{" ChicKEN "};
    ASSERT_THROW(Animal c{""}, AdventureException);
}

TEST(animalTests, attack) {
    Animal a{"snake"};
    unsigned int d = a.attack();
    ASSERT_NE(d, 0);
}

TEST(animalTests, attackDMG) {
    Animal a{"snake"};
    bool b = a.attack(10);
    ASSERT_FALSE(b);
    ASSERT_EQ(a.getHP(), 65);

    b = a.attack(65);
    ASSERT_TRUE(b);
    ASSERT_EQ(a.getHP(), 0);

    a = Animal("snake");
    b = a.attack(100);
    ASSERT_TRUE(b);
    ASSERT_EQ(a.getHP(), 0);
}

TEST(animalTests, getDrops) {
    Animal a{"shark"};
    const std::vector<std::shared_ptr<Object>>& drops = a.getDrops();
    ASSERT_GT(drops.size(), 0);
}