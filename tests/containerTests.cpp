// updated 7 June 2024

#include <gtest/gtest.h>
#include <map>
#include "AdventureException.hpp"
#include "Container.hpp"
#include "Liquid.hpp"

TEST(containerTests, constructor) {
    Container c = Container("bucket");
}

TEST(containerTests, getters) {
    Container c = Container("bucket");
    ASSERT_EQ(c.getName(), "bucket");
    ASSERT_EQ(c.getContentName(), "empty");
    auto recipe = c.getRecipe();
    ASSERT_GT(recipe.size(), 0);
    ASSERT_TRUE(c.isEmpty());

    c = Container("bucket", Liquid("water"));
    ASSERT_EQ(c.getContentName(), "water");
    ASSERT_FALSE(c.isEmpty());
}

TEST(containerTests, fromString) {
    Container* c = Container::fromString("CONTAINER, bucket, LIQUID, water, 3");
    ASSERT_EQ(c->getName(), "bucket");
    ASSERT_EQ(c->getContentName(), "water");
    ASSERT_FALSE(c->isEmpty());
    delete c;
}

TEST(containerTests, fill) {
    Container c = Container("bucket");
    ASSERT_TRUE(c.isEmpty());
    c.fill("milk");
    ASSERT_FALSE(c.isEmpty());
    ASSERT_EQ(c.getContentName(), "milk");
}

TEST(containerTests, fullContainer) {
    Container c = Container("bucket of water");
    ASSERT_FALSE(c.isEmpty());
    ASSERT_EQ(c.getContentName(), "water");

    c = Container("vial of milk");
    ASSERT_FALSE(c.isEmpty());
    ASSERT_EQ(c.getContentName(), "milk");
}