// updated 26 May 2024

#include <gtest/gtest.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

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

TEST(fileReaderTests, getTitlesFromFile) {
    std::vector<std::string> v = FileReader::getTitlesFromFile("skillsets.json");
    ASSERT_EQ(v.size(), 9);
    std::vector<std::string> keys{{"Brawler", "Forager", "Hunter", "Jester", "Lumberjack", "Miner", "Scout", "Traveler", "Warrior"}};
    ASSERT_EQ(v, keys);
}

TEST(fileReaderTests, getSaveFileNames) {
    if (!fs::exists("saves/test1")) fs::create_directory("saves/test1");
    std::vector<std::string> v = FileReader::getSaveFileNames();
    ASSERT_GT(v.size(), 0);
    ASSERT_NE(std::find(v.begin(), v.end(), "test1"), v.end());
    fs::remove("saves/test1");
}

TEST(fileReaderTests, getConfigs) {
    json data = {
        {"test1", "test1Val"}
    };
    std::ofstream ofs("config.game");
    ofs << data.dump(4) << std::endl;
    ofs.close();

    json j = FileReader::getConfigs(".");
    ASSERT_EQ(j.size(), 1);
    ASSERT_EQ(j["test1"].get<std::string>(), "test1Val");

    fs::remove("config.game");
}

TEST(fileReaderTests, getInfoData) {
    std::string helpText = FileReader::getInfoData("help.txt");
    ASSERT_GT(helpText.size(), 0);
}

TEST(fileReaderTests, readFileByLine) {
    std::string text = "This is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\n";
    std::ofstream ofs("test.txt");
    ofs << text;
    ofs.close();

    std::vector<std::string> v = FileReader::readFileByLine("test.txt");
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(v[0], "This is a test");

    fs::remove("test.txt");
}