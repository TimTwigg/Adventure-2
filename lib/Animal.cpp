// updated 24 May 2024

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <sstream>
#include "Animal.hpp"
#include "Entity.hpp"
#include "Object.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "Resource.hpp"
#include "Formulae.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;

Animal::Animal(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Animal: animal name required");
    json data = FileReader::getFromFile("animals.json", name);
    this->name = name;
    this->type = ENT::ANIMAL;
    this->value = data["value"].get<int>();
    this->maxHP = data["health"].get<int>();
    this->hp = maxHP;
    this->damage = data["damage"].get<int>();
    this->xp = data["xp"].get<int>();
    this->drops = data["drops"].get<std::map<std::string, int>>();
}

double Animal::attack() noexcept {
    return Formula::animalDamage(damage, gen);
}

bool Animal::attack(double dmg) noexcept {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Object>> Animal::getDrops() noexcept {
    std::vector<std::shared_ptr<Object>> v;
    for (const std::pair<std::string, int>& item : this->drops) {
        if (gen.getRandBool()) v.push_back(std::shared_ptr<Object>(new Resource(item.first, gen.getRandInt(1, item.second))));
    }
    return v;
}

Animal* Animal::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }
    Animal* a = new Animal(v[1]);
    a->hp = std::stof(v[2]);
    return a;
}