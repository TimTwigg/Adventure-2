// updated 26 May 2024

#include <gtest/gtest.h>
#include <map>
#include <iostream>
#include "Factory.hpp"
#include "AdventureException.hpp"
#include "Object.hpp"
#include "Entity.hpp"

TEST(factoryTests, constructor) {
    Factory f = Factory();
}

TEST(factoryTests, inIndex) {
    Factory f = Factory();
    ASSERT_TRUE(f.inIndex("wood"));
    ASSERT_TRUE(f.inIndex("stone-axe"));
    ASSERT_TRUE(f.inIndex("snake"));
    ASSERT_FALSE(f.inIndex("notInIndex"));
}

TEST(factoryTests, getTypeOf) {
    Factory f = Factory();
    ASSERT_EQ(f.getTypeOf("snake"), FactoryType::Animal);
    ASSERT_EQ(f.getTypeOf("hut"), FactoryType::Civ);
    ASSERT_EQ(f.getTypeOf("bottle"), FactoryType::Container);
    ASSERT_EQ(f.getTypeOf("saddle"), FactoryType::CraftableResource);
    ASSERT_EQ(f.getTypeOf("orc"), FactoryType::Enemy);
    ASSERT_EQ(f.getTypeOf("wood"), FactoryType::Resource);
    ASSERT_EQ(f.getTypeOf("stone-axe"), FactoryType::Tool);
    ASSERT_EQ(f.getTypeOf("notInIndex"), FactoryType::None);
}

TEST(factoryTests, getFileOf) {
    Factory f = Factory();
    ASSERT_EQ(f.getFileOf("snake"), "animals.json");
    ASSERT_EQ(f.getFileOf("hut"), "civ-types.json");
    ASSERT_EQ(f.getFileOf("bottle"), "containers.json");
    ASSERT_EQ(f.getFileOf("saddle"), "craftableResources.json");
    ASSERT_EQ(f.getFileOf("orc"), "enemies.json");
    ASSERT_EQ(f.getFileOf("wood"), "resources.json");
    ASSERT_EQ(f.getFileOf("stone-axe"), "tools.json");
    ASSERT_THROW(f.getFileOf("notInIndex"), AdventureException);
}

TEST(factoryTests, make) {
    Factory f = Factory();
    Object* t = static_cast<Object*>(f.make("wood"));
    ASSERT_EQ(t->getType(), OBJCLASS::RESOURCE);
    delete t;

    t = static_cast<Object*>(f.make("stone-axe"));
    ASSERT_EQ(t->getType(), OBJCLASS::TOOL);
    delete t;

    Entity* e = static_cast<Entity*>(f.make("snake"));
    ASSERT_EQ(e->getType(), ENT::ANIMAL);
    delete e;

    t = static_cast<Object*>(f.make("notInIndex"));
    ASSERT_FALSE(t);
}

TEST(factoryTests, makeFromCode) {
    Factory f = Factory();
    Object* t = static_cast<Object*>(f.makeFromCode("RESOURCE, metal-ore, 1"));
    ASSERT_EQ(t->getType(), OBJCLASS::RESOURCE);
    ASSERT_EQ(t->getName(), "metal-ore");
    delete t;

    t = static_cast<Object*>(f.makeFromCode("TOOL, stone-axe, 1"));
    ASSERT_EQ(t->getType(), OBJCLASS::TOOL);
    ASSERT_EQ(t->getName(), "stone-axe");
    delete t;
}