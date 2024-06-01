// updated 1 June 2024

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Machine.hpp"
#include "StringHelpers.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

Machine::Machine(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Machine: machine name required");
    json data = FileReader::getFromFile("machines.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::MACHINE;
    requiresFuel = (data["fuel"].get<std::vector<std::string>>().size() > 0);
}

Machine::operator std::string() const noexcept {
    return "MACHINE, " + name;
}

bool Machine::fueled() const noexcept {
    return requiresFuel;
}

Machine* Machine::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }
    
    return new Machine(v[1]);
}