// updated 8 April 2022

#include <gtest/gtest.h>
#include <string>
#include <map>
#include "Liquid.hpp"
#include "AdventureException.hpp"

TEST(liquidTests, constructor) {
    Liquid l;
    Liquid l2{"water"};
}

TEST(liquidTests, operatorString) {
    Liquid l;
    std::string s = l;
    ASSERT_EQ(s, "LIQUID, empty");
}

TEST(liquidTests, getters) {
    Liquid l{"milk"};
    ASSERT_EQ(l.getHunger(), 3);
    ASSERT_EQ(l.getThirst(), 3);
    ASSERT_EQ(l.getHP(), 25);
}