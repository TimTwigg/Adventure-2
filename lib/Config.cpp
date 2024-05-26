// updated 26 May 2024

#include <vector>
#include <sstream>
#include <algorithm>
#include "Config.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"

Config::Config(std::string configFile = "config.ini") {
    std::vector<std::string> lines = FileReader::readFileByLine(configFile);
    std::for_each(lines.begin(), lines.end(), [&](std::string line) {
        char firstChar = line[0];
        if (firstChar == '[' || firstChar == ';');
        else configValues.push_back(parseLine(line));
    });
}

std::string Config::getStringOption(std::string key) const noexcept {
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) return cv.strValue;
    });
}

int Config::getIntOption(std::string key) const noexcept {
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) return cv.intValue;
    });
}

ConfigValue Config::getConfigValue(std::string key) const noexcept {
    std::for_each(configValues.begin(), configValues.end(), [&](ConfigValue cv) {
        if (cv.key == key) return cv;
    });
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