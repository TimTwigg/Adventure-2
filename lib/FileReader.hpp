// updated 15 February 2022

#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include "json.hpp"
using json = nlohmann::json;

namespace FileReader {
    static const std::string datapath = "data\\";
    json readFile(std::string filename);
    json getFromFile(std::string filename, std::string itemname);
}

#endif