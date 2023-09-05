// updated 5 September 2023

#include <string>
#include "Object.hpp"
#include "Liquid.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;

Liquid::Liquid(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Liquid: liquid name required");
    json data = FileReader::getFromFile("liquids.json", name);
    this->name = name;
    this->type = OBJCLASS::LIQUID;
    this->weight = data["weight"].get<int>();
    this->value = data["value"].get<int>();
    hunger = data["hunger"].get<int>();
    thirst = data["thirst"].get<int>();
    hp = data["hp"].get<int>();
}

Liquid::operator std::string() const noexcept {
    return "LIQUID, " + name;
}

int Liquid::getHunger() const noexcept {
    return hunger;
}

int Liquid::getThirst() const noexcept {
    return thirst;
}

int Liquid::getHP() const noexcept {
    return hp;
}

Liquid* Liquid::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    return new Liquid(v[1]);
}