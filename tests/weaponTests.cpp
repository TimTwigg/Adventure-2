// updated 8 April 2022

#include <gtest/gtest.h>
#include <string>
#include <map>
#include "Weapon.hpp"
#include "AdventureException.hpp"

TEST(weaponTests, constructor) {
    Weapon w = Weapon("stone-sword");
    Weapon w2 = Weapon("bow");
    ASSERT_THROW(Weapon(""), AdventureException);
}

TEST(weaponTests, getters) {
    Weapon w = Weapon("bow");
    ASSERT_EQ(w.getName(), "bow");
    ASSERT_EQ(w.getType(), OBJCLASS::WEAPON);
    ASSERT_EQ(w.getValue(), 10);
    ASSERT_EQ(w.getDamage(), 2);

    std::map<std::string, unsigned int> r = w.getRecipe();
    ASSERT_EQ(r.size(), 2);
    ASSERT_EQ(r["wood"], 1);
    ASSERT_EQ(r["string"], 1);
}

TEST(weaponTests, operatorString) {
    Weapon w = Weapon("bow");
    std::string s = w;
    ASSERT_EQ(s, "WEAPON, bow, 20");
}

TEST(weaponTests, use) {
    Weapon w = Weapon("bow");
    w.use();
    ASSERT_EQ(w.getUses(), 19);

    for (int i = 0; i < 19; ++i) {
        w.use();
    }
    ASSERT_THROW(w.use(), AdventureException);
}

TEST(waeponTests, fromString) {
    Weapon* w = Weapon::from_string("WEAPON, stone-sword, 1");
    ASSERT_EQ(w->getUses(), 1);
    delete w;
}