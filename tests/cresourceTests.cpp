// updated 6 April 2022

#include <gtest/gtest.h>
#include <map>
#include "CResource.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(cresourceTests, constructor) {
    CResource r{"arrow", 10};
}

TEST(cresourceTests, getRecipe) {
    CResource r{"arrow"};
    std::map<std::string, unsigned int> recipe = r.getRecipe();
    ASSERT_EQ(recipe.size(), 3);
    ASSERT_TRUE(recipe.find("wood") != recipe.end());
    ASSERT_TRUE(recipe.find("metal") != recipe.end());
    ASSERT_TRUE(recipe.find("feather") != recipe.end());
}