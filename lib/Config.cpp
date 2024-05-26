// updated 26 May 2024

#include <vector>
#include <sstream>
#include <algorithm>
#include "Config.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"

Config::Config(std::string configFile) {
    std::vector<std::string> lines = FileReader::readFileByLine(configFile);
    std::for_each(lines.begin(), lines.end(), [&](std::string line) {
        char firstChar = line[0];
        if (firstChar == '[' || firstChar == ';');
        else configValues.push_back(parseLine(line));
    });
}

std::string Config::getStringOption(std::string key) const noexcept {
    std::string result = "";
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) result = cv.strValue;
    });
    return result;
}

int Config::getIntOption(std::string key) const noexcept {
    int result = 0;
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) result = cv.intValue;
    });
    return result;
}

ConfigValue Config::getConfigValue(std::string key) const noexcept {
    ConfigValue result;
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) result = cv;
    });
    return result;
}

ConfigValue Config::parseLine(std::string line) {
    std::istringstream iss(line);
    std::string key;
    std::string value;
    std::getline(iss, key, '=');
    std::getline(iss, value);
    if (strHelp::isNumber(value)) {
        return ConfigValue{key, ConfigType::INT, "", std::stoi(value)};
    }
    return ConfigValue{key, ConfigType::STRING, value, 0};
}