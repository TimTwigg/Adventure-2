#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <iostream>
#include "Player.hpp"
#include "SkillSets.hpp"

TEST(PlayerTests, Constructor) {
    Player p{SkillSets::TRAVELER, "test"};
    p.save();
}

TEST(PlayerTests, getStatNames) {
    Player p{SkillSets::TRAVELER, "test"};
    std::vector<std::string> v = p.getStatNames();
    ASSERT_EQ(v.size(), 20);
}