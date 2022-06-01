// updated 31 May 2022

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <map>
#include "Animal.hpp"
#include "Entity.hpp"
#include "Object.hpp"
#include "AdventureException.hpp"
#include "FileReader.hpp"
#include "Resource.hpp"
#include "Formulae.hpp"
#include "json.hpp"
using json = nlohmann::json;

Animal::Animal(std::string name) {
    Entity::format(name);
    if (name.size() < 1) throw AdventureException("Animal: animal name required");
    json data = FileReader::getFromFile("animals.json", name);
    this->name = name;
    this->type = ENT::ANIMAL;
    this->value = data["value"].get<int>();
    this->maxHP = data["health"].get<int>();
    this->hp = maxHP;
    this->damage = data["damage"].get<int>();
    this->xp = data["xp"].get<int>();
    for (const std::pair<std::string, int>& item : data["drops"].get<std::map<std::string, int>>()) {
        this->drops.push_back(std::shared_ptr<Object>(new Resource(item.first.substr(10), item.second)));
    }
}

unsigned int Animal::attack() const noexcept {
    return Formula::animalDamage(damage);
}

bool Animal::attack(double dmg) noexcept {
    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        return true;
    }
    return false;
}

const std::vector<std::shared_ptr<Object>>& Animal::getDrops() const noexcept {
    return drops; // TODO - don't always drop all drops - choose randomly
}