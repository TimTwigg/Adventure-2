// updated 4 August 2023

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Weapon.hpp"
#include "FileReader.hpp"
#include "AdventureException.hpp"
#include "StringHelpers.hpp"
#include "json.hpp"
using json = nlohmann::json;

Weapon::Weapon(std::string name) {
    strHelp::format(name);
    if (name.size() < 1) throw AdventureException("Weapon: weapon name required");
    json data = FileReader::getFromFile("weapons.json", name);
    this->value = data["value"].get<int>();
    this->name = name;
    this->weight = data["weight"].get<int>();
    this->type = OBJCLASS::WEAPON;
    recipe = data["recipe"].get<std::map<std::string, unsigned int>>();
    uses = data["hitpoints"].get<int>();
    category = data["type"].get<std::string>();
    dmg = (category == "melee") ? data["m-damage"].get<double>() : data["r-damage"].get<double>();
    ammo = (category == "ranged") ? data["ammo"].get<std::string>() : "None";
}

Weapon::operator std::string() const noexcept {
    return "WEAPON, " + name + ", " + std::to_string(uses);
}

std::map<std::string, unsigned int> Weapon::getRecipe() const noexcept {
    return recipe;
}

int Weapon::getUses() const noexcept {
    return uses;
}

double Weapon::getDamage() const noexcept {
    return dmg;
}

std::string Weapon::weaponType() const noexcept {
    return category;
}

std::string Weapon::ammoType() const noexcept {
    return ammo;
}

void Weapon::use() {
    if (uses < 1) throw AdventureException("Weapon::use weapon is already broken");
    --uses;
}

Weapon* Weapon::fromString(const std::string& code) {
    std::stringstream ss{code};
    std::vector<std::string> v;
    while (ss.good()) {
        std::string s;
        std::getline(ss, s, ',');
        v.push_back(s);
    }

    Weapon* w = new Weapon(v[1]);
    w->uses = std::stoi(v[2]);
    return w;
}