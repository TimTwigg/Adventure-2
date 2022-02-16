// updated 15 February 2022
// Object is an abstract base class, but has static methods which need to be tested

#include <gtest/gtest.h>
#include <string>
#include "Object.hpp"

TEST(objectTests, strip) {
    std::string s = "  Hello World       ";
    Object::strip(s);
    ASSERT_EQ(s, "HelloWorld");
}

TEST(objectTests, format) {
    std::string s = "HEllo World    ";
    Object::format(s);
    ASSERT_EQ(s, "helloworld");
}