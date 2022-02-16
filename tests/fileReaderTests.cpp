// updated 15 February 2022

#include <gtest/gtest.h>
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

TEST(fileReaderTests, readFile) {
    json j = FileReader::readFile("skillsets.json");
    ASSERT_EQ(j.size(), 9);

    ASSERT_THROW(FileReader::readFile("data.json"), AdventureException);
    ASSERT_THROW(FileReader::readFile(""), AdventureException);
    ASSERT_THROW(FileReader::readFile("  "), AdventureException);
}

TEST(fileReaderTests, getFromFile) {
    json j = FileReader::getFromFile("skillsets.json", "Traveler");
    ASSERT_GT(j.size(), 0);

    ASSERT_THROW(FileReader::getFromFile("skillsets.json", "Boxer"), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("data.json", "Food"), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("skillsets.json", ""), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("", "Food"), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("  ", "Food"), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("skillsets.json", ""), AdventureException);
    ASSERT_THROW(FileReader::getFromFile("skillsets.json", "  "), AdventureException);
}