// updated 14 June 2022

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

namespace FileReader {
    static const std::string datapath = "data\\";
    json readFile(std::string filename);
    json getFromFile(std::string filename, std::string itemname);
    std::vector<std::string> getTitlesFromFile(std::string filename);
    std::vector<std::string> getSaveFileNames();
    json getConfigs(std::string path);
}

#endif