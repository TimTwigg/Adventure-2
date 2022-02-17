// updated 16 February

#include <gtest/gtest.h>
#include <string>
#include "Resource.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(resourceTests, constructor) {
    Resource r = Resource("gold");
    Resource r2 = Resource("metal", 5);
    ASSERT_THROW(Resource(""), AdventureException);
}

TEST(resourceTests, getters) {
    Resource r = Resource("feather", 5);
    ASSERT_EQ(r.getName(), "feather");
    ASSERT_EQ(r.getType(), OBJCLASS::RESOURCE);
    ASSERT_EQ(r.getCategory(), Category::OBJECT);
    ASSERT_EQ(r.getCount(), 5);
    ASSERT_EQ(r.getWeight(), 0);
    ASSERT_EQ(r.getCarryWeight(), 0);
    ASSERT_EQ(r.getValue(), 2);
    ASSERT_EQ(r.getTotalValue(), 10);
}

TEST(resourceTests, operatorString) {
    Resource r = Resource("Gold", 5);
    std::string s = r;
    ASSERT_EQ(s, "RESOURCE, gold, 5");

    Resource r2 = Resource("METAL");
    std::string s2 = r2;
    ASSERT_EQ(s2, "RESOURCE, metal, 1");
}

TEST(resourceTests, jsonSerializer) {
    Resource r = Resource("gold", 2);
    json j = r;
}

TEST(resourceTests, add) {
    Resource r = Resource("feather");
    ASSERT_EQ(r.getCount(), 1);
    r.add(3);
    ASSERT_EQ(r.getCount(), 4);
}