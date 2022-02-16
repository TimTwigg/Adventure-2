// updated 15 February 2022

#include <fstream>
#include <string>
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "json.hpp"
using json = nlohmann::json;

namespace {
    void strip(std::string& s) {
        s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    }
}

json FileReader::readFile(std::string filename) {
    strip(filename);
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
    strip(filename);
    strip(itemname);
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