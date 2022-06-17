// updated 17 June 2022

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

json FileReader::readFile(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    std::ifstream in;    
    in.open(datapath + "\\" + filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    json j;
    in >> j;
    return j;
}

json FileReader::getFromFile(std::string filename, std::string itemname) {
    strHelp::strip(filename);
    strHelp::strip(itemname);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");
    if (itemname.size() < 1) throw AdventureException("FileReader: itemname required");

    std::ifstream in;
    in.open(datapath + filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    json j;
    in >> j;

    if (!j.contains(itemname)) {
        throw AdventureException("FileReader: Item not Found: " + itemname + " in File: " + filename);
    }
    return j[itemname];
}

std::vector<std::string> FileReader::getTitlesFromFile(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    std::ifstream in;
    in.open(datapath + filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    json j;
    in >> j;

    std::vector<std::string> v;
    for (const auto& [k, _] : j.items()) v.push_back(k);
    return v;
}

std::vector<std::string> FileReader::getSaveFileNames() {
    std::vector<std::string> v;
    for (const auto& f : fs::directory_iterator("saves")) v.push_back(f.path().stem().string());
    return v;
}

json FileReader::getConfigs(std::string path) {
    if (!fs::exists(fs::path(path))) throw AdventureException("FileReader::getConfigs path does not exist");
    std::ifstream in;
    json j;
    in.open(path + "\\config.game");
    in >> j;
    in.close();
    return j;
}