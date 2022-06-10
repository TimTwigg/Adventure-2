// updated 9 June 2022

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
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

unsigned int Animal::attack() noexcept {
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
    for (const auto& obj : drops) {
        OBJCLASS type = obj->getType();
        if (gen.getRandBool()) {
            if (type == OBJCLASS::RESOURCE) {
                Resource* r = static_cast<Resource*>(obj.get());
                int num = gen.getRandInt(1, r->getCount());
                v.push_back(std::shared_ptr<Object>(new Resource(r->getName(), num)));
            }
            else {
                v.push_back(std::shared_ptr<Object>(obj));
            }
        }
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