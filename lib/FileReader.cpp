// updated 26 May 2024

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

#include <iostream>

json FileReader::readFile(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    if (cache.find(filename) != cache.end()) {
        return cache[filename];
    }

    std::ifstream in;
    in.open(datapath + filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    json j;
    in >> j;
    in.close();
    cache[filename] = j;
    return j;
}

json FileReader::getFromFile(std::string filename, std::string itemname, std::string origin) {
    strHelp::strip(filename);
    strHelp::strip(itemname);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");
    if (itemname.size() < 1) throw AdventureException("FileReader: itemname required");

    json j;
    if (cache.find(filename) != cache.end()) {
        j = cache[filename];
    }
    else {
        std::ifstream in;
        in.open(origin + filename);
        if (!in) {
            throw AdventureException("FileReader: File not Found: " + filename);
        }
        in >> j;
        in.close();
        cache[filename] = j;
    }

    if (!j.contains(itemname)) {
        throw AdventureException("FileReader: Item not Found: \"" + itemname + "\" in File: " + filename);
    }
    return j[itemname];
}

std::vector<std::string> FileReader::getTitlesFromFile(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    json j;
    if (cache.find(filename) != cache.end()) {
        j = cache[filename];
    }
    else {
        std::ifstream in;
        in.open(datapath + filename);
        if (!in) {
            throw AdventureException("FileReader: File not Found: " + filename);
        }

        in >> j;
        in.close();
        cache[filename] = j;
    }

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

std::string FileReader::getInfoData(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    std::ifstream in;
    in.open(infodatapath + filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    std::stringstream out;
    std::string line;
    while (in) {
        line.clear();
        std::getline(in, line);
        if (line[0] == '@') {
            if (line == "@NEWLINE") out << std::endl;
            else if (line == "@COMMANDS") {
                std::ifstream inC;
                inC.open(infodatapath + "commands.txt");
                std::getline(inC, line);
                out << line;
                while (inC) {
                    line.clear();
                    std::getline(inC, line);
                    if (line == "END") break;
                    out << ", " << line;
                }
                inC.close();
            }
        }
        else out << line << " ";
    }
    in.close();
    out << std::endl;
    return out.str();
}

std::vector<std::string> FileReader::readFileByLine(std::string filename) {
    strHelp::strip(filename);
    if (filename.size() < 1) throw AdventureException("FileReader: filename required");

    if (cache.find(filename) != cache.end()) {
        return cache[filename];
    }

    std::ifstream in;
    in.open(filename);
    if (!in) {
        throw AdventureException("FileReader: File not Found: " + filename);
    }

    std::vector<std::string> out;
    std::string line;
    while (in) {
        line.clear();
        std::getline(in, line);
        if (line.size() > 0) out.push_back(line);
    }
    in.close();
    
    return out;
}