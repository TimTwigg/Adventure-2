// updated 6 August 2022

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

namespace FileReader {
    static const std::string datapath = "data\\";
    static const std::string infodatapath = "infodata\\";
    json readFile(std::string filename);
    json getFromFile(std::string filename, std::string itemname, std::string origin = datapath);
    std::vector<std::string> getTitlesFromFile(std::string filename);
    std::vector<std::string> getSaveFileNames();
    json getConfigs(std::string path);
    std::string getInfoData(std::string filename);
}

#endif