// updated 26 January 2022

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "AdventureException.hpp"

namespace {
    std::vector<std::string> names{"level", "xp", "skillset", "health", "max_health",
        "base_damage", "fist_base_damage", "hunger", "max_hunger", "thirst", "max_thirst",
        "speed", "swimming_speed", "consumption_ratio", "chopping_ratio", "mining_ratio",
        "wealth", "inventory", "savepath", "ratios"};
}

TEST(PlayerTests, Constructor) {
    Player p{SkillSets::TRAVELER, "test"};
}

TEST(PlayerTests, getStatNames) {
    Player p{SkillSets::TRAVELER, "test"};
    std::vector<std::string> v = p.getStatNames();
    ASSERT_EQ(v.size(), 20);
    
    std::sort(v.begin(), v.end());
    std::sort(names.begin(), names.end());
    ASSERT_EQ(v, names);
}

TEST(PlayerTests, statMethods) {
    Player p{SkillSets::JESTER, "test"};
    std::vector<std::string> inv_names = p.getInvalidStatNames();
    for (auto n : p.getStatNames()) {
        if (std::find(inv_names.begin(), inv_names.end(), n) != inv_names.end()) {
            ASSERT_THROW(p.stat(n), AdventureException);
        }
        else {
            if (n == "wealth" || n == "xp") ASSERT_EQ(p.stat(n), 0);
            else ASSERT_GT(p.stat(n), 0);
        }
    }
}

TEST(PlayerTests, getters) {
    Player p{SkillSets::JESTER, "test"};
    std::string savepath = p.getSavepath();
    SkillSets ss = p.getSkillset();
    ASSERT_EQ(ss, SkillSets::JESTER);
    ASSERT_EQ(savepath.substr(6,4), "test");
}

TEST(PlayerTests, saveLoadFiles) {
    Player p{SkillSets::WARRIOR, "test"};
    p.save();
    Player q{p.getSavepath()};
    std::vector<std::string> pn = p.getStatNames();
    std::vector<std::string> qn = q.getStatNames();
    ASSERT_EQ(pn, qn);

    std::vector<std::string> inv_names = p.getInvalidStatNames();
    for (auto s : pn) {
        if (std::find(inv_names.begin(), inv_names.end(), s) != inv_names.end()) continue;
        ASSERT_EQ(p.stat(s), q.stat(s));
    }

    ASSERT_EQ(p.getSkillset(), q.getSkillset());
    ASSERT_EQ(p.getSavepath(), q.getSavepath());
}