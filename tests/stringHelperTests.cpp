// updated 17 June 2022

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "StringHelpers.hpp"
namespace sh = strHelp;

TEST(stringHelperTests, strip) {
    std::string s = "";
    sh::strip(s);
    ASSERT_EQ(s, "");
    s = " ";
    sh::strip(s);
    ASSERT_EQ(s, "");
    s = " h e llo worLd";
    sh::strip(s);
    ASSERT_EQ(s, "helloworLd");
}

TEST(stringHelperTests, format) {
    std::string s = "";
    sh::format(s);
    ASSERT_EQ(s, "");
    s = " ";
    sh::format(s);
    ASSERT_EQ(s, "");
    s = "This is a sentence.";
    sh::format(s);
    ASSERT_EQ(s, "thisisasentence.");
}

TEST(stringHelperTests, isVowel) {
    ASSERT_TRUE(sh::isVowel('a'));
    ASSERT_TRUE(sh::isVowel('e'));
    ASSERT_TRUE(sh::isVowel('i'));
    ASSERT_TRUE(sh::isVowel('o'));
    ASSERT_TRUE(sh::isVowel('u'));
    ASSERT_TRUE(sh::isVowel('A'));
    ASSERT_TRUE(sh::isVowel('E'));
    ASSERT_TRUE(sh::isVowel('I'));
    ASSERT_TRUE(sh::isVowel('O'));
    ASSERT_TRUE(sh::isVowel('U'));

    ASSERT_FALSE(sh::isVowel('.'));
    ASSERT_FALSE(sh::isVowel(' '));
    ASSERT_FALSE(sh::isVowel('b'));
    ASSERT_FALSE(sh::isVowel('B'));
    ASSERT_FALSE(sh::isVowel('d'));
    ASSERT_FALSE(sh::isVowel('D'));
    ASSERT_FALSE(sh::isVowel('F'));
    ASSERT_FALSE(sh::isVowel('G'));
    ASSERT_FALSE(sh::isVowel('Z'));
    ASSERT_FALSE(sh::isVowel('Y'));
    ASSERT_FALSE(sh::isVowel('Z'));
    ASSERT_FALSE(sh::isVowel('z'));
    ASSERT_FALSE(sh::isVowel('y'));
    ASSERT_FALSE(sh::isVowel('p'));
}

TEST(stringHelperTests, lower) {
    std::string s = "";
    sh::lower(s);
    ASSERT_EQ(s, "");
    s = " ";
    sh::lower(s);
    ASSERT_EQ(s, " ");
    s = "Hello WORLd";
    sh::lower(s);
    ASSERT_EQ(s, "hello world");
}

TEST(stringHelperTests, split) {
    std::string s = "";
    auto v = sh::split(s);
    ASSERT_EQ(v.size(), 0);
    s = " ";
    v = sh::split(s);
    ASSERT_EQ(v.size(), 0);
    s = "word one";
    v = sh::split(s);
    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(v[0], "word");
    ASSERT_EQ(v[1], "one");
}

TEST(stringHelperTests, reduce) {
    std::vector<std::string> v{{"", "this", "with", "fill", "the", "bottle", "from", "the", "river"}};
    auto out = sh::reduce(v);
    ASSERT_EQ(out.size(), 3);
    ASSERT_EQ(out[0], "fill");
    ASSERT_EQ(out[1], "bottle");
    ASSERT_EQ(out[2], "river");
}