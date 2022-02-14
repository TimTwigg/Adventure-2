// updated 14 February

#include <gtest/gtest.h>
#include <string>
#include <map>
#include "Resource.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(resourceTests, constructor) {
    Resource r = Resource("gold");
    Resource r2 = Resource("iron", 5);
}

TEST(resourceTests, getters) {
    Resource r = Resource("Food", 5);
    ASSERT_EQ(r.getName(), "food");
    ASSERT_EQ(r.getType(), OBJCLASS::RESOURCE);
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