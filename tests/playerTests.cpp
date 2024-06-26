// updated 26 May 2024

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include "Player.hpp"
#include "SkillSets.hpp"
#include "AdventureException.hpp"
#include "Object.hpp"

namespace {
    std::vector<std::string> names{"level", "xp", "skillset", "health", "max_health",
        "base_damage", "fist_base_damage", "hunger", "max_hunger", "thirst", "max_thirst",
        "carry_weight", "speed", "consumption_ratio", "chopping_ratio", "mining_ratio",
        "wealth", "ratios", "diff_ratio", "time", "days"};
}

TEST(PlayerTests, Constructor) {
    Player p{SkillSets::TRAVELER, "saves\\test"};
}

TEST(PlayerTests, getStatNames) {
    Player p{SkillSets::TRAVELER, "saves\\test"};
    std::vector<std::string> v = p.getStatNames();
    ASSERT_EQ(v.size(), names.size());
    
    std::sort(v.begin(), v.end());
    std::sort(names.begin(), names.end());
    ASSERT_EQ(v, names);
}

TEST(PlayerTests, statMethods) {
    Player p{SkillSets::JESTER, "saves\\test"};
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
    Player p{SkillSets::JESTER, "saves\\test"};
    std::string savepath = p.getSavepath();
    SkillSets ss = p.getSkillset();
    ASSERT_EQ(ss, SkillSets::JESTER);
    ASSERT_EQ(savepath.substr(6,4), "test");
}

TEST(PlayerTests, wealthSetters) {
    Player p{SkillSets::BRAWLER, "saves\\test"};
    ASSERT_EQ(p.stat("wealth"), 0);

    p.addWealth(100);
    ASSERT_EQ(p.stat("wealth"), 100);

    p.removeWealth(15);
    ASSERT_EQ(p.stat("wealth"), 85);

    p.removeWealth(85);
    ASSERT_EQ(p.stat("wealth"), 0);

    ASSERT_THROW(p.removeWealth(1), AdventureException);
}

TEST(PlayerTests, saveLoadFiles) {
    Player p{SkillSets::WARRIOR, "saves\\test"};
    p.addWealth(10);
    p.save();
    Player* q = Player::load(p.getSavepath());
    std::vector<std::string> pn = p.getStatNames();
    std::vector<std::string> qn = q->getStatNames();
    ASSERT_EQ(pn, qn);

    std::vector<std::string> inv_names = p.getInvalidStatNames();
    for (auto s : pn) {
        if (std::find(inv_names.begin(), inv_names.end(), s) != inv_names.end()) continue;
        ASSERT_EQ(p.stat(s), q->stat(s));
    }

    ASSERT_EQ(p.getSkillset(), q->getSkillset());
    ASSERT_EQ(p.getSavepath(), q->getSavepath());
    delete q;

    Player a{SkillSets::TRAVELER, "saves\\test"};
    a.addItem(OBJCLASS::RESOURCE, "stone", 5);
    a.addItem(OBJCLASS::CRESOURCE, "arrow", 15);
    a.save();

    Player* b = Player::load(a.getSavepath());
    ASSERT_EQ(b->itemCount("stone"), 5);
    ASSERT_EQ(b->itemCount("arrow"), 15);
    delete b;
}

TEST(PlayerTests, healthManagers) {
    Player p{SkillSets::SCOUT, "saves\\test"};
    int total_hp = p.stat("health");
    
    p.damage(10);
    ASSERT_EQ(p.stat("health"), total_hp - 10);

    p.heal(5);
    ASSERT_EQ(p.stat("health"), total_hp - 5);

    p.heal(10);
    ASSERT_EQ(p.stat("health"), total_hp);

    p.damage(total_hp - 1);
    ASSERT_EQ(p.stat("health"), 1);
}

TEST(PlayerTests, addXP) {
    Player p{SkillSets::SCOUT, "saves\\test"};
    p.addXP(10);
    ASSERT_EQ(p.stat("xp"), 10);
}

TEST(PlayerTests, hungerManagers) {
    Player p{SkillSets::SCOUT, "saves\\test"};
    double hunger = p.stat("hunger");

    p.eat(15);
    ASSERT_EQ(p.stat("hunger"), hunger);

    p.reduceHT(10, 0);
    ASSERT_EQ(p.stat("hunger"), hunger - 10);

    p.eat(5);
    ASSERT_EQ(p.stat("hunger"), hunger - 5);

    p.reduceHT(hunger + 5, 0);
    ASSERT_EQ(p.stat("hunger"), 0);
}

TEST(PlayerTests, thirstManagers) {
    Player p{SkillSets::BRAWLER, "saves\\test"};
    double thirst = p.stat("thirst");

    p.drink(15);
    ASSERT_EQ(p.stat("thirst"), thirst);

    p.reduceHT(0, 10);
    ASSERT_EQ(p.stat("thirst"), thirst - 10);

    p.drink(5);
    ASSERT_EQ(p.stat("thirst"), thirst - 5);

    p.reduceHT(0, thirst + 5);
    ASSERT_EQ(p.stat("thirst"), 0);
}

TEST(PlayerTests, inventoryManagers) {
    Player p{SkillSets::MINER, "saves\\test"};
    ASSERT_FALSE(p.inInventory("feather"));
    ASSERT_TRUE(p.inInventory("cooked-chicken", 0));
    ASSERT_FALSE(p.inInventory("stone-pick"));

    p.addItem(OBJCLASS::RESOURCE, "feather");
    ASSERT_TRUE(p.inInventory("feather"));
    ASSERT_FALSE(p.inInventory("feather", 2));

    p.addItem(OBJCLASS::RESOURCE, "feather");
    ASSERT_TRUE(p.inInventory("feather"));
    ASSERT_TRUE(p.inInventory("feather", 2));
    ASSERT_EQ(p.itemCount("feather"), 2);

    p.removeItem("feather");
    ASSERT_EQ(p.itemCount("feather"), 1);

    p.addItem(OBJCLASS::TOOL, "stone-pick");
    p.addItem(OBJCLASS::TOOL, "stone-pick", 3);
    ASSERT_EQ(p.itemCount("stone-pick"), 4);

    ASSERT_EQ(p.itemCount("computer"), 0);
}

TEST(PlayerTests, weight) {
    Player p{SkillSets::TRAVELER, "saves\\test"};
    ASSERT_EQ(p.weight(), 0);

    p.addItem(OBJCLASS::RESOURCE, "stone"); // 10
    p.addItem(OBJCLASS::TOOL, "metal-axe"); // 20
    p.addItem(OBJCLASS::WEAPON, "bow"); // 8
    p.addItem(OBJCLASS::CRESOURCE, "arrow", 20); // 20
    p.addItem(OBJCLASS::CONTAINER, "bucket"); // 6

    ASSERT_EQ(p.weight(), 64);
}