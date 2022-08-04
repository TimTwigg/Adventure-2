// updated 4 August 2022

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

TEST(civTests, buysell) {
    Civilization c{"city"};
    auto buying = c.getBuyingTrades();
    auto item = *buying.begin();
    c.buy(item.first.first, item.first.second);
    if (item.second.first == 1) {
        auto trades = c.getBuyingTrades();
        ASSERT_EQ(trades.find(item.first), trades.end());
    }
    else ASSERT_EQ(c.getBuyingTrades().at(item.first).first+1, item.second.first);

    auto selling = c.getSellingTrades();
    auto item2 = *selling.begin();
    c.sell(item2.first.first, item2.first.second);
    if (item2.second.first == 1) {
        auto trades = c.getSellingTrades();
        ASSERT_EQ(trades.find(item2.first), trades.end());
    }
    else ASSERT_EQ(c.getSellingTrades().at(item2.first).first+1, item2.second.first);
}

TEST(civTests, raid) {
    Civilization c{"town"};
    bool raided = c.raid(1500);
    ASSERT_FALSE(raided);
    raided = c.raid(499);
    ASSERT_FALSE(raided);
    raided = c.raid(2);
    ASSERT_TRUE(raided);
    ASSERT_TRUE(c.isRaided());
    ASSERT_FALSE(c.isLooted());
    auto loot = c.getLoot();
    ASSERT_TRUE(c.isLooted());
}