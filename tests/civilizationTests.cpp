// updated 19 June 2022

#include <gtest/gtest.h>
#include <string>
#include <map>
#include <utility>
#include "Civilization.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(civTests, constructor) {
    Civilization c{"town"};
    ASSERT_THROW(Civilization c2{"mars"}, AdventureException);
}

TEST(civTests, getters) {
    Civilization c{"town"};
    ASSERT_EQ(c.getName(), "town");
    auto trades = c.getBuyingTrades();
    ASSERT_EQ(trades.size(), 6);
    trades = c.getSellingTrades();
    ASSERT_EQ(trades.size(), 6);
    auto loot = c.getLoot();
    ASSERT_GT(loot.size(), 0);
}

TEST(civTests, saveload) {
    Civilization c{"town"};
    json j = c.save();
    ASSERT_EQ(j.size(), 7);

    Civilization* c2 = Civilization::load(j);
    ASSERT_EQ(c.getBuyingTrades(), c2->getBuyingTrades());
    ASSERT_EQ(c.getSellingTrades(), c2->getSellingTrades());
    ASSERT_EQ(c.getName(), c2->getName());
    ASSERT_EQ(c.getWealth(), c2->getWealth());
    delete c2;
}