// updated 25 January 2022

#include <gtest/gtest.h>
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(SkillSetsTests, getSet) {
    auto f = [](const json& s){
        std::vector<std::string> keys{"health", "damage", "fist_damage", "speed", "consumption_ratio",
            "chopping_ratio", "mining_ratio", "hunger_ratio", "swimming_speed"};
        bool flag = true;
        for (const auto& i : s.items()) {
            std::string k = i.key();
            if (std::find(keys.begin(), keys.end(), k) == keys.end()) flag = false;
        }
        return flag;
    };

    json set = SET::getSet(SkillSets::TRAVELER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::WARRIOR);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::BRAWLER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::SCOUT);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::LUMBERJACK);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::MINER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::FORAGER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::HUNTER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));

    set = SET::getSet(SkillSets::JESTER);
    ASSERT_EQ(set.size(), 9);
    ASSERT_TRUE(f(set));
}

TEST(SkillSetsTests, to_string) {
    ASSERT_EQ(SET::to_string(SkillSets::TRAVELER), "Traveler");
    ASSERT_EQ(SET::to_string(SkillSets::WARRIOR), "Warrior");
    ASSERT_EQ(SET::to_string(SkillSets::BRAWLER), "Brawler");
    ASSERT_EQ(SET::to_string(SkillSets::SCOUT), "Scout");
    ASSERT_EQ(SET::to_string(SkillSets::LUMBERJACK), "Lumberjack");
    ASSERT_EQ(SET::to_string(SkillSets::MINER), "Miner");
    ASSERT_EQ(SET::to_string(SkillSets::FORAGER), "Forager");
    ASSERT_EQ(SET::to_string(SkillSets::HUNTER), "Hunter");
    ASSERT_EQ(SET::to_string(SkillSets::JESTER), "Jester");
}

TEST(SkillSetsTests, to_skillset) {
    ASSERT_EQ(SET::to_skillset("Traveler"), SkillSets::TRAVELER);
    ASSERT_EQ(SET::to_skillset("Warrior"), SkillSets::WARRIOR);
    ASSERT_EQ(SET::to_skillset("Brawler"), SkillSets::BRAWLER);
    ASSERT_EQ(SET::to_skillset("Scout"), SkillSets::SCOUT);
    ASSERT_EQ(SET::to_skillset("Lumberjack"), SkillSets::LUMBERJACK);
    ASSERT_EQ(SET::to_skillset("Miner"), SkillSets::MINER);
    ASSERT_EQ(SET::to_skillset("Forager"), SkillSets::FORAGER);
    ASSERT_EQ(SET::to_skillset("Hunter"), SkillSets::HUNTER);
    ASSERT_EQ(SET::to_skillset("Jester"), SkillSets::JESTER);
}