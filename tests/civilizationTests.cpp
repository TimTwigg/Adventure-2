// updated 18 June 2022

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

TEST(civTests, saveload) {
    Civilization c{"town"};
    json j = c.save();
    ASSERT_EQ(j.size(), 7);

    // loading
}