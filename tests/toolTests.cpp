// updated 6 April 2022

#include <gtest/gtest.h>
#include <string>
#include <map>
#include "Tool.hpp"
#include "AdventureException.hpp"

TEST(toolTests, constructor) {
    Tool t = Tool("wood-axe");
    Tool t2 = Tool("Metal-PICK");
    ASSERT_THROW(Tool(""), AdventureException);

    Tool t3 = Tool("flute");
}

TEST(toolTests, getters) {
    Tool t = Tool(" stone-axe  ");
    ASSERT_EQ(t.getName(), "stone-axe");
    ASSERT_EQ(t.getType(), OBJCLASS::TOOL);

    Tool t2 = Tool("metal-PICK  ");
    ASSERT_EQ(t2.getName(), "metal-pick");
    ASSERT_EQ(t2.getType(), OBJCLASS::TOOL);

    std::map<std::string, unsigned int> recipe = t.getRecipe();
    ASSERT_GT(recipe.size(), 0);
}

TEST(toolTests, operatorString) {
    Tool t = Tool("metal-axe");
    std::string s = t;
    ASSERT_EQ(s, "TOOL, metal-axe, 20");
}